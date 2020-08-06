
//! \file

#ifndef Y_MEMORY_SMALL_OBJECT_HOARD_INCLUDED
#define Y_MEMORY_SMALL_OBJECT_HOARD_INCLUDED 1

#include "y/memory/small/hoard.hpp"
#include "y/memory/small/objects.hpp"



namespace upsylon {

    namespace memory {

        namespace small {

            template <typename HOST>
            class object_hoard : public mt<HOST>
            {
            public:
                inline explicit object_hoard(objects &_, HOST &usr) throw() :
                mt<HOST>(_.Access,usr)
                {
                }

                inline virtual ~object_hoard() throw()
                {
                }
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(object_hoard);
            };
        }
    }
}

#endif
