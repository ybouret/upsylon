//! \file

#ifndef Y_MPL_SPRP_INCLUDED
#define Y_MPL_SPRP_INCLUDED 1

#include "y/mpl/natural.hpp"

namespace upsylon
{
    namespace mpl
    {

        class sprp : public base_class
        {
        public:
            const natural n; //!< 2^s*d+1, must be odd
            const natural m; //!< n-1,     even
            const natural s; //!< exponent
            const natural d; //!< odd value

            virtual ~sprp() throw();
            sprp( const natural &value );
            sprp( const sprp    &other );

            friend std::ostream & operator<<( std::ostream &, const sprp & );

            bool is_for( const mpn &a ) const;

        private:
            Y_DISABLE_ASSIGN(sprp);
        };

    }


}

#endif

