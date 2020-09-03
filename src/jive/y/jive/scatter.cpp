

#include "y/jive/scatter.hpp"
#include "y/code/base2.hpp"
#include "y/type/utils.hpp"
#include "y/type/block/swap.hpp"
#include "y/code/utils.hpp"
#include "y/code/ilog2.hpp"
#include <iomanip>


namespace upsylon {

    namespace Jive
    {

        Scatter::Node *Scatter::Slot:: push(Node *node) throw()
        {

            assert(node);
            assert(0==node->next);
            if(!head)
            {
                assert(!tail);
                head = tail = node;
            }
            else
            {
                assert(tail);
                tail = tail->next = node;
            }
            return node;
        }

        Scatter::Node *Scatter::Slot:: pop() throw()
        {
            assert(head);
            assert(tail);
            Node *node=head;
            if(head!=tail)
            {
                head=node->next;
                node->next = 0;
            }
            else
            {
                head=tail=0;
            }
            return node;
        }


        static const size_t          ScatterBlockSize = 256* sizeof(Scatter::Slot);
        static const size_t          ScatterBlockExp2 = ilog2<ScatterBlockSize>::value;
        static inline Scatter::Slot *ScatterAcquire()
        {
            return static_cast<Scatter::Slot *>(object::dyadic_acquire(ScatterBlockExp2));
        }

        Scatter:: Scatter() :
        slots( ScatterAcquire() )
        {
            std::cerr << "bytes=" << ScatterBlockSize << std::endl;
        }

        const Scatter::Slot &Scatter:: operator[](const uint8_t code) const throw()
        {
            return slots[code];
        }

        size_t      Scatter::  operator()(const uint8_t code) const throw()
        {
            const Slot &slot  = slots[code];
            size_t      count = 0;
            for(const Node *node=slot.head;node;node=node->next)
            {
                ++count;
            }
            return count;
        }



        Scatter:: ~Scatter() throw()
        {

            for(size_t i=0;i<256;++i)
            {
                Slot &slot = slots[i];
                while(slot.head)
                {
                    Node *node = slot.pop();
                    object::release1(node);
                }
                assert(0==slot.head);
                assert(0==slot.tail);
            }

            object::dyadic_release(slots,ScatterBlockExp2);
            slots=0;
        }

        void Scatter:: record(const void *p, const uint8_t c)
        {
            slots[c].push( object::acquire1<Node>() )->addr = p;
        }

        static inline
        void __display(std::ostream &os, const void *addr)
        {
            os << addr;
        }

        void Scatter:: display(std::ostream&os, void (*proc)(std::ostream &,const void*)) const
        {
            if(!proc) proc = __display;
            os << "<Scatter>"<< std::endl;
            for(size_t i=0;i<256;++i)
            {
                const Slot &slot = slots[i];
                const Node *node = slot.head;
                if(node)
                {
                    os << "[" << cchars::visible[i] << "] : ";
                    while(node)
                    {
                        proc(os << ' ',node->addr);
                        node=node->next;
                    }
                    os << std::endl;
                }
            }
            os << "<Scatter/>" << std::endl;
        }




        void Scatter:: record(const void *p, const Leading &fc)
        {
            for(const Interval *i=fc.head();i;i=i->next)
            {
                const unsigned upper = i->upper;
                const unsigned lower = i->lower;
                for(unsigned code=lower;code<=upper;++code)
                {
                    record(p,code);
                }
            }
        }

        void Scatter:: remove(const void *addr) throw()
        {
            for(size_t i=0;i<256;++i)
            {
                Slot &slot = slots[i];
                Slot  temp = { NULL, NULL };
                while(slot.head)
                {
                    Node *node = slot.pop();
                    if(addr==node->addr)
                    {
                        object::release1(node);
                    }
                    else
                    {
                        temp.push(node);
                    }
                }
                bswap(slot,temp);
            }
        }



    }

}
