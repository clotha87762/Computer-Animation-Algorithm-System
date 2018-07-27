#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "CBmp.h"

#pragma warning(disable:4996)
#pragma pack( push, 1 )
typedef struct _bmp_header_info
{
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;

    // bitmap header
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXpelsPerMeter;
    int biYpelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} bmp_header_info;
#pragma pack( pop, 1 )

CBmp::CBmp()
{
    w = 0;
    h = 0;
    rgb = NULL;
    memset( this, 0, sizeof(CBmp) );
}

CBmp::~CBmp()
{
    if(rgb!=NULL)
        free(rgb);
}

void CBmp::load( const char *spath )
{

    bmp_header_info bhi;
    {
        FILE *f0 = fopen( spath, "rb" );
        if( f0==NULL )
        {
            printf( "[Error] CBmp::load, file %s not found.\n", spath );
            exit(-1);
        }

        fread( &bhi, sizeof(bmp_header_info), 1, f0 );
        fclose(f0);
    }

    if( bhi.bfType != 'MB' )
    {
        printf( "[Error] CBmp::load, not bitmap file\n" );
        exit(-1);
    }

    if( bhi.biCompression != 0 )
    {
        printf( "[Error] CBmp::load, only uncompressed bitmap is supported\n" );
        exit(-1);
    }

    if( bhi.biBitCount != 24 )
    {
        printf( "[Error] CBmp::load, must be RGB888 bitmap\n" );
        exit(-1);
    }

    if(rgb)
        free(rgb);

    w = bhi.biWidth;
    h = bhi.biHeight;
    rgb = (unsigned char*) malloc( w*h*3*sizeof(unsigned char) );
    {
        FILE *f0 = fopen( spath, "rb" );
        fseek( f0, bhi.bfOffBits, SEEK_SET );
        int j;
        for( j=0; j<h; j++ )
        {
            fread( &rgb[j*w*3], sizeof(unsigned char), w*3, f0 );
            fseek( f0, (4-w*3%4)%4, SEEK_CUR );
        }
        fclose(f0);
    }
    rb_swap();
}

void CBmp::load( int width, int height )
{
    if(rgb)
        free(rgb);

    w = width;
    h = height;
    rgb = (unsigned char*) malloc( w*h*3*sizeof(unsigned char) );
    memset( rgb, 0, w*h*3*sizeof(unsigned char) );
}

void CBmp::load( int width, int height, unsigned char *rgb_data )
{
    if(rgb) 
        free(rgb);

    w = width;
    h = height;
    rgb = (unsigned char*) malloc( w*h*3*sizeof(unsigned char) );
    memcpy( rgb, rgb_data, w*h*3*sizeof(unsigned char) );
}

void CBmp::flip_vectical()
{
    unsigned char* tmp_rgb = (unsigned char*) malloc( w*h*3*sizeof(unsigned char) );
    int j;
    for( j=0; j<h; j++ )
        memcpy( &tmp_rgb[j*w*3], &rgb[(h-j-1)*w*3], w*3*sizeof(unsigned char) );

    memcpy( rgb, tmp_rgb, w*h*3*sizeof(unsigned char) );

    free(tmp_rgb);
}
void CBmp::clear_white(unsigned char threshold_value)
{
    unsigned char* tmp_rgb = (unsigned char*) malloc( w*h*3*sizeof(unsigned char) );
    int i,j;

    for( j=0; j<h; j++ )
    {
        for( i=0; i<w; i++ )
        {
            if(rgb[(j*w+i)*3]   >= threshold_value &&
               rgb[(j*w+i)*3+1] >= threshold_value &&
               rgb[(j*w+i)*3+2] >= threshold_value)
            {
                tmp_rgb[(j*w+i)*3]   = 255;
                tmp_rgb[(j*w+i)*3+1] = 255;
                tmp_rgb[(j*w+i)*3+2] = 255;
            }
            else
            {
                memcpy( &tmp_rgb[(j*w+i)*3], &rgb[(j*w+i)*3], 3*sizeof(unsigned char) );
            }
        }
    }

    memcpy( rgb, tmp_rgb, w*h*3*sizeof(unsigned char) );

    free(tmp_rgb);
}

void CBmp::rb_swap()
{
    unsigned char tmp;
    int i,j;
    for( j=0; j<h; j++ )
    {
        for( i=0; i<w; i++ )
        {
            tmp = rgb[(j*w+i)*3];
            rgb[(j*w+i)*3] = rgb[(j*w+i)*3+2];
            rgb[(j*w+i)*3+2] = tmp;
        }
    }
}
void CBmp::save( const char *spath )
{
    bmp_header_info bhi;
        bhi.bfType = 'MB';
        bhi.bfSize = w*h*3*sizeof(unsigned char) + sizeof(bhi);
        bhi.bfReserved1 = 0;
        bhi.bfReserved2 = 0;
        bhi.bfOffBits = sizeof(bhi);

        bhi.biSize = 40;
        bhi.biWidth = w;
        bhi.biHeight = h;
        bhi.biPlanes = 1;
        bhi.biBitCount = 24;
        bhi.biCompression = 0;
        bhi.biSizeImage = 0;
        bhi.biXpelsPerMeter = 0;
        bhi.biYpelsPerMeter = 0;
        bhi.biClrUsed = 0;
        bhi.biClrImportant = 0;

    int j;
    CBmp a;
    a.load( w,h, rgb );
    a.rb_swap();
    unsigned char pad[3] = {0};

    FILE *f0 = fopen( spath, "wb" );
    fwrite( &bhi, sizeof(bmp_header_info), 1, f0 );
    for( j=0; j<h; j++ )
    {
        fwrite( &a.rgb[j*w*3], sizeof(unsigned char), w*3, f0 );
        fwrite( pad, sizeof(unsigned char), (4-w*3%4)%4, f0 );
    }
    fclose(f0);
}