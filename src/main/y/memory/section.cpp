#include "y/memory/section.hpp"
#include "y/code/round.hpp"
#include <iostream>

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

            guard += --blocks;

            entry->prev = 0;
            entry->next = guard;
            entry->from = 0;
            entry->size = --blocks;

            guard->prev = entry;
            guard->next = NULL;
            guard->size = 0;
            guard->from = this;


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

                std::cerr << "n=" << n << " : boundary=" << boundary << " : required=" << required << std::endl;

                // loop over free blocks
                block *currBlock = entry;
                block *nextBlock = entry->next;
                while(nextBlock&&!(currBlock->from))
                {
                    const size_t available = currBlock->size;
                    if(available>=required)
                    {
                        // ok
                        std::cerr << "found " << available << "/" << required << std::endl;

                        if(available>=required+block_left)
                        {
                            std::cerr << "should split!" << std::endl;
                            return 0;
                        }
                        else
                        {
                            currBlock->from = this;
                            n = available * block_size;
                            void *p = &currBlock[1];
                            memset(p,0,n);
                            return p;
                        }
                    }
                    currBlock=nextBlock;
                    nextBlock=nextBlock->next;
                }
                return 0;

            }
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
    }

}

