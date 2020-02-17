//! \file
#ifndef Y_INFORMATION_RLE_INCLUDED
#define Y_INFORMATION_RLE_INCLUDED 1

#include "y/information/filter/queue.hpp"

namespace upsylon {

    namespace information {

        namespace RLE {

            //! data for RLE processing
            class Context : public filterQ
            {
            public:
                virtual ~Context() throw(); //!< cleanup

            protected:
                explicit Context() throw(); //!< setup last=-1
                void     restart() throw(); //!< last=-1, free()
                int      last;


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Context);
            };


      
        }

    }

}

#endif

