
#include "y/memory/section.hpp"
#include <iostream>

namespace upsylon {

    namespace memory {

        section:: section(void        *data,
                          const size_t size) throw():
        entry( static_cast<block *>(data) ),
        guard( entry ),
        large( entry )
        {
            assert(data!=NULL);
            assert(size>=small_size);
            size_t blocks = size/block_size; assert(blocks>=min_blocks);
            std::cerr << "user size: " << size << " => #blocks=" << blocks << " : max=" << (blocks-2)*block_size << std::endl;
            
            guard += --blocks;

            entry->prev = 0;
            entry->next = guard;
            entry->from = 0;
            entry->size = --blocks * block_size;

            guard->prev = entry;
            guard->next = NULL;
            guard->size = 0;
            guard->from = this;


        }

        section:: ~section() throw()
        {
            entry = guard = 0;
        }


    }

}

#include <iostream>

namespace upsylon {

    namespace memory {



        void section:: display() const
        {
            const block *blk = entry;
            while(blk)
            {
                if(blk->prev==NULL) std::cerr << '|';
                if(blk->next==NULL)
                {
                    std::cerr << '|';
                }
                else
                {
                    if(blk->from)
                    {
                        std::cerr << "<" << blk->size << ">";
                    }
                    else
                    {
                        std::cerr << "[" << blk->size << "]";
                    }
                }
                blk = blk->next;
            }
            std::cerr << std::endl;
        }
    }

}

