#include "y/gfx/ops/blobs.hpp"
#include "y/sort/merge-list.hpp"

namespace upsylon
{
    
    namespace graphic
    {

        blobs:: ~blobs() throw()
        {
        }
        
        
        blobs:: blobs(const unit_t W, const unit_t H, const shared_knots &K):
        blist(),
        bmask(W,H),
        cache(K)
        {

        }

        knot * blobs:: fetch_knot()
        {
            return cache->size ? cache->pop_front() : knot::create_alive();
        }

        void  blobs:: rewrite() throw()
        {
            for(const blob *b=blist.head;b;b=b->next)
            {
                b->dispatch(b->label,aliasing::_(bmask));
            }
        }

        void blobs:: relabel() throw()
        {
            size_t label = 0;
            for(blob *b=aliasing::_(blist).head;b;b=b->next)
            {
                aliasing::_(b->label) = ++label;
            }
            rewrite();
        }

        void blobs:: clear() throw()
        {
            aliasing::_(blist).release();
            aliasing::_(bmask).ldz();
        }

        void  blobs:: expunge(blob *b) throw()
        {
            assert(b!=NULL);
            delete aliasing::_(blist).unlink( b->remove_from( aliasing::_(bmask) ) );
        }

        blob * blobs:: produce(size_t &label)
        {
            return aliasing::_(blist).push_back( new blob(++label,cache) );
        }

        void blobs::sort()
        {
            merge_list_of<blob>::sort(aliasing::_(blist), blob::decreasing_size,NULL);
            relabel();
        }



        void  blobs:: remove_if( blob::proc proc, void *args) throw()
        {
            assert(NULL!=proc);
            {
                blob_list tmp;
                while(blist.size)
                {
                    blob *b = aliasing::_(blist).pop_back();
                    if(proc(*b,args))
                    {
                        b->remove_from( aliasing::_(bmask) );
                        delete b;
                    }
                    else
                    {
                        tmp.push_front(b);
                    }
                }
                aliasing::_(blist).swap_with(tmp);
            }
            relabel();
        }

        void blobs::remove_below(const size_t cutSize) throw()
        {
            remove_if(blob::is_smaller_than,aliasing::any(cutSize));
        }
        
        
    }
    
}

