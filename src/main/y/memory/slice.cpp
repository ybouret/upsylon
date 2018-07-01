
#include "y/memory/slice.hpp"
#include <iostream>

namespace upsylon
{
    namespace memory
    {
        slice:: slice(void        *buffer,
                      const size_t buflen) throw() :
        entry( static_cast<block*>(buffer) ),
        guard( entry + (buflen/block_size) ),
        count(1)
        {
            assert(buflen>=2*sizeof(block_size));
            assert(guard>entry+1);

            entry->prev = 0;
            entry->from = 0; //!< aka 'free'
            entry->next = guard;
            entry->size = static_cast<size_t>( guard-(entry+1) ) * block_size;
        }

        slice::~slice() throw()
        {

        }

    }

}

#include "y/code/round.hpp"
namespace upsylon
{
    namespace memory
    {
        void * slice:: acquire(size_t &n) throw()
        {
            assert(n>0);
            for(block *b=entry;b!=guard;b=b->next)
            {
                if(!(b->from) && b->size>=n)
                {
                    //__________________________________________________________
                    //
                    // found an available block
                    //__________________________________________________________
                    const size_t available_bytes = b->size;
                    const size_t preferred_bytes = Y_ALIGN_FOR_ITEM(block,n); assert(preferred_bytes<=available_bytes);
                    const size_t remaining_bytes = available_bytes-preferred_bytes;
                    std::cerr << "required: " << n << std::endl;
                    std::cerr << "\tavailable_bytes : " << available_bytes << std::endl;
                    std::cerr << "\tpreferred_bytes : " << preferred_bytes << std::endl;
                    std::cerr << "\tremaining_bytes : " << remaining_bytes << std::endl;

                    if(remaining_bytes>=small_size)
                    {
                        std::cerr << "\t\tsplit!" << std::endl;
                    }

                    //__________________________________________________________
                    //
                    // update status
                    //__________________________________________________________
                    n       = b->size;
                    b->from = this;
                    void *addr = &b[1];
                    memset(addr,0,n);
                    --count;
                    return addr;
                }
            }
            return 0;
        }
    }

}

namespace upsylon
{
    namespace memory
    {
        void slice:: display() const
        {
            std::cerr << "<slice count='" << count << "'>" << std::endl;
            for(const block *b = entry; b!=guard; b=b->next)
            {
                const char *status = (b->from) ? "USED" : "FREE";
                std::cerr << "\t" << status << " +" << b->size << std::endl;
            }
            std::cerr << "<slice/>" << std::endl;
        }

    }
}


