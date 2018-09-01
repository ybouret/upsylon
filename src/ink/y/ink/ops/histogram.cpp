#include "y/ink/ops/histogram.hpp"

namespace upsylon
{
    namespace Ink
    {
        Histogram:: ~Histogram() throw()
        {

        }

        Histogram:: Histogram() throw() : Object(), bins()
        {
            reset();
        }

        void Histogram:: append( const size_t *other_bins ) throw()
        {
            assert(other_bins);
            for(size_t i=0;i<BINS;++i)
            {
                bins[i] += other_bins[i];
            }
        }

        void Histogram:: reset() throw()
        {
            memset(bins,0,sizeof(bins));
        }

        size_t Histogram:: threshold() const throw()
        {
            size_t total = 0, sum1 = 0;
            moments(total,sum1);
            double vmax  = 0.0;
            size_t level = 0;

            size_t sumB  = 0;
            size_t wB    = 0;
            for(size_t i=0;i<BINS;++i)
            {
                wB += bins[i]; assert(wB<=total);
                const size_t wF = total - wB;
                if (wB<=0 || wF<=0) continue;
                sumB += i*bins[i]; assert(sumB<=sum1);
                const double mF    = double(sum1 - sumB)/wF;
                const double vtmp = (wB * wF) * square_of<double>( (double(sumB) / wB) - mF);
                if(vtmp>=vmax)
                {
                    vmax   = vtmp;
                    level  = i;
                }
            }
            return level;
        }
    }
}

