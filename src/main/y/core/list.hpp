//! \file
#ifndef Y_CORE_LIST_INCLUDED
#define Y_CORE_LIST_INCLUDED 1

#include "y/core/linked.hpp"
#include "y/type/cswap.hpp"

namespace upsylon
{

    namespace core
    {
        //! check provided node is valid
#define Y_CORE_CHECK_LIST_NODE(node) \
assert((node)!=NULL);                \
assert((node)->next==NULL);          \
assert((node)->prev==NULL)

        //! list of nodes
        /**
         using pointers and size_t, with virtual interface, should be
         a power of 2 in size
         */
        template <typename NODE>
        class list_of : public linked
        {
        public:
            typedef NODE node_type; //!< keep track of NODE

            //! constructor
            inline explicit list_of() throw() : linked(), head(NULL), tail(NULL) {}

            //! destructor
            inline virtual ~list_of() throw() { assert(is_vacant()); assert(NULL==head); assert(NULL==tail); }

            NODE        *head; //!< head NODE
            NODE        *tail; //!< last NODE


            //! no-throw swap with another list
            inline void swap_with(list_of &other) throw()
            {
                _cswap(head,other.head);
                _cswap(tail,other.tail);
                _cswap(size,other.size);
            }

            //! initialize with first node
#define Y_CORE_LIST_PUSH_FIRST(node)         \
assert(!head); assert(!tail); assert(!size); \
head = tail = node; increase_size()

            //! append a NODE
            inline NODE *push_back( NODE *node ) throw()
            {
                Y_CORE_CHECK_LIST_NODE(node);
                if( is_vacant() )
                {
                    Y_CORE_LIST_PUSH_FIRST(node);
                }
                else
                {
                    assert(head!=NULL);
                    assert(tail!=NULL);
                    assert(size>0);
                    node->prev = tail;
                    tail->next = node;
                    tail       = node;
                    increase_size();
                }
                return node;
            }
            
            //! prepend a NODE
            inline NODE *push_front( NODE *node ) throw()
            {
                Y_CORE_CHECK_LIST_NODE(node);
                if( is_vacant() )
                {
                    Y_CORE_LIST_PUSH_FIRST(node);
                }
                else
                {
                    assert(head!=NULL);
                    assert(tail!=NULL);
                    assert(size>0);
                    node->next = head;
                    head->prev = node;
                    head       = node;
                    increase_size();
                }
                return node;
            }
#undef Y_CORE_LIST_PUSH_FIRST

            //! remove last NODE
            inline NODE *pop_back() throw()
            {
                assert(size>0);
                assert(head!=NULL);
                assert(tail!=NULL);
                if( size <= 1 )
                {
                    return pop_last();
                }
                else
                {
                    NODE *node = tail;
                    tail = tail->prev;
                    tail->next = NULL;
                    node->prev = NULL;
                    decrease_size();
                    Y_CORE_CHECK_LIST_NODE(node);
                    return node;
                }
            }

            //! remove first NODE
            inline NODE *pop_front() throw()
            {
                assert(size>0);
                assert(head!=NULL);
                assert(tail!=NULL);
                if( size <= 1 )
                {
                    return pop_last();
                }
                else
                {
                    NODE *node = head;
                    head = head->next;
                    head->prev = NULL;
                    node->next = NULL;
                    decrease_size();
                    Y_CORE_CHECK_LIST_NODE(node);
                    return node;
                }
            }

            //! append another list content
            inline void merge_back( list_of &other ) throw()
            {
                assert( this != &other );
                while( other.size )
                {
                    push_back( other.pop_front() );
                }
            }
            

            //! prepend another list content
            inline void merge_front( list_of &other ) throw()
            {
                assert( this != &other );
                while( other.size )
                {
                    push_front( other.pop_back() );
                }
            }

            //! mostly to debug
            inline bool owns( const NODE *node ) const throw()
            {
                for( const NODE *scan = head; scan != NULL; scan = scan->next)
                {
                    if( node == scan ) return true;
                }
                return false;
            }

            //! hard reset for embedded lists
            inline void reset() throw() { head = tail = NULL; force_no_size(); }


