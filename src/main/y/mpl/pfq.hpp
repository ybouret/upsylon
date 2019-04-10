//! \file
#ifndef Y_MPL_PFQ_INCLUDED
#define Y_MPL_PFQ_INCLUDED

#include "y/mpl/pfd.hpp"

namespace upsylon
{
    namespace mpl
    {
        // prime factors quotient
        class pfq : public counted_object
        {
        public:
            const pfd n;
            const pfd d;

            virtual ~pfq() throw();
            explicit pfq();
            explicit pfq( const pfd     &N, const pfd     &D);
            explicit pfq( const natural &N, const natural &D);
            

            friend std::ostream & operator<<( std::ostream &, const pfq &);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pfq);
            void update();
        };
    }

}


#endif

