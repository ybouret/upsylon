
//! \file

#ifndef Y_CORE_STACK_INCLUDED
#define Y_CORE_STACK_INCLUDED 1

#include "y/type/args.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace core {

        //! stack of addresses
        template <typename T>
        class stack
        {
        public:
            Y_DECL_ARGS(T,type);              //!< alias
            typedef mutable_type *addr_type;  //!< alias

            //! setup
            explicit stack( type **base, const size_t size) throw() :
            count(0), slots(size), slot( (addr_type *)base )
            {
            }

            //! cleanup
            virtual ~stack() throw()
            {
                aliasing::_(count) = 0;
                aliasing::_(slots) = 0;
                slot = 0;
            }

            //! push a new address
            inline void push( type *addr ) throw()
            {
                assert(slots);
                assert(count<slots);
                slot[ aliasing::_(count)++ ] = (addr_type) addr;
            }

            //! pop last address
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

            //! peek last address
            inline const_type & peek() const throw()
            {
                assert(slots);
                assert(count>0);
                assert(count<=slots);
                return *slot[count-1];
            }


            const size_t count; //!< currently in stack
            const size_t slots; //!< maximum number of addresses



        private:
            addr_type   *slot;

            Y_DISABLE_COPY_AND_ASSIGN(stack);
        };

    }

}

#endif


