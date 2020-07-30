#include "y/memory/section.hpp"
#include "y/code/round.hpp"
#include "y/type/utils.hpp"

#include <iostream>
#include <cstring>

namespace upsylon {

    namespace memory {


        size_t   section:: bytes_to_hold(const size_t bytes) throw()
        {
            return max_of<size_t>(small_size,Y_ROUND_LN2(block_iln2,bytes)+2*block_size);
        }


        section:: section(void        *data,
                          const size_t size) throw():
        entry( static_cast<block *>(data) ),
        guard( entry ),
        next(0),
        prev(0)
        {
            assert(data!=NULL);
            assert(size>=small_size);
            size_t blocks = size/block_size; assert(blocks>=min_blocks);

            guard += --blocks;

            entry->prev = 0;
            entry->next = guard;
            entry->from = 0;
            entry->size = --blocks;


            guard->prev = entry;
            guard->next = NULL;
            guard->size = 0;
            guard->from = this;

            assert(check_block(entry));
            assert(check_block(guard));


        }

        section:: ~section() throw()
        {
            if(!is_free())
            {

            }
            entry = guard = 0;
        }

        bool section:: is_free() const throw()
        {
            return (NULL==entry->from) && (guard==entry->next);
        }




        void * section:: try_acquire(size_t &n, const bool zero) throw()
        {

                const size_t boundary = (n<=0) ? block_size : Y_ROUND_LN2(block_iln2,n);
                const size_t required = boundary >> block_iln2;

                //--------------------------------------------------------------
                //
                // loop over free blocks
                //
                //--------------------------------------------------------------
                const block  *lastBlock = guard;
                block        *currBlock = entry;
                while(currBlock!=lastBlock)
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
                    const size_t available = currBlock->size;
                    if(available>=required)
                    {
                        //------------------------------------------------------
                        //
                        // found
                        //
                        //------------------------------------------------------
                        
                        if(available>=required+2)
                        {
                            // create a new block
                            block *nextBlock = currBlock->next;
                            block *new_block = currBlock+required+1;
                            new_block->prev  = currBlock;
                            new_block->next  = nextBlock;
                            new_block->from  = 0;
                            new_block->size  = available-required-1;

                            // update nextblock
                            nextBlock->prev = new_block;

                            // update currBlock
                            currBlock->size = required;
                            currBlock->next = new_block;

                            assert(check_block(currBlock));
                            assert(check_block(nextBlock));
                            assert(check_block(new_block));
                            n = boundary;
                        }
                        else
                        {
                            // full block
                            n = currBlock->size * block_size;
                        }

                        assert(currBlock->size * block_size == n );
                        currBlock->from = this;
                        void *p = &currBlock[1];
                        if(zero) memset(p,0,n);
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

        void * section::acquire(size_t &n) throw()
        {
            return try_acquire(n,true);
        }

        section * section:: receive(void * &addr, size_t &maxi, const size_t size) throw()
        {
            assert(addr);
            assert(maxi);
            assert(maxi>=size);
            size_t n = size;
            void  *p = try_acquire(n,false);
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

            block   *currBlock = static_cast<block *>(addr) - 1; assert(currBlock->from); assert(currBlock->size*block_size==n);
            section *owner     = currBlock->from;                assert(owner->check_block(currBlock)); assert(owner->guard!=currBlock);

            static const unsigned merge_none = 0x00;
            static const unsigned merge_prev = 0x01;
            static const unsigned merge_next = 0x02;
            static const unsigned merge_both = merge_prev|merge_next;

            unsigned flag      = merge_none;
            block   *prevBlock = currBlock->prev; if(prevBlock&&(0==prevBlock->from))       flag |= merge_prev;
            block   *nextBlock = currBlock->next; assert(nextBlock); if(0==nextBlock->from) flag |= merge_next;

            switch(flag)
            {
                case merge_prev:
                    prevBlock->next = nextBlock;
                    nextBlock->prev = prevBlock;
                    prevBlock->size = static_cast<len_t>(nextBlock-prevBlock)-1;
                    assert(owner->check_block(prevBlock));
                    assert(owner->check_block(nextBlock));
                    break;

                case merge_next:
                    assert(nextBlock->next!=NULL);
                    nextBlock=nextBlock->next;
                    currBlock->next = nextBlock;
                    nextBlock->prev = currBlock;
                    currBlock->size = static_cast<len_t>(nextBlock-currBlock)-1;
                    currBlock->from = 0;
                    assert(owner->check_block(currBlock));
                    assert(owner->check_block(nextBlock));
                    break;

                case merge_both:
                    assert(nextBlock->next!=NULL);
                    nextBlock=nextBlock->next;
                    prevBlock->next = nextBlock;
                    nextBlock->prev = prevBlock;
                    prevBlock->size = static_cast<len_t>(nextBlock-prevBlock)-1;
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


namespace upsylon {

    namespace memory {

        void section:: display() const
        {
            assert(entry);
            assert(guard);
            const block *blk = entry;
            std::cerr << '|';
            while(blk)
            {
                std::cerr << "(" << block_size << ")";
                if(blk->next==NULL)
                {
                    std::cerr << '|';
                }
                else
                {
                    if(blk->from)
                    {
                        std::cerr << "<" << blk->size * block_size << ">";
                    }
                    else
                    {
                        std::cerr << "[" << blk->size * block_size << "]";
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
                Y_MEM_SEC_BLK(blk->size==static_cast<len_t>(nxt-blk)-1);
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
