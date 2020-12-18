
#include "y/momentary/acquire.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace core
    {
        momentary_acquire_:: ~momentary_acquire_() throw()
        {
            release_all();
            addr=0;
            _bzset(capa);
        }


        void  momentary_acquire_:: release_all() throw()
        {
            while(size>0)
            {
                addr[--aliasing::_(size)]->release();
            }
        }

        momentary_acquire_:: momentary_acquire_(releasable **arr, const size_t num) throw() :
        addr(arr),
        capa(num),
        size(0)
        {
            assert(arr!=NULL);
            assert(num>0);
        }


        

        void momentary_acquire_:: record(releasable &obj) throw()
        {
            assert(size<capa);
            addr[aliasing::_(size)++] = &obj;
        }

        momentary_acquire_ & momentary_acquire_:: operator<<( releasable &obj) throw()
        {
            record(obj);
            return *this;
        }



    }

}


