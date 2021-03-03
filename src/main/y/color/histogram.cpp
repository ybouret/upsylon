
#include "y/color/histogram.hpp"
#include <cstring>

namespace upsylon
{

    color_histogram:: ~color_histogram() throw()
    {
    }

    color_histogram:: color_histogram() throw() :
    bin()
    {
        reset();
    }

    color_histogram:: color_histogram(const color_histogram &other) throw() :
    bin()
    {
        memcpy(bin,other.bin,sizeof(bin));
    }

    color_histogram &   color_histogram:: operator=( const color_histogram & other) throw()
    {
        memmove(bin, other.bin, sizeof(bin) );
        return *this;
    }

    void color_histogram:: reset() throw()
    {
        memset(bin,0,sizeof(bin));
    }

    size_t & color_histogram:: operator[](const uint8_t i) throw()
    {
        return bin[i];
    }

    const size_t & color_histogram:: operator[](const uint8_t i) const throw()
    {
        return bin[i];
    }

    void color_histogram:: add(const size_t *b) throw()
    {
        assert(b!=0);
        for(size_t i=0;i<bins;++i)
        {
            bin[i] += b[i];
        }
    }


    void color_histogram:: add(const color_histogram &other) throw()
    {
        add(other.bin);
    }

    


}


