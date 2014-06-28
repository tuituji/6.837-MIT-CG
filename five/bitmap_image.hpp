/*
***************************************************************************
*                                                                         *
*                         Platform Independent                            *
*                   Bitmap Image Reader Writer Library                    *
*                                                                         *
* Author: Arash Partow - 2002                                             *
* URL: http://www.partow.net                                              *
*                                                                         *
* Note: This library only supports 24-bits per pixel bitmap format files. *
*                                                                         *
* Copyright notice:                                                       *
* Free use of the Platform Independent Bitmap Image Reader Writer Library *
* is permitted under the guidelines and in accordance with the most       *
* current version of the Common Public License.                           *
* http://www.opensource.org/licenses/cpl1.0.php                           *
*                                                                         *
***************************************************************************
*/

#ifndef INCLUDE_BITMAP_IMAGE_HPP
#define INCLUDE_BITMAP_IMAGE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <limits>
#include <cmath>
#include <cstdlib>


struct bitmap_file_header
{
   unsigned short type;
   unsigned int   size;
   unsigned short reserved1;
   unsigned short reserved2;
   unsigned int   off_bits;

   unsigned int struct_size()
   {
      return sizeof(type)     +
            sizeof(size)      +
            sizeof(reserved1) +
            sizeof(reserved2) +
            sizeof(off_bits);
   }
};

struct bitmap_information_header
{
   unsigned int   size;
   unsigned int   width;
   unsigned int   height;
   unsigned short planes;
   unsigned short bit_count;
   unsigned int   compression;
   unsigned int   size_image;
   unsigned int   x_pels_per_meter;
   unsigned int   y_pels_per_meter;
   unsigned int   clr_used;
   unsigned int   clr_important;

   unsigned int struct_size()
   {
      return sizeof(size)             +
             sizeof(width)            +
             sizeof(height)           +
             sizeof(planes)           +
             sizeof(bit_count)        +
             sizeof(compression)      +
             sizeof(size_image)       +
             sizeof(x_pels_per_meter) +
             sizeof(y_pels_per_meter) +
             sizeof(clr_used)         +
             sizeof(clr_important);
   }
};


inline void read_bih(std::ifstream& stream,bitmap_information_header& bih);
inline void read_bfh(std::ifstream& stream, bitmap_file_header& bfh);
inline void write_bih(std::ofstream& stream, const bitmap_information_header& bih);
inline void write_bfh(std::ofstream& stream, const bitmap_file_header& bfh);
template<typename T>
   T clamp(const T& v, const T& lower_range, const T& upper_range)
   {
      if (v < lower_range)
          return lower_range;
      else if (v >  upper_range)
         return upper_range;
      else
         return v;
   }
class bitmap_image
{
public:

   enum channel_mode {
                        rgb_mode = 0,
                        bgr_mode = 1
                     };

   enum color_plane {
                       blue_plane  = 0,
                       green_plane = 1,
                       red_plane   = 2
                    };


   bitmap_image()
   : file_name_(""),
     data_(0),
     bytes_per_pixel_(3),
     length_(0),
     width_(0),
     height_(0),
     row_increment_(0),
     channel_mode_(bgr_mode)
   {}

   bitmap_image(const std::string& _filename)
   : file_name_(_filename),
     data_(0),
     bytes_per_pixel_(0),
     length_(0),
     width_(0),
     height_(0),
     row_increment_(0),
     channel_mode_(bgr_mode)
   {
      load_bitmap();
   }

   bitmap_image(const unsigned int width, const unsigned int height)
   : file_name_(""),
     data_(0),
     bytes_per_pixel_(3),
     length_(0),
     width_(width),
     height_(height),
     row_increment_(0),
     channel_mode_(bgr_mode)
   {
     create_bitmap();
   }

   bitmap_image(const bitmap_image& image)
   : file_name_(image.file_name_),
     data_(0),
     bytes_per_pixel_(3),
     width_(image.width_),
     height_(image.height_),
     row_increment_(0),
     channel_mode_(bgr_mode)
   {
      create_bitmap();
      std::copy(image.data_, image.data_ + image.length_, data_);
   }

  ~bitmap_image()
   {
      delete [] data_;
   }

