#include "y/memory/groove.hpp"
#include "y/type/aliasing.hpp"
#include "y/object.hpp"
#include "y/exception.hpp"
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

        void groove:: dismiss() throw()
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

        void groove:: prepare(const size_t n)
        {
            if(n!=bytes)
            {
                release();
                if(n>0)
                {
                    address = object:: operator new(n);
                    aliasing::_(bytes) = n;
                }
            }
            else
            {
                dismiss();
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

#include "y/type/spec.hpp"
#include <iostream>

namespace upsylon {

    namespace memory {

        void groove:: check_type( const std::type_info &against ) const
        {
            const std::type_info &self = tid();
            if( self != against )
            {
                const char *me = *type_name_for(self);
                const char *it = *type_name_for(against);
                throw exception("groove types mismatch <%s> instead of <%s>",it,me);
            }
        }

#define Y_MEM_GROOVE(TYPE) do { \
if(typeid(TYPE) == tid ) { os << ':' << g.as<TYPE>(); } } while(false)

        std::ostream & operator<<(std::ostream &os, const groove &g)
        {
            const std::type_info &tid = g.tid();
            os << '(' << g.bytes << "@" << type_name_for(tid);
            Y_MEM_GROOVE(float);
            Y_MEM_GROOVE(double);
            Y_MEM_GROOVE(string);
            os << ')';
            return os;
        }



    }

}
