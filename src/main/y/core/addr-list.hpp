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
            inline virtual ~addr_node() throw() {}


        private:
            Y_DISABLE_COPY_AND_ASSIGN(addr_node);
        };

        //! list of addresses
        template <typename T,template <typename> class LIST = list_of_cpp >
        class addr_list : public LIST< addr_node<T> >
        {
        public:
            Y_DECL_ARGS(T,type);            //!< alias
            typedef addr_node<T> node_type; //!< alias

            //! constructor
            inline explicit addr_list() throw() {}

            //! destructor
            inline virtual ~addr_list() throw() {}

            //! helper
            inline void append( T *data )
            {
                node_type *node = new node_type( (type *)data );
                this->push_back(node);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(addr_list);
        };
    }
}
#endif

