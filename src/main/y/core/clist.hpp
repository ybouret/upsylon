//! \file
#ifndef Y_CORE_CLIST_INCLUDED
#define Y_CORE_CLIST_INCLUDED 1

#include "y/type/cswap.hpp"

namespace upsylon {
    
    namespace core {
        
        enum clist_node
        {
            clist_next,
            clist_prev
        };
        
        //! check provided node is valid
#define Y_CORE_CHECK_CLIST_NODE(node) \
assert((node)!=NULL);                 \
assert((node)->next==NULL);           \
assert((node)->prev==NULL)
        
        
        template <typename NODE>
        class clist_of
        {
        public:
            
            NODE        *base;
            const size_t size;
            
            //! setup
            inline explicit clist_of() throw() : base(0), size(0) {}
            
            //!cleanup
            inline virtual ~clist_of() throw()
            {
                assert(0==size);
                assert(0==base);
            }
            
            //! hard reset
            inline void reset() throw() { base=0; (size_t&)size=0; }
            
            //! insert node after base
            inline NODE *push_back( NODE *node ) throw()
            {
                Y_CORE_CHECK_CLIST_NODE(node);
                assert(!owns(node));
                switch(size)
                {
                    case 0: push_first(node); break;
                    default: {
                        assert(base);
                        NODE *next = base->next;
                        base->next = node; node->prev=base;
                        next->prev = node; node->next=next;
                        ++(size_t&)size;
                    }  break;
                }
                return node;
            }
            
            //! insert node before base
            inline NODE *push_front( NODE *node ) throw()
            {
                Y_CORE_CHECK_CLIST_NODE(node);
                assert(!owns(node));
                switch(size)
                {
                    case 0:  push_first(node); break;
                    default: {
                        assert(base);
                        NODE *prev = base->prev;
                        base->prev = node; node->next=base;
                        prev->next = node; node->prev=prev;
                        ++(size_t&)size;
                    } break;
                }
                return node;
            }
            
            //! check if node is owned
            inline bool owns( const NODE *node ) const throw()
            {
                assert(node!=NULL);
                const NODE *scan = base;
                for(size_t i=size;i>0;--i,scan=scan->next)
                {
                    if(scan==node) return true;
                }
                return false;
            }
            
            //! remove base, affect to next
            inline NODE *pop_back() throw()
            {
                return pop_base_and_set(clist_next);
            }
            
            //! remove base, affect to prev
            inline NODE *pop_front() throw()
            {
                return pop_base_and_set(clist_prev);
            }
            
            inline clist_of & scroll_forward(  size_t n ) throw()
            {
                assert(size>0);
                n %= size;
                while(n-->0)
                {
                    base=base->next;
                }
                return *this;
            }
            
            inline clist_of & scroll_reverse(  size_t n ) throw()
            {
                assert(size>0);
                n %= size;
                while(n-->0)
                {
                    base=base->prev;
                }
                return *this;
            }
            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(clist_of);
            void push_first(NODE *node) throw()
            {
                base          = node;
                (size_t&)size = 1;
                node->next    = node->prev = base;
            }
            
            NODE *pop_last() throw()
            {
                assert(1==size);
                assert(0!=base);
                assert(base==base->next);
                assert(base==base->prev);
                base->next = 0;
                base->prev = 0;
                NODE *node = base;
                reset();
                return node;
            }
            
            NODE *pop_base_and_set( const clist_node choice ) throw()
            {
                assert(size>0);
                if(1==size)
                {
                    return pop_last();
                }
                else
                {
                    assert(size>1);
                    NODE *node = base;
                    NODE *next = node->next;
                    NODE *prev = node->prev;
                    next->prev = prev;
                    prev->next = next;
                    switch (choice)
                    {
                        case clist_next: base=next; break;
                        case clist_prev: base=prev; break;
                    }
                    --(size_t&)size;
                    return node;
                }
            }
            
            
        };
        
    }
    
}
#endif

