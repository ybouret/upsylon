//! \file
#ifndef Y_CORE_CLIST_INCLUDED
#define Y_CORE_CLIST_INCLUDED 1

#include "y/type/cswap.hpp"

namespace upsylon {

    namespace core {
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

            inline NODE *push_back( NODE *node ) throw()
            {
                Y_CORE_CHECK_CLIST_NODE(node);
                switch(size)
                {
                    case 0: push_first(node); break;
                    default:
                        break;
                }
                return node;
            }

            inline NODE *push_front( NODE *node ) throw()
            {
                Y_CORE_CHECK_CLIST_NODE(node);
                switch(size)
                {
                    case 0: push_first(node); break;
                    default:
                        break;
                }
                return node;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(clist_of);
            void push_first(NODE *node) throw()
            {
                base          = node;
                (size_t&)size = 1;
                node->next    = node->prev = base;

            }
        };

    }

}
#endif

