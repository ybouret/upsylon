
#include "y/gfx/color/histogram.hpp"
#include <cstring>

namespace upsylon
{
    namespace  graphic
    {

        histogram:: ~histogram() throw()
        {
        }

        histogram:: histogram() throw() :
        bin()
        {
            reset();
        }

        histogram:: histogram(const histogram &other) throw() :
        bin()
        {
            memcpy(bin,other.bin,sizeof(bin));
        }

        histogram &   histogram:: operator=( const histogram & other) throw()
        {
            memmove(bin, other.bin, sizeof(bin) );
            return *this;
        }

        void histogram:: reset() throw()
        {
            memset(bin,0,sizeof(bin));
        }

        size_t & histogram:: operator[](const uint8_t i) throw()
        {
            return bin[i];
        }

        const size_t & histogram:: operator[](const uint8_t i) const throw()
        {
            return bin[i];
        }

        void histogram:: add(const size_t *b) throw()
        {
            assert(b!=0);
            for(size_t i=0;i<bins;++i)
            {
                bin[i] += b[i];
            }
        }


        void histogram:: add(const histogram &other) throw()
        {
            add(other.bin);
        }


    }

}