            //! unlink a node, return its address
            inline NODE * unlink( NODE *node ) throw()
            {
                assert( owns(node) );
                if( head == node )
                {
                    return pop_front();
                }
                else
                {
                    if( tail == node )
                    {
                        return pop_back();
                    }
                    else
                    {
                        assert( size > 2 );
                        NODE *next = node->next;
                        NODE *prev = node->prev;
                        next->prev = prev;
                        prev->next = next;
                        node->next = NULL;
                        node->prev = NULL;
                        decrease_size();
                        Y_CORE_CHECK_LIST_NODE(node);
                        return node;
                    }

                }

            }
            

            //! fetch in 0..size-1
            inline NODE *fetch( size_t index ) throw()
            {
                assert( index < size );
                if( index > (size>>1) )
                {
                    NODE *node = tail;
                    size_t m = size - ++index;
                    while( m-- > 0 ) node=node->prev;
                    return node;
                }
                else
                {
                    NODE *node = head;
                    while( index-- > 0 ) node=node->next;
                    return node;
                }
            }

            //! fetch in 0..size-1, CONST
            inline const NODE *fetch( size_t index ) const throw()
            {
                assert( index < size );
                if( index > (size>>1) )
                {
                    const NODE *node = tail;
                    size_t m = size - ++index;
                    while( m-- > 0 ) node=node->prev;
                    return node;
                }
                else
                {
                    const NODE *node = head;
                    while( index-- > 0 ) node=node->next;
                    return node;
                }
            }

            //! move owned node to front
            inline NODE * move_to_front( NODE *node ) throw()
            {
                assert( owns( node ) );
                if(node==head)
                {
                    return node; //...
                }
                else
                {
                    if(node==tail)
                    {
                        assert(size>=2);
                        tail = tail->prev;
                        tail->next = NULL;
                    }
                    else
                    {
                        assert(size>=3);
                        assert(node->prev);
                        assert(node->next);
                        NODE *prev = node->prev;
                        NODE *next = node->next;
                        prev->next = next;
                        next->prev = prev;
                    }
                    node->prev = NULL;
                    node->next = head;
                    head->prev = node;
                    head = node;
                    return node;
                }
            }

            //! replace and return mine
            inline NODE* replace( NODE *mine, NODE *yours ) throw()
            {
                assert( owns(mine) );
                Y_CORE_CHECK_LIST_NODE(yours);
                if( head == mine )
                {
                    NODE *node = pop_front();
                    push_front(yours);
                    Y_CORE_CHECK_LIST_NODE(node);
                    return node;
                }
                else
                {
                    if( tail == mine )
                    {
                        NODE *node = pop_back();
                        push_back(yours);
                        Y_CORE_CHECK_LIST_NODE(node);
                        return node;
                    }
                    else
                    {
                        assert( size > 2 );
                        NODE *next  = mine->next;
                        NODE *prev  = mine->prev;
                        next->prev  = yours;
                        prev->next  = yours;
                        yours->prev = prev;
                        yours->next = next;
                        mine->next  = NULL;
                        mine->prev  = NULL;
                        Y_CORE_CHECK_LIST_NODE(mine);
                        return mine;
                    }
                }
            }

            //! index in 0..size-1
            inline size_t index_of( const NODE *node ) const throw()
            {
                assert( owns(node) );
                size_t ans = 0;
                for( const NODE *scan = head; scan!=node; scan=scan->next,++ans)
                    ;
                return ans;
            }
            
            //! reverser list order
            inline void reverse() throw()
            {
                list_of tmp;
                while(has_nodes()) tmp.push_back( pop_back() );
                swap_with(tmp);
            }
            
            //! reverse only a part
            inline void reverse_last(size_t n) throw()
            {
                assert(n<=size);
                list_of tmp;
                while(n-->0)
                {
                    tmp.push_back( pop_back() );
                }
                merge_back(tmp);
            }

            //! insertion of yours after mine
            inline NODE * insert_after(NODE *mine, NODE *yours) throw()
            {
                assert( size>0     );
                assert( owns(mine) );
                assert( NULL == yours->prev );
                assert( NULL == yours->next );
                if(tail==mine)
                {
                    push_back(yours);
                }
                else
                {
                    assert(size>1);
                    NODE *next = mine->next;
                    mine->next = yours; yours->prev = mine;
                    next->prev = yours; yours->next = next;
                    increase_size();
                }
                return yours; // which is mine, now
            }

