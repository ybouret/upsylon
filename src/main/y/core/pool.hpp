//! \file
#ifndef Y_CORE_POOL_INCLUDED
#define Y_CORE_POOL_INCLUDED 1


#include "y/core/linked.hpp"

namespace upsylon
{
    namespace core
    {
        //! sanity check for node
#define Y_CORE_CHECK_POOL_NODE(node) \
assert((node)!=NULL);                \
assert((node)->next==NULL)
        
        //! core pool
        template <typename NODE>
        class pool_of : public linked
        {
        public:
            //! default constructor
            inline explicit pool_of() throw() : linked(), head(NULL) {}

            //! destructor: pool must be empty
            inline virtual ~pool_of() throw() { assert( is_vacant() ); assert(NULL==head); }

            NODE        *head;  //!< head of pool

            //! push a valid node
            NODE *store( NODE *node ) throw()
            {
                Y_CORE_CHECK_POOL_NODE(node);
                node->next = head;
                head       = node;
                increase_size();
                return node;
            }

            //! query if size>0
            inline NODE *query() throw()
            {
                assert(NULL!=head);
                assert(has_nodes());
                NODE *node = head;
                head = head->next;
                node->next = NULL;
                decrease_size();
                Y_CORE_CHECK_POOL_NODE(node);
                return node;
            }

            //! linear search
            inline bool owns( const NODE *node ) const throw()
            {
                for( const NODE *scan = head; scan != NULL; scan = scan->next )
                {
                    if( scan == node ) return true;
                }
                return false;
            }

            //! hard reset
            inline void reset() throw() { force_no_size(); head = NULL; }

            //! no-throw swap
            inline void swap_with( pool_of<NODE> &other ) throw()
            {
                _cswap(size,other.size);
                _cswap(head,other.head);
            }

            //! reverse order
            inline void reverse() throw()
            {
                pool_of<NODE> tmp;
                while( has_nodes() ) { tmp.store( query() ); }
                swap_with(tmp);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pool_of);
        };


    }

}

#include "y/type/releasable.hpp"

namespace upsylon
{
    namespace core
    {
        //! pool of C++ NODEs
        template <typename NODE>
        class pool_of_cpp : public pool_of<NODE>, public releasable
        {
        public:
            //! constructor
            explicit pool_of_cpp() throw() : pool_of<NODE>(), releasable() {}

            //! destructor, delete NODEs
            virtual ~pool_of_cpp() throw() { release(); }

            //! proper NODEs deleting
            inline virtual void release() throw()
            {
                while(this->has_nodes())
                {
                    delete this->query();
                }
            }

            //! delete one NODE
            inline void pop() throw()
            {
                assert(this->top);
                assert(this->size>0);
                delete this->query();
            }
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pool_of_cpp);
        };
        
    }
    
}

#endif
