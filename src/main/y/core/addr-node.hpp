
//! \file
#ifndef Y_ADDR_NODE_INCLUDED
#define Y_ADDR_NODE_INCLUDED 1

#include "y/object.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    namespace core
    {
        //! node to store some address
        template <typename T>
        class addr_node : public object
        {
        public:
            Y_DECL_ARGS(T,type); //!< alias
            addr_node    *next;  //!< for list
            addr_node    *prev;  //!< for list
            type         *addr;  //!< the address

            //! store the address
            inline explicit addr_node( type *args ) throw() : next(0), prev(0), addr( args ) {}
            //! destructor
            inline virtual ~addr_node() throw() { assert(0==next); assert(0==prev); addr=0; }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(addr_node);
        };
    }
}

#endif
