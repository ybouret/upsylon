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

    }
}

