

#include "y/jive/scatter.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/code/base2.hpp"
#include "y/type/utils.hpp"
#include "y/code/utils.hpp"
#include <iomanip>


namespace upsylon {

    namespace Jive
    {

        typedef memory::global ScatterAllocator;

        Scatter:: Scatter() :
        bytes( 256* sizeof(Slot) ),
        slots( static_cast<Slot *>(ScatterAllocator::instance().acquire(bytes)) )
        {
            std::cerr << "bytes=" << bytes << std::endl;
        }

        Scatter:: ~Scatter() throw()
        {
            static ScatterAllocator &mgr = ScatterAllocator::location();

            for(size_t i=0;i<256;++i)
            {
                Slot &slot = slots[i];
                while(slot)
                {
                    Node *node = slot;
                    slot = node->next;
                    object::release1(node);
                }
            }


            mgr.release( *(void **)&slots, bytes);
        }

        void Scatter:: record(const void *p, const uint8_t c)
        {
            Node * node = object::acquire1<Node>();
            node->addr  = p;
            Slot & slot = slots[c];
            node->next  = slot;
            slot        = node;
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
                const Node *node = slots[i];
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

        


    }

}
