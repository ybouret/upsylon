//! \file
#ifndef Y_OBJECT_INCLUDED
#define Y_OBJECT_INCLUDED 1

#include "y/memory/small-object.hpp"

#ifndef Y_LIMIT_SIZE
#define Y_LIMIT_SIZE 256
#endif

#ifndef Y_CHUNK_SIZE
#define Y_CHUNK_SIZE 4096
#endif

namespace upsylon
{
    class object : public memory::small_object<Y_LIMIT_SIZE,Y_CHUNK_SIZE>
    {
    public:
        explicit object() throw();
        virtual ~object() throw();
    private:
        Y_DISABLE_COPY_AND_ASSIGN(object);
    };
}

#endif


