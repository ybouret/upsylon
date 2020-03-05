//! \file

#ifndef Y_ZERO_FLUX_INDEX_INCLUDED
#define Y_ZERO_FLUX_INDEX_INCLUDED 1

#include "y/type/block/zset.hpp"

namespace upsylon {

    namespace core {

        //! compute indices to get a null gradient on borders
        template <const unit_t OFFSET>
        class zero_flux_index
        {
        public:
            const unit_t range; //!< number of items
            const unit_t idmin; //!< OFFSET
            const unit_t idmax; //!< OFFSET+range-1
            const unit_t delta; //!< to compute upper indices

            //! setup
            inline explicit zero_flux_index(const unit_t n) throw() :
            range(n),
            idmin(OFFSET),
            idmax(range-1+OFFSET),
            delta(idmax+idmax)
            {
                assert(idmax-idmin+1==range);
            }


            //! cleanup
            inline virtual ~zero_flux_index() throw()
            {
                _bzset(range);
                _bzset(idmin);
                _bzset(idmax);
                _bzset(delta);
            }

            //! recursive index call
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
            Y_DISABLE_COPY_AND_ASSIGN(zero_flux_index);
        };
    }
}

#endif

