//! \file
#ifndef Y_CORE_POOL_INCLUDED
#define Y_CORE_POOL_INCLUDED 1


#include "y/core/list.hpp"
#include "y/type/cswap.hpp"

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

            //! push a valid node on top
            NODE *store( NODE *node ) throw()
            {
                Y_CORE_CHECK_POOL_NODE(node);
                node->next = head;
                head       = node;
                increase_size();
                return node;
            }

            //! push at end
            NODE *stash(NODE *node) throw()
            {
                if(size)
                {
                    Y_CORE_CHECK_POOL_NODE(node);
                    NODE *curr = head; assert(NULL!=curr);
                    while(curr->next)
                    {
                        curr = curr->next;
                    }
                    assert(NULL!=curr);
                    assert(NULL==curr->next);
                    curr->next = node;
                    increase_size();
                    return node;
                }
                else
                {
                    return store(node);
                }
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

            //! store a list
            inline void store( core::list_of<NODE> &l ) throw()
            {
                const size_t n = l.size;
                if(n>0)
                {
                    for(NODE *node = l.head;node;node=node->next)
                    {
                        node->prev=0;
                    }
                    l.tail->next = head;
                    head         = l.head;
                    increase_size(n);
                    l.reset();
                }
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

            //! store a node, syntactic helper
            inline pool_of & operator<<( NODE *node ) throw()
            {
                (void)store(node);
                return *this;
            }

            //! func(node)
            template <typename FUNC> inline
            void yield(FUNC &func) throw()
            {
                while(size)
                {
                    NODE *node = query();
                    func(node);
                }
            }


            //! func(node,args)
            template <typename FUNC, typename ARGS> inline
            void yield(FUNC &func, ARGS &args) throw()
            {
                while(size)
                {
                    NODE *node = query();
                    func(node,args);
                }
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
                assert(this->head);
                assert(this->size>0);
                delete this->query();
            }
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pool_of_cpp);
        };
        
    }
    
}

#endif
