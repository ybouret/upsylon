
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
                        std::cerr << "\t\t...split" << std::endl;
                        //______________________________________________________
                        //
                        // insert new block
                        //______________________________________________________
                        block *t = &b[1+preferred_bytes/block_size];
                        t->prev = b;
                        t->next = b->next; assert(t->next>t+1);
                        t->size = static_cast<size_t>( t->next-(t+1) ) * block_size;
                        t->from = NULL;

                        //______________________________________________________
                        //
                        // update status
                        //______________________________________________________
                        b->next = t;
                        b->size = preferred_bytes;
                        ++count;
                    }

                    //__________________________________________________________
                    //
                    // prepare allocated
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
        void slice:: release(void *&p, size_t &n) throw()
        {
            if(p)
            {
                assert(n>0);
                block *curr = static_cast<block *>(p)-1; assert(n==curr->size||die("slice block sizes mismatch"));
                slice *from = curr->from;                assert(from||die("unable to find slice"));
                from->__release(curr);
                p = 0;
                n = 0;
            }
            else
            {
                assert(n<=0);
            }
        }

        void slice:: __release(block *curr) throw()
        {
            assert(curr);
            assert(this==curr->from);
            
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
            size_t nfree = 0;
            size_t nused = 0;
            for(const block *b = entry; b!=guard; b=b->next)
            {
                const char *status = (b->from) ? "USED" : "FREE";
                if(b->from)
                {
                    nused += b->size;
                }
                else
                {
                    nfree += b->size;
                }
                std::cerr << "\t" << status << " +" << b->size << std::endl;
            }
            std::cerr << "#used=" << nused << std::endl;
            std::cerr << "#free=" << nfree << std::endl;
            std::cerr << "<slice/>" << std::endl;
        }

    }
}


