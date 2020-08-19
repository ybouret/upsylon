

//! \file
#ifndef Y_AP_LIBRARY_INCLUDED
#define Y_AP_LIBRARY_INCLUDED 1

#include "y/yap/natural.hpp"

namespace upsylon {


    namespace yap {

        class library : public singleton<library>
        {
        public:
            Y_SINGLETON_DECL(library);

            const natural _0;
            const natural _1;
            const natural _2;
            const natural _3;
            const natural _4;
            const natural _5;
            const natural _6;
            const natural _7;
            const natural _8;
            const natural _9;
            const natural _10;
            const natural _11;
            const natural _12;
            const natural _13;
            const natural _14;
            const natural _15;
            const natural _16;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(library);
            explicit library();
            virtual ~library() throw();
        };

    }

}

#endif
