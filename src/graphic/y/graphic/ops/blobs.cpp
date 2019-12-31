
#include "y/graphic/ops/blobs.hpp"
#include "y/sort/merge.hpp"
#include "y/comparison.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace Graphic {

        Blob:: Blob( const size_t blobLabel ) throw() : label(blobLabel) {}
        Blob:: ~Blob() throw()
        {
        }
        

      

        Blobs:: Blobs( const size_t W, const size_t  H) :
        Pixmap<size_t>(W,H),
        Blob::List()
        {
        }

        Blobs:: ~Blobs() throw()
        {
            
        }

        static inline int compareBlobs( const Blob *lhs, const Blob *rhs, void *) throw()
        {
            return comparison::decreasing(lhs->size,rhs->size );
        }

        void Blobs:: rewrite()
        {
            Blob::List &blobs = *this;
            merging<Blob>::sort(blobs, compareBlobs, NULL);

            Blob *blob = blobs.head;
            for(size_t i=1;i<=blobs.size;++i)
            {
                aliasing::_(blob->label) = i;
            }
        }

        void Blobs:: keepAbove( const size_t minSize ) throw()
        {
            Pixmap<size_t> &field = *this;
            Blob::List     &blobs = *this;
            while(blobs.tail && blobs.tail->size<=minSize )
            {
                for( const PNode *node = blobs.tail->head; node; node=node->next )
                {
                    field[ **node ] = 0;
                }
                delete blobs.pop_back();
            }
        }

    }

}
