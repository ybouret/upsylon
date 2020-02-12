//! \file
#ifndef Y_INFORMATION_FILTER_INCLUDED
#define Y_INFORMATION_FILTER_INCLUDED 1

#include "y/sequence/list.hpp"
#include "y/ios/istream.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {

    namespace information {


        class filter : public ios::istream, public ios::ostream
        {
        public:
            virtual ~filter() throw();

        protected:
            explicit filter() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(filter);
        };

    

    }

}

#endif