            //! insertion of yours before mine
            inline NODE *insert_before(NODE *mine, NODE *yours) throw()
            {
                assert( size>0     );
                assert( owns(mine) );
                assert( NULL == yours->prev );
                assert( NULL == yours->next );
                if(head==mine)
                {
                    push_front(yours);
                }
                else
                {
                    assert(size>1);
                    NODE *prev = mine->prev;
                    mine->prev = yours; yours->next = mine;
                    prev->next = yours; yours->prev = prev;
                    increase_size();
                }
                return yours; // which is mine now
            }

            //! move toward tail
            inline void towards_tail(NODE *node) throw()
            {
                assert(owns(node));
                assert(size>=2);
                assert(node->next);
                NODE *next = node->next;
                insert_after(next,unlink(node));
            }

            //! move towards head
            inline void towards_head(NODE *node) throw()
            {
                assert(owns(node));
                assert(size>=2);
                assert(node->prev);
                NODE *prev = node->prev;
                insert_before(prev,unlink(node));
            }

            //! yield all FUNC(node)
            template <typename FUNC>
            void yield(FUNC &func) throw()
            {
                while(size>0)
                {
                    NODE *node = pop_back();
                    func(node);
                }
            }

            //! yield all FUNC(node,args)
            template <typename FUNC, typename ARGS>
            void yield(FUNC &func, ARGS &args ) throw()
            {
                while(size>0)
                {
                    NODE *node = pop_back();
                    func(node,args);
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(list_of);
            inline NODE *pop_last() throw()
            {
                assert(NULL!=head); assert(NULL!=tail); assert(1==size); assert(head==tail);
                Y_CORE_CHECK_LIST_NODE(head);
                Y_CORE_CHECK_LIST_NODE(tail);
                NODE *node = head;
                reset();
                return node;
            }
        };
    }


}
#include "y/type/releasable.hpp"

namespace upsylon
{
    namespace core
    {
        //! a node is a C++ object
        template <typename NODE>
        class list_of_cpp : public list_of<NODE>, public releasable
        {
        public:
            //! constructor
            explicit list_of_cpp() throw() : list_of<NODE>(), releasable() {}

            //! delete content
            inline virtual void release() throw()
            {
                release_();
            }

            //! clear on destructor
            virtual ~list_of_cpp() throw() { release_(); }

            //! valid only if a copy ctor is defined for NODE
            inline list_of_cpp( const list_of_cpp &other ) : list_of<NODE>(), releasable()
            {
                try
                {
                    for(const NODE *node=other.head;node;node=node->next)
                    {
                        this->push_back( new NODE( *node ) );
                    }
                }
                catch(...)
                {
                    release();
                    throw;
                }
            }


            //! copy and merge_back
            inline void merge_back_copy( const list_of_cpp &other)
            {
                list_of_cpp tmp(other);
                this->merge_back(tmp);
            }

            //! copy and merge front
            inline void merge_front_copy( const list_of_cpp &other)
            {
                list_of_cpp tmp(other);
                this->merge_front(tmp);
            }


        private:
            Y_DISABLE_ASSIGN(list_of_cpp);
            static inline void delete_node(NODE *node) throw() { assert(node); delete node; }

            inline void release_() throw()
            {
                this->yield(delete_node);
            }
            
        };


    }

}

namespace upsylon
{
    namespace core
    {
        //! a node is a cloneable C++ object
        template <typename NODE>
        class list_of_cloneable : public list_of<NODE>, public releasable
        {
        public:

            //! constructor
            explicit list_of_cloneable() throw() : list_of<NODE>(), releasable() {}

            //! delete content
            inline virtual void release() throw()
            {
                while(this->has_nodes())
                {
                    delete this->pop_back();
                }
            }

            //! clear on destructor
            virtual ~list_of_cloneable() throw() { release(); }

            //! valid only if a copy ctor is defined for NODE
            inline list_of_cloneable( const list_of_cloneable &other ) : list_of<NODE>(), releasable()
            {
                try
                {
                    for(const NODE *node=other.head;node;node=node->next)
                    {
                        this->push_back( node->clone() );
                    }
                }
                catch(...)
                {
                    release();
                    throw;
                }
            }

            //! copy by cloning and merge_back
            inline void merge_back_copy( const list_of_cloneable &other)
            {
                list_of_cloneable tmp(other);
                this->merge_back(tmp);
            }

            //! copy by cloning and merge front
            inline void merge_front_copy( const list_of_cloneable &other)
            {
                list_of_cloneable tmp(other);
                this->merge_front(tmp);
            }


        private:
            Y_DISABLE_ASSIGN(list_of_cloneable);
        };
    }

}


#endif
