//
// bitmap.cpp
//
// handle MS bitmap I/O. For portability, we don't use the data structure defined in Windows.h
// However, there is some strange thing, the side of our structure is different from what it 
// should though we define it in the same way as MS did. So, there is a hack, we use the hardcoded
// constanr, 14, instead of the sizeof to calculate the size of the structure.
// You are not supposed to worry about this part. However, I will appreciate if you find out the
// reason and let me know. Thanks.
//

#include "bitmap.h"

BMP_BITMAPFILEHEADER bmfh;
BMP_BITMAPINFOHEADER bmih;

unsigned char *readBMP(char *fname, int &width, int &height)
{
    FILE *file;
    BMP_DWORD pos;

    if ((file = fopen(fname, "rb")) == NULL)
	return NULL;

    //      I am doing fread( &bmfh, sizeof(BMP_BITMAPFILEHEADER), 1, file ) in a safe way. :}
    fread(&(bmfh.bfType), 2, 1, file);
    fread(&(bmfh.bfSize), 4, 1, file);
    fread(&(bmfh.bfReserved1), 2, 1, file);
    fread(&(bmfh.bfReserved2), 2, 1, file);
    fread(&(bmfh.bfOffBits), 4, 1, file);

    pos = bmfh.bfOffBits;

    fread(&bmih, sizeof(BMP_BITMAPINFOHEADER), 1, file);

    // error checking
    if (bmfh.bfType != 0x4d42) {	// "BM" actually
	return NULL;
    }
    if (bmih.biBitCount != 24)
	return NULL;
    /*
       if ( bmih.biCompression != BMP_BI_RGB ) {
       return NULL;
       }
     */
    fseek(file, pos, SEEK_SET);

    width = bmih.biWidth;
    height = bmih.biHeight;

    int padWidth = width * 3;
    int pad = 0;
    if (padWidth % 4 != 0) {
	pad = 4 - (padWidth % 4);
	padWidth += pad;
    }
    int bytes = height * padWidth;

    unsigned char *data = new unsigned char[bytes];

    int foo = fread(data, bytes, 1, file);

    if (!foo) {
	delete[]data;
	return NULL;
    }

    fclose(file);

    // shuffle bitmap data such that it is (R,G,B) tuples in row-major order
    int i, j;
    j = 0;
    unsigned char temp;
    unsigned char *in;
    unsigned char *out;

    in = data;
    out = data;

    for (j = 0; j < height; ++j) {
	for (i = 0; i < width; ++i) {
	    out[1] = in[1];
	    temp = in[2];
	    out[2] = in[0];
	    out[0] = temp;

	    in += 3;
	    out += 3;
	}
	in += pad;
    }

    return data;
}

void writeBMP(char *iname, int width, int height, unsigned char *data)
{
    int bytes, pad;
    bytes = width * 3;
    pad = (bytes % 4) ? 4 - (bytes % 4) : 0;
    bytes += pad;
    bytes *= height;

    bmfh.bfType = 0x4d42;	// "BM"
    bmfh.bfSize =
	sizeof(BMP_BITMAPFILEHEADER) + sizeof(BMP_BITMAPINFOHEADER) +
	bytes;
    bmfh.bfReserved1 = 0;
    bmfh.bfReserved2 = 0;
    bmfh.bfOffBits =		/*hack sizeof(BMP_BITMAPFILEHEADER)=14, sizeof doesn't work? */
	14 + sizeof(BMP_BITMAPINFOHEADER);

    bmih.biSize = sizeof(BMP_BITMAPINFOHEADER);
    bmih.biWidth = width;
    bmih.biHeight = height;
    bmih.biPlanes = 1;
    bmih.biBitCount = 24;
    bmih.biCompression = BMP_BI_RGB;
    bmih.biSizeImage = 0;
    bmih.biXPelsPerMeter = (int) (100 / 2.54 * 72);
    bmih.biYPelsPerMeter = (int) (100 / 2.54 * 72);
    bmih.biClrUsed = 0;
    bmih.biClrImportant = 0;

    // "w+b", not "wb" -- Eugene
    FILE *foo = fopen(iname, "w+b");

    //      fwrite(&bmfh, sizeof(BMP_BITMAPFILEHEADER), 1, foo);
    fwrite(&(bmfh.bfType), 2, 1, foo);
    fwrite(&(bmfh.bfSize), 4, 1, foo);
    fwrite(&(bmfh.bfReserved1), 2, 1, foo);
    fwrite(&(bmfh.bfReserved2), 2, 1, foo);
    fwrite(&(bmfh.bfOffBits), 4, 1, foo);

    fwrite(&bmih, sizeof(BMP_BITMAPINFOHEADER), 1, foo);

    bytes /= height;
    unsigned char *scanline = new unsigned char[bytes];
    for (int j = 0; j < height; ++j) {
	memcpy(scanline, data + j * 3 * width, bytes);
	for (int i = 0; i < width; ++i) {
	    unsigned char temp = scanline[i * 3];
	    scanline[i * 3] = scanline[i * 3 + 2];
	    scanline[i * 3 + 2] = temp;
	}
	fwrite(scanline, bytes, 1, foo);
    }

    delete[]scanline;

    fclose(foo);
}
