#include "y/graphic/ops/histogram.hpp"
#include <cstring>

namespace upsylon {

    namespace Graphic {

        Histogram:: Histogram() throw() : bin()
        {
            zero();
        }

        Histogram:: ~Histogram() throw()
        {
            zero();
        }

        void Histogram:: zero() throw()
        {
            memset( bin, 0, sizeof(bin) );
        }

        size_t &    Histogram:: operator[]( const uint8_t u ) throw()
        {
            return bin[u];
        }

        const size_t &  Histogram::operator[]( const uint8_t u ) const throw()
        {
            return bin[u];
        }

        void Histogram:: set_( const size_t *bins ) throw()
        {
            assert(bins);
            assert(bins!=bin);
            memcpy(bin,bins,sizeof(bin));
        }


        void Histogram:: set( const Histogram &H ) throw()
        {
            set_(H.bin);
        }

        void Histogram:: add_(const size_t *bins) throw()
        {
            assert(bins);
            for(size_t i=0;i<BINS;++i)
            {
                bin[i] += bins[i];
            }
        }

        void Histogram:: add( const Histogram &H ) throw()
        {
            add_( H.bin );
        }

        uint8_t Histogram:: Otsu1D() const throw()
        {

            size_t  total = 0;
            size_t  sum   = 0;
            for(unsigned t=0 ; t<256 ; t++)
            {
                const size_t n = bin[t];
                sum   += t * n;
                total += n;
            }

            size_t  sumB = 0;
            size_t  wB   = 0;

            double   varMax    = 0;
            unsigned threshold = 0;

            for (unsigned t=0 ; t<256 ; t++) {
                const size_t n = bin[t];
                wB += n;// Weight Background

                if (wB == 0)
                {
                    threshold = t;
                    continue;
                }

                const size_t wF = total - wB;                 // Weight Foreground
                if (wF == 0)
                {
                    break;
                }

                sumB += (float) (t * n);

                const double mB = double(sumB) / wB;          // Mean Background
                const double mF = double(sum - sumB) / wF;    // Mean Foreground

                // Calculate Between Class Variance
                const double varBetween = double(wB) * double(wF) * square_of(mB - mF);

                // Check if new maximum found
                if (varBetween >= varMax)
                {
                    varMax    = varBetween;
                    threshold = t;
                }
            }
            return threshold;
        }



        void Histogram:: prolog( Tiles &tiles )
        {
            tiles.cacheAcquireFor<size_t>(BINS);
        }

        void Histogram:: epilog(Tiles &tiles) throw()
        {

            set_( & tiles[0].get<size_t>(0) );
            const size_t nt = tiles.size();
            for(size_t i=1;i<nt;++i)
            {
                add_(& tiles[i].get<size_t>(0) );
            }

        }

        Histogram::Metrics Histogram:: getMetrics( const uint8_t a, const uint8_t b ) const throw()
        {
            const size_t ini   = a;
            const size_t end   = b;
            if(end<=ini)
            {
                return Metrics(bin[a],0);
            }
            else
            {
                assert(end>ini);
                size_t       num = 0;
                size_t       mu1 = 0;
                size_t       mu2 = 0;
                const size_t end = b;
                for(size_t i=ini;i<=end;++i)
                {
                    const size_t x = bin[i];
                    const size_t ix = i*x;
                    num  += x;
                    mu1  += ix;
                    mu2  += ix*x;
                }
                if(num>0)
                {
                    const double average  = double(mu1)/num;
                    const double variance = double(mu2)/num - average*average;
                    return Metrics(average,variance);
                }
                else
                {
                    return Metrics(0,0);
                }
            }
        }


    }
}


