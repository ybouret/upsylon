#include "y/memory/section.hpp"
#include "y/code/round.hpp"
#include <iostream>
#include <cstring>

namespace upsylon {

    namespace memory {
        
        section:: section(void        *data,
                          const size_t size) throw():
        entry( static_cast<block *>(data) ),
        guard( entry )
        {
            assert(data!=NULL);
            assert(size>=small_size);
            size_t blocks = size/block_size; assert(blocks>=min_blocks);
            std::cerr << "Keeping " << blocks * block_size << " out of " << size << std::endl;

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
            entry = guard = 0;
        }





        void * section:: acquire(size_t &n) throw()
        {
            if(n<=0)
            {
                return 0;
            }
            else
            {
                const size_t boundary = Y_ROUND_LN2(block_iln2,n);
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
                        //std::cerr << "found " << available << "/" << required << std::endl;

                        if(available>=required+2)
                        {
                            //std::cerr << "should split!" << std::endl;

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
                            n = currBlock->size * block_size;
                        }

                        assert(currBlock->size * block_size == n );
                        currBlock->from = this;
                        void *p = &currBlock[1];
                        memset(p,0,n);
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
        }

        section *section:: release(void *addr, size_t &n) throw()
        {
            assert(addr);
            assert(n>0);
            block   *currBlock = static_cast<block *>(addr) - 1; assert(currBlock->from); assert(currBlock->size*block_size==n);
            section *owner     = currBlock->from;                assert(owner->check_block(currBlock));

            return owner;
         }



    }

}

#include <iostream>

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

