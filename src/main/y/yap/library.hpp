

//! \file
#ifndef Y_AP_LIBRARY_INCLUDED
#define Y_AP_LIBRARY_INCLUDED 1

#include "y/yap/natural.hpp"

namespace upsylon {


    namespace yap {

        //______________________________________________________________________
        //
        //
        //! precompiled data
        //
        //______________________________________________________________________
        class library : public singleton<library>
        {
        public:
            Y_SINGLETON_DECL(library); //!< aliases

            const natural _0;  //!< 0x00
            const natural _1;  //!< 0x01
            const natural _2;  //!< 0x02
            const natural _3;  //!< 0x03
            const natural _4;  //!< 0x04
            const natural _5;  //!< 0x05
            const natural _6;  //!< 0x06
            const natural _7;  //!< 0x07
            const natural _8;  //!< 0x08
            const natural _9;  //!< 0x09
            const natural _10; //!< 0x0a
            const natural _11; //!< 0x0b
            const natural _12; //!< 0x0c
            const natural _13; //!< 0x0d
            const natural _14; //!< 0x0e
            const natural _15; //!< 0x0f
            const natural _16; //!< 0x10


        private:
            Y_DISABLE_COPY_AND_ASSIGN(library);
            explicit library();
            virtual ~library() throw();
        };

    }

}

#endif
