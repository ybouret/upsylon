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
        bytes(0),
        address(0),
        destroy(0),
        type_id(0)
        {

        }

        

        bool groove:: cpp() const throw()
        {
            if(type_id)
            {
                assert(destroy);
                return true;
            }
            else
            {
                assert(0==destroy);
                return false;
            }
        }

        const std::type_info & groove:: tid() const throw()
        {
            if(type_id)
            {
                assert(destroy);
                return *type_id;
            }
            else
            {
                assert(0==destroy);
                return typeid(null_type);
            }
        }




    }

}
