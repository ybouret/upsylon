
//! \file
#ifndef Y_CORE_ROLL_INCLUDED
#define Y_CORE_ROLL_INCLUDED 1

#include "y/core/linked.hpp"
#include "y/type/cswap.hpp"

namespace upsylon
{

    namespace core
    {
        //! check provided node is valid
#define Y_CORE_CHECK_ROLL_NODE(node) \
assert((node)!=NULL);                \
assert((node)->next==NULL)

        //! roll of nodes
        /**
         using pointers and size_t, with virtual interface, should be
         a power of 2 in size
         */
        template <typename NODE>
        class roll_of : public linked
        {
        public:
            typedef NODE node_type; //!< keep track of NODE

            //! constructor
            inline explicit roll_of() throw() : linked(), head(NULL), tail(NULL) {}

            //! destructor
            inline virtual ~roll_of() throw() { assert(is_vacant()); assert(NULL==head); assert(NULL==tail); }

            NODE        *head; //!< head NODE
            NODE        *tail; //!< last NODE


            //! no-throw swap with another list
            inline void swap_with(roll_of &other) throw()
            {
                _cswap(head,other.head);
                _cswap(tail,other.tail);
                _cswap(size,other.size);
            }

            //! push back a valid node
            inline NODE * push_back( NODE *node ) throw()
            {
                Y_CORE_CHECK_ROLL_NODE(node);
                if(is_vacant())
                {
                    head = tail = node;
                }
                else
                {
                    tail->next = node;
                    tail       = node;
                }
                increase_size();
                return node;
            }

            //! push front a valid node
            inline NODE * push_front( NODE *node ) throw()
            {
                Y_CORE_CHECK_ROLL_NODE(node);
                if(is_vacant())
                {
                    head = tail = node;
                }
                else
                {
                    node->next = head;
                    head       = node;
                }
                increase_size();
                return node;
            }

            //! check ownership
            inline bool owns(const NODE *node) const throw()
            {
                assert(node);
                for(const NODE *scan=head;scan;scan=scan->next)
                {
                    if(scan==node) return true;
                }
                return false;
            }

            NODE *pop_front() throw()
            {
                assert(size>0);
                NODE *node = head; assert(node);
                if(size>1)
                {
                    assert(node->next!=0);
                    head = node->next;
                    node->next = 0;
                }
                else
                {
                    assert(node->next==0);
                    assert(tail==head);
                    head=tail=0;
                }
                decrease_size();
                return node;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(roll_of);
        };
    }
}

#endif
