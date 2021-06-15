//! \file
#ifndef Y_ADDR_LIST_INCLUDED
#define Y_ADDR_LIST_INCLUDED 1

#include "y/core/list.hpp"
#include "y/core/addr-node.hpp"

namespace upsylon
{
    namespace core
    {

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
            inline void add(T *data)
            {
                node_type *node = new node_type( (type *)data );
                this->push_back(node);
            }

            //! helper
            inline node_type *rem() throw()
            {
                assert(this->size>0);
                node_type *node = this->pop_back();
                node->addr = NULL;
                return node;
            }

            //! helper
            inline void pre(T *data)
            {
                node_type *node = new node_type( (type *)data );
                this->push_front(node);
            }

            //! helper
            inline node_type *pop() throw()
            {
                assert(this->size>0);
                node_type *node = this->pop_front();
                node->addr = NULL;
                return node;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(addr_list);
        };
    }
}
#endif

