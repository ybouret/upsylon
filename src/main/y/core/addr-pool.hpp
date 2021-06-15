
//! \file
#ifndef Y_ADDR_POOL_INCLUDED
#define Y_ADDR_POOL_INCLUDED 1

#include "y/core/pool.hpp"
#include "y/core/addr-node.hpp"

namespace upsylon
{
    namespace core
    {

        //! list of addresses
        template <typename T,template <typename> class POOL = pool_of_cpp >
        class addr_pool : public POOL< addr_node<T> >
        {
        public:
            Y_DECL_ARGS(T,type);            //!< alias
            typedef addr_node<T> node_type; //!< alias

            //! constructor
            inline explicit addr_pool() throw() {}

            //! destructor
            inline virtual ~addr_pool() throw() {}

            //! helper
            inline void push( T *data )
            {
                node_type *node = new node_type( (type *)data );
                this->store(node);
            }

            //! helper
            inline node_type *pop() throw()
            {
                assert(this->size>0);
                node_type *node = this->query();
                node->addr      = NULL;
                return node;
            }

            //! helper
            inline node_type *pop(T *new_data) throw()
            {
                assert(this->size>0);
                node_type *node = this->query();
                node->addr      = new_data;
                return node;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(addr_pool);
        };
    }
}
#endif

