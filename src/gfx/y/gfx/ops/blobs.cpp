#include "y/gfx/ops/blobs.hpp"
#include "y/sort/merge-list.hpp"

namespace upsylon
{
    
    namespace graphic
    {
        blobs:: ~blobs() throw()
        {
        }
        
        
        blobs:: blobs() throw()
        {
        }
        
        
        
        knot * blobs:: fetch_knot(shared_knots &cache)
        {
            return cache->size ? cache->pop_front() : knot::create_alive();
        }
        
        void blobs:: relabel() throw()
        {
            size_t label = 0;
            for(blob *b=head;b;b=b->next)
            {
                aliasing::_(b->label) = ++label;
            }
        }

        
        void blobs:: sort(pixmap<size_t> &masks)
        {
            merge_list_of<blob>::sort(*this, blob::decreasing_size,NULL);
            relabel();
            rewrite(masks);
        }

        void blobs:: rewrite(pixmap<size_t> &masks) const throw()
        {
            for(const blob *b=head;b;b=b->next)
            {
                b->dispatch(b->label,masks);
            }
        }


        void  blobs:: remove_if( blob::proc proc, void *args, pixmap<size_t> &masks) throw()
        {
            assert(proc);
            {
                blobs_ tmp;
                while(size)
                {
                    blob *b = pop_back();
                    if(proc(*b,args))
                    {
                        b->remove_from(masks);
                        delete b;
                    }
                    else
                    {
                        tmp.push_front(b);
                    }
                }
                swap_with(tmp);
            }
            relabel();
            rewrite(masks);
        }

        static inline bool smaller_than(const blob &b, void *args) throw()
        {
            assert(args);
            return b.size <= *(size_t *)args;
        }

        void  blobs:: remove_below(const size_t cutSize, pixmap<size_t> &masks) throw()
        {
            remove_if(smaller_than, (void*)&cutSize, masks);
        }
        

    }
    
}

