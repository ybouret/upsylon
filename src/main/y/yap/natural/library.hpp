

//! \file
#ifndef Y_AP_NATURAL_LIBRARY_INCLUDED
#define Y_AP_NATURAL_LIBRARY_INCLUDED 1

#include "y/yap/natural.hpp"

namespace upsylon {


    namespace yap {

        class library : public singleton<library>
        {
        public:
            Y_SINGLETON_DECL(library);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(library);
            
        };

    }

}

#endif