   bitmap_image& operator=(const bitmap_image& image)
   {
      if (this != &image)
      {
         file_name_ = image.file_name_;
         bytes_per_pixel_ = image.bytes_per_pixel_;
         width_ = image.width_;
         height_ = image.height_;
         row_increment_ = 0;
         channel_mode_ = image.channel_mode_;
         create_bitmap();
         std::copy(image.data_, image.data_ + image.length_, data_);
      }
      return *this;
   }
   inline void get_pixel(const unsigned int x, const unsigned int y,
                         unsigned char& red,
                         unsigned char& green,
                         unsigned char& blue)
   {
      blue  = data_[(y * row_increment_) + (x * bytes_per_pixel_ + 0)];
      green = data_[(y * row_increment_) + (x * bytes_per_pixel_ + 1)];
      red   = data_[(y * row_increment_) + (x * bytes_per_pixel_ + 2)];
   }

   inline void set_pixel(const unsigned int x, const unsigned int y,
                        const unsigned char red,
                        const unsigned char green,
                        const unsigned char blue)
   {
      data_[(y * row_increment_) + (x * bytes_per_pixel_ + 0)] = blue;
      data_[(y * row_increment_) + (x * bytes_per_pixel_ + 1)] = green;
      data_[(y * row_increment_) + (x * bytes_per_pixel_ + 2)] = red;
   }

   inline bool copy_from(const bitmap_image& image)
   {
      if ((image.height_ !=  height_) ||
          (image.width_  !=  width_))
      {
         return false;
      }
      std::copy(image.data_,image.data_ + image.length_,data_);
      return true;
   }

   

   void save_image(const std::string& file_name)
   {
      std::ofstream stream(file_name.c_str(),std::ios::binary);

      if (!stream)
      {
         std::cout << "bitmap_image::save_image(): Error - Could not open file "  << file_name << " for writing!" << std::endl;
         return;
      }

      bitmap_file_header bfh;
      bitmap_information_header bih;

      bih.width            = width_;
      bih.height           = height_;
      bih.bit_count        = static_cast<unsigned short>(bytes_per_pixel_ << 3);
      bih.clr_important    =  0;
      bih.clr_used         =  0;
      bih.compression      =  0;
      bih.planes           =  1;
      bih.size             = 40;
      bih.x_pels_per_meter =  0;
      bih.y_pels_per_meter =  0;
      bih.size_image       = (((bih.width * bytes_per_pixel_) + 3) & 0xFFFC) * bih.height;

      bfh.type      = 19778;
      bfh.size      = 55 + bih.size_image;
      bfh.reserved1 = 0;
      bfh.reserved2 = 0;
      bfh.off_bits  = bih.struct_size() + bfh.struct_size();

      write_bfh(stream,bfh);
      write_bih(stream,bih);

      unsigned int padding = (4 - ((3 * width_) % 4)) % 4;
      char padding_data[4] = {0x0,0x0,0x0,0x0};
      for (unsigned int i = 0; i < height_; ++i)
      {
         unsigned char* data_ptr = data_ + (row_increment_ * (height_ - i - 1));
         stream.write(reinterpret_cast<char*>(data_ptr),sizeof(unsigned char) * bytes_per_pixel_ * width_);
         stream.write(padding_data,padding);
      }
      stream.close();
   }
	
   inline const unsigned char* data()
   {
      return data_;
   }

   int width(){return width_;}
   int height(){return height_;}

  
private:

   void create_bitmap()
   {
      length_ = width_ * height_ * bytes_per_pixel_;
      row_increment_ = width_ * bytes_per_pixel_;
      if (0 != data_)
      {
         delete[] data_;
      }
      data_ = new unsigned char[length_];
      valid_ = true;
   }
    inline unsigned char* row(unsigned int row_index) const
   {
      return data_ + (row_index * row_increment_);
   }
   void load_bitmap()
   {
      std::ifstream stream(file_name_.c_str(),std::ios::binary);
      if (!stream)
      {
         std::cerr << "bitmap_image::load_bitmap() ERROR: bitmap_image - file " << file_name_ << " not found!" << std::endl;
         return;
      }

      bitmap_file_header bfh;
      bitmap_information_header bih;

      read_bfh(stream,bfh);
      read_bih(stream,bih);

      if (bfh.type != 19778)
      {
         stream.close();
         std::cerr << "bitmap_image::load_bitmap() ERROR: bitmap_image - Invalid type value " << bfh.type << " expected 19778." << std::endl;
         return;
      }

      if (bih.bit_count != 24)
      {
         stream.close();
         std::cerr << "bitmap_image::load_bitmap() ERROR: bitmap_image - Invalid bit depth " << bih.bit_count << " expected 24." << std::endl;
         return;
      }

      height_ = bih.height;
      width_  = bih.width;

      bytes_per_pixel_ = bih.bit_count >> 3;

      unsigned int padding = (4 - ((3 * width_) % 4)) % 4;
      char padding_data[4] = {0,0,0,0};

      create_bitmap();

      for (unsigned int i = 0; i < height_; ++i)
      {
         unsigned char* data_ptr = row(height_ - i - 1); // read in inverted row order
         stream.read(reinterpret_cast<char*>(data_ptr),sizeof(char) * bytes_per_pixel_ * width_);
         stream.read(padding_data,padding);
      }

      valid_ = true;
   }

   

