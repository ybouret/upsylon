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

        double Histogram:: getMetrics( const uint8_t a, const uint8_t b ) const throw()
        {
            const size_t ini   = a;
            const size_t end   = b;

            if(end<=ini)
            {
                return 0;
            }
            else
            {
                double n  = 0;
                double m1 = 0;
                for(size_t i=ini;i<=end;++i)
                {
                    const double ni = bin[i];
                    n  += ni;
                    m1 += i*ni;
                }
                if(n>0)
                {
                    const double mu = m1/n;
                    double       s2 = 0;
                    for(size_t i=ini;i<=end;++i)
                    {
                        const double d = double(i)-mu;
                        s2 += d*d*bin[i];
                    }
                    return (mu*s2)/n;
                }
                else
                {
                    return 0;
                }
            }

#if 0
            if(end<=ini)
            {
                return 0;
            }
            else
            {
                assert(end>ini);
                size_t  m0 = 0;
                size_t  m1 = 0;
                size_t  m2 = 0;
                for(size_t i=ini;i<=end;++i)
                {
                    assert(i<=255);
                    const size_t v0 = bin[i];
                    const size_t v1 = i * v0;
                    m0 += v0;
                    m1 += v1;
                    m2 += v1*i;
                }
               // std::cerr << "\tm(" << ini << "," << end << ") : " << m0 << "," << m1 << "," << m2 << std::endl;
                if(m0>0)
                {
                    const double M0 = double(m0);
                    const double M1 = double(m1);
                    const double M2 = double(m2);

                    return  (M1*(M0*M2-M1*M1)) / (M0*M0*M0);
                }
                else
                {
                    return 0;
                }
            }
#endif
        }


    }
}


