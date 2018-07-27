#ifndef CBMP_H
#define CBMP_H

class CBmp
{
    public:
        int w,h;
        unsigned char *rgb;

        CBmp();
        ~CBmp();

        void load( const char *spath );
        void load( int width, int height );
        void load( int width, int height, unsigned char *rgb_data );
        void save( const char *spath );
        void flip_vectical();
        void clear_white(unsigned char threshold_value);

    private:
        void rb_swap();
};

#endif