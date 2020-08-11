#include "y/memory/joint/section.hpp"
#include "y/code/round.hpp"
#include "y/type/utils.hpp"
#include "y/type/aliasing.hpp"
#include "y/code/base2.hpp"
#include "y/exceptions.hpp"
#include <iostream>
#include <cstring>

namespace upsylon {

    namespace memory {

        namespace joint {

            size_t  section::block:: round(const size_t bytes) throw()
            {
                if(bytes<=size)
                {
                    return size;
                }
                else
                {
                    assert(bytes>size);
                    const size_t ans = Y_ROUND_LN2(exp2,bytes);
                    assert(0==(ans%size));
                    return ans;
                }
            }

            size_t section:: holding(const size_t bytes, size_t &shift)
            {

                const size_t aligned_bytes = block::round(bytes);
                if(aligned_bytes>max_allocated) throw exception("joint::section:: too many bytes to hold");

                const size_t ans = next_power_of_two(max_of(aligned_bytes+2*block::size,min_size));
                shift            = integer_log2(ans); /* and check... */ assert(size_t(1)<<shift==ans);  assert(ans>=bytes);
                return ans;

             }


            section:: section(void        *usr_data,
                              const size_t usr_size,
                              const size_t usr_exp2) throw():
            entry( static_cast<block *>(usr_data) ),
            guard( entry ),
            next(0),
            prev(0),
            size(usr_size),
            exp2(usr_exp2),
            greatest(entry),
            capacity(0)
            {
                assert(usr_data!=NULL);
                assert(size>=min_size);
                assert(is_a_power_of_two(size));
                assert(size_t(1)<<exp2==size);

                size_t blocks = size >> block::exp2;
                assert(blocks>=min_blocks);
                guard += --blocks;

                entry->prev = 0;
                entry->next = guard;
                entry->from = 0;
                entry->bulk = --blocks;


                guard->prev = entry;
                guard->next = NULL;
                guard->bulk = 0;
                guard->from = this;

                assert(check_block(entry));
                assert(check_block(guard));
                
                capacity = greatest->bulk << block::exp2;

            }
            

            section:: ~section() throw()
            {
                if(!is_empty())
                {
                    std::cerr << "[memory.section] not empty: ";
                    for(const block *blk=entry;blk;blk=blk->next)
                    {
                        if(blk->from)
                        {
                            std::cerr << blk->bulk << "/";
                        }
                    }
                    std::cerr << std::endl;
                }
                entry = guard = 0;
            }

            bool section:: is_empty() const throw()
            {
                return (NULL==entry->from) && (guard==entry->next);
            }

            static inline void __zero(void *p, const size_t n) throw()
            {
                assert(n>0);
                assert(p!=NULL);
                memset(p,0,n);
            }


            void *section:: acquire(size_t &n) throw()
            {
                return acquire(n,__zero);
            }


            void * section:: acquire(size_t &n, finalize proc) throw()
            {

                static const size_t split_blocks = 3;
                static const size_t extra_blocks = split_blocks-1;
                static const size_t delta_blocks = extra_blocks-1;

                assert(proc);
                const size_t boundary = block::round(n);
                const size_t required = boundary >> block::exp2;

                //--------------------------------------------------------------
                //
                // loop over free blocks
                //
                //--------------------------------------------------------------
                block   *currBlock = entry;
                while(0!=currBlock)
                {
                    //----------------------------------------------------------
                    //
                    // check available
                    //
                    //----------------------------------------------------------
                    if(currBlock->from)
                    {
                        currBlock=currBlock->next;
                        continue;
                    }

                    //----------------------------------------------------------
                    //
                    // check big enough
                    //
                    //----------------------------------------------------------
                    const size_t available = currBlock->bulk;
                    if(available>=required)
                    {
                        //------------------------------------------------------
                        //
                        // found
                        //
                        //------------------------------------------------------
                        if(available>=required+extra_blocks)
                        {
                            // create a new block
                            block *nextBlock = currBlock->next;
                            block *new_block = currBlock+required+1;
                            new_block->prev  = currBlock;
                            new_block->next  = nextBlock;
                            new_block->from  = 0;
                            new_block->bulk  = available-required-delta_blocks;

                            // update nextBlock
                            nextBlock->prev = new_block;

                            // update currBlock
                            currBlock->bulk = required;
                            currBlock->next = new_block;

                            assert(check_block(currBlock));
                            assert(check_block(nextBlock));
                            assert(check_block(new_block));
                            n = boundary;
                        }
                        else
                        {
                            // full block
                            n = currBlock->bulk * block::size;
                        }

                        assert(currBlock->bulk * block::size == n );
                        currBlock->from = this;
                        void *p = &currBlock[1];
                        proc(p,n);
                        return p;
                    }

                    //----------------------------------------------------------
                    //
                    // try next block
                    //
                    //----------------------------------------------------------
                    currBlock=currBlock->next;
                }

                //--------------------------------------------------------------
                //
                // no possible allocation
                //
                //--------------------------------------------------------------
                return 0;
            }


