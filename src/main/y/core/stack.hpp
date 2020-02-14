
//! \file

#ifndef Y_CORE_STACK_INCLUDED
#define Y_CORE_STACK_INCLUDED 1

#include "y/type/args.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace core {

        template <typename T>
        class stack
        {
        public:
            Y_DECL_ARGS(T,type);
            typedef mutable_type *addr_type;

            explicit stack( type **base, const size_t size) throw() :
            count(0), slots(size), slot( (addr_type *)base )
            {
            }

            virtual ~stack() throw()
            {
            }

            inline void push( type *addr ) throw()
            {
                assert(slots);
                assert(count<slots);
                slot[ aliasing::_(count)++ ] = (addr_type) addr;
            }

            inline type * pop() throw()
            {
                assert(slots);
                assert(count>0);
                assert(count<=slots);
                addr_type *curr = &slot[ --aliasing::_(count) ];
                type      *addr = *curr;
                *curr = 0;
                return addr;
            }

            inline const_type & peek() const throw()
            {
                assert(slots);
                assert(count>0);
                assert(count<=slots);
                return *slot[count-1];
            }


            const size_t count;
            const size_t slots;



        private:
            addr_type   *slot;

            Y_DISABLE_COPY_AND_ASSIGN(stack);
        };

    }

}

#endif


