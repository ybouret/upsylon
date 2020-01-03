#include "y/graphic/ops/histogram.hpp"
#include <cstring>

namespace upsylon
{

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

        size_t Histogram:: count() const throw()
        {
            size_t sum = bin[0];
            for(size_t i=1;i<BINS;++i) sum += bin[i];
            return sum;
        }

        uint8_t Histogram:: Otsu1D() const throw()
        {
            const size_t total = count();
            float sum = 0;
            for (size_t t=0 ; t<256 ; ++t) sum += t * bin[t];

            float sumB = 0;
            int   wB = 0;
            int   wF = 0;

            float varMax    = 0;
            int   threshold = 0;

            for (size_t t=0 ; t<256 ; ++t) {
                wB += bin[t];               // Weight Background
                if (wB == 0) continue;

                wF = total - wB;                 // Weight Foreground
                if (wF == 0) break;

                sumB += (float) (t * bin[t]);

                float mB = sumB / wB;            // Mean Background
                float mF = (sum - sumB) / wF;    // Mean Foreground

                // Calculate Between Class Variance
                const float varBetween = float(wB) * float(wF) * (mB - mF) * (mB - mF);

                // Check if new maximum found
                if (varBetween > varMax)
                {
                    varMax = varBetween;
                    threshold = t;
                }
            }

            return threshold;
        }


        void Histogram:: prolog( Tiles &tiles )
        {
            tiles.localAcquireFor<size_t>(BINS);
            tiles.localCleanUp();

        }

        void Histogram:: epilog(Tiles &tiles) throw()
        {
            
            set_( & tiles[0].as<size_t>(0) );
            const size_t nt = tiles.size();
            for(size_t i=1;i<nt;++i)
            {
                add_(& tiles[i].as<size_t>(0) );
            }

        }

    }
}