            static inline void __nope(void*,const size_t) throw()
            {

            }

            section * section:: receive(void * &addr, size_t &maxi, const size_t size) throw()
            {
                assert(addr);
                assert(maxi);
                assert(maxi>=size);
                size_t n = size;
                void  *p = acquire(n,__nope);
                if(p)
                {
                    assert(n>=size);
                    memcpy(p,addr,size);
                    memset(static_cast<char*>(p)+size,0,n-size);
                    section *s = release(addr,maxi);
                    addr = p;
                    maxi = n;
                    return s;
                }
                else
                {
                    // left untouched
                    return 0;
                }
            }



            section *section:: release(void * &addr, size_t &n) throw()
            {
                assert(addr); assert(n>0);

                //------------------------------------------------------------------
                // get block an owner
                //------------------------------------------------------------------
                block   *currBlock = static_cast<block *>(addr) - 1; assert(currBlock->from); assert(currBlock->bulk*block::size==n);
                section *owner     = currBlock->from;                assert(owner->check_block(currBlock)); assert(owner->guard!=currBlock);

                //------------------------------------------------------------------
                // check status
                //------------------------------------------------------------------
                static const unsigned merge_none = 0x00;
                static const unsigned merge_prev = 0x01;
                static const unsigned merge_next = 0x02;
                static const unsigned merge_both = merge_prev|merge_next;

                unsigned flag      = merge_none;
                block   *prevBlock = currBlock->prev; if(prevBlock&&(0==prevBlock->from))       flag |= merge_prev;
                block   *nextBlock = currBlock->next; assert(nextBlock); if(0==nextBlock->from) flag |= merge_next;

                //------------------------------------------------------------------
                // merge
                //------------------------------------------------------------------
                switch(flag)
                {
                    case merge_prev:
                        prevBlock->next = nextBlock;
                        nextBlock->prev = prevBlock;
                        prevBlock->bulk = static_cast<len_t>(nextBlock-prevBlock)-1;
                        assert(owner->check_block(prevBlock));
                        assert(owner->check_block(nextBlock));
                        break;

                    case merge_next:
                        assert(nextBlock->next!=NULL);
                        nextBlock=nextBlock->next;
                        currBlock->next = nextBlock;
                        nextBlock->prev = currBlock;
                        currBlock->bulk = static_cast<len_t>(nextBlock-currBlock)-1;
                        currBlock->from = 0;
                        assert(owner->check_block(currBlock));
                        assert(owner->check_block(nextBlock));
                        break;

                    case merge_both:
                        assert(nextBlock->next!=NULL);
                        nextBlock=nextBlock->next;
                        prevBlock->next = nextBlock;
                        nextBlock->prev = prevBlock;
                        prevBlock->bulk = static_cast<len_t>(nextBlock-prevBlock)-1;
                        assert(owner->check_block(prevBlock));
                        assert(owner->check_block(nextBlock));
                        break;

                    default:
                        assert(merge_none==flag);
                        currBlock->from = 0;
                        break;
                }

                addr = 0;
                n    = 0;
                return owner;
            }


        }
    }

}


namespace upsylon {

    namespace memory {

        namespace joint {

            void section:: display() const
            {
                assert(entry);
                assert(guard);
                const block *blk = entry;
                std::cerr << '|';
                while(blk)
                {
                    std::cerr << "(" << block::size << ")";
                    if(blk->next==NULL)
                    {
                        std::cerr << '|';
                    }
                    else
                    {
                        if(blk->from)
                        {
                            std::cerr << "<" << blk->bulk * block::size << ">";
                        }
                        else
                        {
                            std::cerr << "[" << blk->bulk * block::size << "]";
                        }
                    }
                    blk = blk->next;
                }
                std::cerr << std::endl;

            }

#define Y_MEM_SEC_BLK(EXPR) do { if(!(EXPR)) { std::cerr << pfx << #EXPR << sfx << std::endl; return false; } } while(false)

            bool section:: check_block(const block *blk) const
            {
                static const char pfx[] = "memory.section.check_block: ! '";
                static const char sfx[] = "'";
                assert(blk);
                if(blk->next)
                {
                    const block *nxt = blk->next;
                    Y_MEM_SEC_BLK(nxt->prev==blk);
                    Y_MEM_SEC_BLK(blk->bulk==static_cast<len_t>(nxt-blk)-1);
                }

                if(blk->from)
                {
                    Y_MEM_SEC_BLK(blk->from==this);
                }

                if(blk->prev)
                {
                    const block *prv = blk->prev;
                    Y_MEM_SEC_BLK(prv->next==blk);
                }

                return true;
            }

            bool section:: check() const
            {
                for(const block *blk=entry; blk; blk=blk->next)
                {
                    if(!check_block(blk)) return false;
                }
                return true;
            }
        }
    }

}

