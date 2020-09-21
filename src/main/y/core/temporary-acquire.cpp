
#include "y/core/temporary-acquire.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace core
    {
        temporary_acquire_:: ~temporary_acquire_() throw()
        {
            while(size>0)
            {
                addr[--aliasing::_(size)]->release();
            }
            addr=0;
            _bzset(capa);
        }

        temporary_acquire_:: temporary_acquire_(releasable **arr, const size_t num) throw() :
        addr(arr),
        capa(num),
        size(0)
        {
            assert(arr!=NULL);
            assert(num>0);
        }

        void temporary_acquire_:: record(releasable &obj) throw()
        {
            assert(size<capa);
            addr[aliasing::_(size)++] = &obj;
        }



    }

}


