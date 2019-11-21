//! \file

#ifndef Y_ZERO_FLUX_INDEX_INCLUDED
#define Y_ZERO_FLUX_INDEX_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace core {

        template <const unit_t OFFSET>
        class zero_flux_index
        {
        public:
            const unit_t range;
            const unit_t idmin;
            const unit_t idmax;
            const unit_t delta;

            inline explicit zero_flux_index(const unit_t n) throw() :
            range(n),
            idmin(OFFSET),
            idmax(range-1+OFFSET),
            delta(idmax+idmax)
            {
                assert(idmax-idmin+1==range);
            }


            inline virtual ~zero_flux_index() throw()
            {

            }

            inline unit_t operator()(const unit_t i) const
            {
                const zero_flux_index &self = *this;
                if(i<OFFSET)
                {
                    return self( (OFFSET+OFFSET)-i );
                }
                else if(i>idmax)
                {
                    return self(delta-i);
                }
                else
                {
                    return i;
                }
            }


        private:
            Y_DISABLE_ASSIGN(zero_flux_index);
        };
    }
}

#endif