   bool           valid_;
   std::string    file_name_;
   unsigned char* data_;
   unsigned int   bytes_per_pixel_;
   unsigned int   length_;
   unsigned int   width_;
   unsigned int   height_;
   unsigned int   row_increment_;
   channel_mode   channel_mode_;
};

inline bool big_endian()
{
   unsigned int v = 0x01;
   return (1 != reinterpret_cast<char*>(&v)[0]);
}

inline unsigned short flip(const unsigned short& v)
{
   return ((v >> 8) | (v << 8));
}

inline unsigned int flip(const unsigned int& v)
{
   return (((v & 0xFF000000) >> 0x18) |
           ((v & 0x000000FF) << 0x18) |
           ((v & 0x00FF0000) >> 0x08) |
           ((v & 0x0000FF00) << 0x08));
}
template<typename T>
inline void read_from_stream(std::ifstream& stream,T& t)
{
   stream.read(reinterpret_cast<char*>(&t),sizeof(T));
}

template<typename T>
inline void write_to_stream(std::ofstream& stream,const T& t)
{
   stream.write(reinterpret_cast<const char*>(&t),sizeof(T));
}

inline void read_bfh(std::ifstream& stream, bitmap_file_header& bfh)
{
   read_from_stream(stream,bfh.type);
   read_from_stream(stream,bfh.size);
   read_from_stream(stream,bfh.reserved1);
   read_from_stream(stream,bfh.reserved2);
   read_from_stream(stream,bfh.off_bits);

   if (big_endian())
   {
      flip(bfh.type);
      flip(bfh.size);
      flip(bfh.reserved1);
      flip(bfh.reserved2);
      flip(bfh.off_bits);
   }
}

inline void write_bfh(std::ofstream& stream, const bitmap_file_header& bfh)
{
   if (big_endian())
   {
      flip(bfh.type);
      flip(bfh.size);
      flip(bfh.reserved1);
      flip(bfh.reserved2);
      flip(bfh.off_bits);
   }

   write_to_stream(stream,bfh.type);
   write_to_stream(stream,bfh.size);
   write_to_stream(stream,bfh.reserved1);
   write_to_stream(stream,bfh.reserved2);
   write_to_stream(stream,bfh.off_bits);
}
inline void read_bih(std::ifstream& stream,bitmap_information_header& bih)
{
   read_from_stream(stream,bih.size);
   read_from_stream(stream,bih.width);
   read_from_stream(stream,bih.height);
   read_from_stream(stream,bih.planes);
   read_from_stream(stream,bih.bit_count);
   read_from_stream(stream,bih.compression);
   read_from_stream(stream,bih.size_image);
   read_from_stream(stream,bih.x_pels_per_meter);
   read_from_stream(stream,bih.y_pels_per_meter);
   read_from_stream(stream,bih.clr_used);
   read_from_stream(stream,bih.clr_important);
   if (big_endian())
   {
      flip(bih.size);
      flip(bih.width);
      flip(bih.height);
      flip(bih.planes);
      flip(bih.bit_count);
      flip(bih.compression);
      flip(bih.size_image);
      flip(bih.x_pels_per_meter);
      flip(bih.y_pels_per_meter);
      flip(bih.clr_used);
      flip(bih.clr_important);
   }
}

inline void write_bih(std::ofstream& stream, const bitmap_information_header& bih)
{
   if (big_endian())
   {
      flip(bih.size);
      flip(bih.width);
      flip(bih.height);
      flip(bih.planes);
      flip(bih.bit_count);
      flip(bih.compression);
      flip(bih.size_image);
      flip(bih.x_pels_per_meter);
      flip(bih.y_pels_per_meter);
      flip(bih.clr_used);
      flip(bih.clr_important);
   }
   write_to_stream(stream,bih.size);
   write_to_stream(stream,bih.width);
   write_to_stream(stream,bih.height);
   write_to_stream(stream,bih.planes);
   write_to_stream(stream,bih.bit_count);
   write_to_stream(stream,bih.compression);
   write_to_stream(stream,bih.size_image);
   write_to_stream(stream,bih.x_pels_per_meter);
   write_to_stream(stream,bih.y_pels_per_meter);
   write_to_stream(stream,bih.clr_used);
   write_to_stream(stream,bih.clr_important);
}
#endif
