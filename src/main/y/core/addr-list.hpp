//! \file
#ifndef Y_ADDR_LIST_INCLUDED
#define Y_ADDR_LIST_INCLUDED 1

#include "y/core/list.hpp"
#include "y/object.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    namespace core
    {
        template <typename T>
        class addr_node : public object
        {
        public:
            Y_DECL_ARGS(T,type); //!< alias
            addr_node    *next;
            addr_node    *prev;
            type         *addr;

            inline explicit addr_node( type *args ) throw() : next(0), prev(0), addr( args ) {}
            inline virtual ~addr_node() throw() {}


        private:
            Y_DISABLE_COPY_AND_ASSIGN(addr_node);
        };

        //! list of addresses
        template <typename T,template <typename> class LIST = list_of_cpp >
        class addr_list : public LIST< addr_node<T> >
        {
        public:
            typedef addr_node<T> node_type;

            inline explicit addr_list() throw() {}
            inline virtual ~addr_list() throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(addr_list);
        };
    }
}
#endif

