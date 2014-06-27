#!/bin/sh
rm *.tga
rm *.bmp

./a4 -input scene01_plane.txt  -size 200 200 -output 1.bmp
./a4 -input scene02_cube.txt  -size 200 200 -output 2.bmp
./a4 -input scene03_sphere.txt  -size 200 200 -output 3.bmp
./a4 -input scene04_axes.txt  -size 200 200 -output 4.bmp
./a4 -input scene05_bunny_200.txt  -size 200 200 -output 5.bmp
./a4 -input scene06_bunny_1k.txt  -size 200 200 -output 6.bmp
./a4 -input scene07_shine.txt  -size 200 200 -output 7.bmp
./a4 -input scene08_c.txt -size 200 200 -output 8.bmp
./a4 -input scene09_s.txt -size 200 200 -output 9.bmp
