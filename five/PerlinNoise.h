// JAVA REFERENCE IMPLEMENTATION OF IMPROVED NOISE - COPYRIGHT 2002 KEN PERLIN.
// http://mrl.nyu.edu/~perlin/noise/
// translated to C++ for 6.837

#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <cmath>
#include <cstdio>
#include <Vector3f.h>

class PerlinNoise
{
public:

    static double noise( double x, double y, double z )
    {
        int X = (int)floor(x) & 255;                  // FIND UNIT CUBE THAT
        int Y = (int)floor(y) & 255;                  // CONTAINS POINT.
        int Z = (int)floor(z) & 255;
        x -= floor(x);                                // FIND RELATIVE X,Y,Z
        y -= floor(y);                                // OF POINT IN CUBE.
        z -= floor(z);
        double u = fade(x);                                // COMPUTE FADE CURVES
        double v = fade(y);                                // FOR EACH OF X,Y,Z.
        double w = fade(z);
        int A = p[X  ]+Y; int AA = p[A]+Z; int AB = p[A+1]+Z;      // HASH COORDINATES OF
        int B = p[X+1]+Y; int BA = p[B]+Z; int BB = p[B+1]+Z;      // THE 8 CUBE CORNERS,
        return lerp(w, lerp(v, lerp(u, grad(p[AA  ], x  , y  , z   ),  // AND ADD
                        grad(p[BA  ], x-1, y  , z   )), // BLENDED
                    lerp(u, grad(p[AB  ], x  , y-1, z   ),  // RESULTS
                        grad(p[BB  ], x-1, y-1, z   ))),// FROM  8
                lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ),  // CORNERS
                        grad(p[BA+1], x-1, y  , z-1 )), // OF CUBE
                    lerp(u, grad(p[AB+1], x  , y-1, z-1 ),
                        grad(p[BB+1], x-1, y-1, z-1 ))));
    }


    static double octaveNoise( const Vector3f& pt, int octaves )
    {
        double answer = 0;
        for (int i = 0; i < octaves; i++)
        {
            float tmp = pow(2.0f,i);
            answer += noise(tmp*pt[0],tmp*pt[1],tmp*pt[2]) / float(tmp);
        }
        return answer;
    }

private:

    static double fade( double t )
    { 
        return t * t * t * (t * (t * 6 - 15) + 10);
    }
    
    static double lerp( double t, double a, double b )
    { 
        return a + t * (b - a);
    }

    static double grad( int hash, double x, double y, double z )
    {
        int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
        double u = h<8 ? x : y;                 // INTO 12 GRADIENT DIRECTIONS.
        double v = h<4 ? y : h==12||h==14 ? x : z;
        return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
    
    }

    // permutation
    static int p[512];

};

#endif
