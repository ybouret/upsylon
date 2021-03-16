
#include "y/gfx/color/histogram.hpp"
#include "y/type/utils.hpp"
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

        size_t histogram:: private_bytes() const throw()
        {
            return sizeof(bin);
        }



        void histogram:: add(const histogram &other) throw()
        {
            add(other.bin);
        }

        bool operator==( const histogram &lhs, const histogram &rhs) throw()
        {
            for(size_t i=0;i<histogram::bins;++i)
            {
                if( lhs.bin[i]!=rhs.bin[i] ) return false;
            }
            return true;
        }

        bool operator!=( const histogram &lhs, const histogram &rhs) throw()
        {
            for(size_t i=0;i<histogram::bins;++i)
            {
                if( lhs.bin[i]!=rhs.bin[i] ) return true;
            }
            return false;
        }

        uint8_t histogram:: threshold() const throw()
        {
            size_t  tot = 0;
            double  sum = 0;
            for(size_t i=0;i<bins;++i)
            {
                const size_t h =  bin[i];
                tot += h;
                sum += double(i) * double(h);
            }

            double sumB = 0;
            size_t wB   = 0;

            float   varMax = 0;
            uint8_t Threshold = 0;

            for(int t=0 ; t<256 ; t++) {
                wB += bin[t];               // Weight Background
                if (wB<=0) continue;

                const size_t wF = tot - wB;                 // Weight Foreground
                if (wF<= 0) break;

                sumB += double(t) * double(bin[t]);

                const double mB = sumB / wB;            // Mean Background
                const double mF = (sum - sumB) / wF;    // Mean Foreground

                // Calculate Between Class Variance
                const double varBetween = double(wB) * double(wF) * square_of(mB - mF);

                // Check if new maximum found
                if (varBetween > varMax)
                {
                    varMax = varBetween;
                    Threshold = t;
                }
            }
            
            return Threshold;
        }

    }

}

#include "y/ios/ocstream.hpp"

namespace upsylon
{
    namespace  graphic
    {

        void histogram:: save(const string &filename) const
        {
            ios::ocstream fp(filename);
            for(unsigned long i=0;i<histogram::bins;++i)
            {
                fp("%lu %lu\n", i, (unsigned long)( bin[i] ) );
            }
        }
    }

}

