
#include "y/memory/slice.hpp"
#include <iostream>

namespace upsylon
{
    namespace memory
    {
#define Y_SLICE_SET_SIZE(S) (S)->size = ( static_cast<size_t>( (S)->next - ( (S)+1 ) ) * block_size )

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
            Y_SLICE_SET_SIZE(entry);
            assert(__check());
        }

        slice::~slice() throw()
        {
            if(entry->next!=guard && entry->from!=NULL)
            {
                std::cerr << "[memory.slice] not empty" << std::endl;
            }
        }

        bool slice:: __check() const
        {
            assert(entry);
            assert(guard);
            assert(entry->prev==NULL);
            assert(guard>entry+1);

            size_t fBlock = 0;
            size_t uBlock = 0;
            size_t iBlock = 0;
            for(const block *blk = entry; blk != guard; blk=blk->next )
            {
                ++iBlock;
                // check linking
                if(blk!=entry)
                {
                    if(blk->prev==NULL)
                    {
                        std::cerr << "invalid blk->prev for block #" << iBlock << std::endl;
                        return false;
                    }
                }
                if(blk->next==NULL)
                {
                    std::cerr << "invalid blk->next for block #" << iBlock << std::endl;
                    return false;
                }
                // check size
                const size_t length = static_cast<size_t>(blk->next-(blk+1)) * block_size;
                if(length!=blk->size)
                {
                    std::cerr << "invalid blk->size for block #" << iBlock << std::endl;
                }
                if(blk->from!=NULL)
                {
                    ++uBlock;
                }
                else
                {
                    ++fBlock;
                }
            }
            if(uBlock+fBlock!=iBlock)
            {
                std::cerr << "invalid checksum" << std::endl;
                return false;
            }
            if(fBlock!=count)
            {
                std::cerr << "invalid count of free blocks: " << fBlock << "/" << count <<  std::endl;
                return false;
            }

            return true;
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
            if(!count) return 0;
            for(block *curr=entry;curr!=guard;curr=curr->next)
            {
                if(!(curr->from) && curr->size>=n)
                {
                    //__________________________________________________________
                    //
                    // found an available block
                    //__________________________________________________________
                    const size_t available_bytes = curr->size;                      assert(available_bytes>0); assert(0==available_bytes%block_size);
                    const size_t preferred_bytes = Y_ALIGN_FOR_ITEM(block,n);       assert(preferred_bytes<=available_bytes);
                    const size_t remaining_bytes = available_bytes-preferred_bytes; assert(0==remaining_bytes%block_size);
#if 0
                    std::cerr << "required: " << n << std::endl;
                    std::cerr << "\tavailable_bytes : " << available_bytes << std::endl;
                    std::cerr << "\tpreferred_bytes : " << preferred_bytes << std::endl;
                    std::cerr << "\tremaining_bytes : " << remaining_bytes << std::endl;
#endif
                    if(remaining_bytes>=small_size)
                    {
                        //______________________________________________________
                        //
                        // insert new block
                        //______________________________________________________
                        block *temp  = &curr[1+preferred_bytes/block_size];
                        block *after = curr->next;

                        temp->next = after; assert(temp->next>temp+1);
                        if(after!=guard)
                        {
                            after->prev = temp;
                        }
                        temp->prev = curr;
                        Y_SLICE_SET_SIZE(temp);
                        temp->from = NULL;

                        //______________________________________________________
                        //
                        // update current
                        //______________________________________________________
                        curr->next = temp;
                        Y_SLICE_SET_SIZE(curr); assert(preferred_bytes==curr->size);
                        ++count;
                        assert(__check());
                    }

                    //__________________________________________________________
                    //
                    // prepare allocated
                    //__________________________________________________________
                    n          = curr->size;
                    curr->from = this;
                    void *addr = &curr[1];
                    memset(addr,0,n);
                    --count;
                    assert(__check());
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
            assert(p);
            assert(n>0);
            block *curr = static_cast<block *>(p)-1; assert(n==curr->size||die("slice block sizes mismatch"));
            slice *from = curr->from;                assert(from||die("unable to find slice"));
            from->__release(curr);
            p = 0;
            n = 0;
        }


        void slice:: __release(block *curr) throw()
        {
            assert(curr);
            assert(curr>=entry);
            assert(curr<guard);
            assert(this==curr->from);

            static const int reset_block_only = 0x00;
            static const int fusion_with_prev = 0x01;
            static const int fusion_with_next = 0x02;
            static const int fusion_with_both = fusion_with_prev | fusion_with_next;

            int    action = reset_block_only;
            block *prev   = curr->prev; if(prev!=NULL  && !(prev->from) ) action |= fusion_with_prev;
            block *next   = curr->next; if(guard!=next && !(next->from) ) action |= fusion_with_next;

            switch(action)
            {
                case fusion_with_prev: {
                    prev->next = next;
                    next->prev = prev;
                    Y_SLICE_SET_SIZE(prev);
                    // no new block
                    assert(__check()||die("fusion_with_prev"));
                } break;

                case fusion_with_next: {
                    block *after = next->next;
                    curr->next = after;
                    if(after!=guard) {
                        after->prev = curr;
                    }
                    Y_SLICE_SET_SIZE(curr);
                    // no new block
                    curr->from=0;
                    assert(__check()||die("fusion_with_next"));
                } break;

                case fusion_with_both: {
                    block *after = next->next;
                    prev->next = after;
                    if(after!=guard) {
                        after->prev = prev;
                    }
                    Y_SLICE_SET_SIZE(prev);
                    assert(count>=2);
                    --count;
                    assert(__check()||die("fusion_with_both"));
                } break;

                default:
                    assert(reset_block_only==action);
                    curr->from = 0;
                    ++count; //!< another available block!
                    assert(__check()||die("reset_block_only"));

            }

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


