#include "y/memory/groove.hpp"
#include "y/type/aliasing.hpp"
#include <cstring>

namespace upsylon {

    namespace memory {



        groove:: ~groove() throw()
        {
            release();
        }

        void groove:: release() throw()
        {
            if(bytes)
            {
                assert(address);
                if(destroy)
                {
                    assert(type_id);
                    destroy(address);
                    destroy=0;
                    type_id=0;
                    memset(address,0,bytes);
                }
                else
                {
                    assert(0==type_id);
                }
                object:: operator delete(address,bytes);
                aliasing::_(bytes) = 0;
                address            = 0;
            }
            else
            {
                assert(0==address);
                assert(0==destroy);
                assert(0==type_id);
            }

            assert(0==destroy);
            assert(0==type_id);
        }

        void groove:: free() throw()
        {
            if(bytes)
            {
                assert(address);
                if(destroy)
                {
                    assert(type_id);
                    destroy(address);
                    destroy=0;
                    type_id=0;
                    memset(address,0,bytes);
                }
                else
                {
                    assert(0==type_id);
                }
            }
            else
            {
                assert(0==address);
                assert(0==destroy);
                assert(0==type_id);
            }

            assert(0==destroy);
            assert(0==type_id);
        }

        void groove:: acquire(const size_t n)
        {
            if(n>bytes)
            {
                release();
                address = object:: operator new(n);
                aliasing::_(bytes) = n;
            }
            else
            {
                free();
            }
        }


        groove:: groove() throw() :
        object(),
        inode<groove>(),
        bytes(0),
        address(0),
        destroy(0),
        type_id(0)
        {

        }

        groove:: groove(size_t n) :
        object(),
        inode<groove>(),
        bytes(n),
        address( (bytes>0) ? object:: operator new(bytes) : 0),
        destroy(0),
        type_id(0)
        {

        }


    }

}
