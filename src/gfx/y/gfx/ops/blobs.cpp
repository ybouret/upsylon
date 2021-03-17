
#include "y/gfx/ops/blobs.hpp"

namespace upsylon
{

    namespace graphic
    {

        knots:: knots() throw()
        {
        }

        knots:: ~knots() throw()
        {
        }


    }

}

namespace upsylon
{

    namespace graphic
    {

        blob:: blob(const size_t        id,
                    const shared_knots &ks) throw() :
        label(id),
        cache(),
        kpool(ks)
        {
        }

        blob:: ~blob() throw()
        {
            kpool->merge_back(cache);
        }


    }

}


namespace upsylon
{

    namespace graphic
    {
        blobs:: ~blobs() throw()
        {
        }


        blobs:: blobs(const unit_t W, const unit_t H, const shared_knots &K) :
        pixmap<size_t>(W,H),
        kpool(K)
        {
        }

        void blobs:: initialize(size_t n)
        {
            release();
            ldz();
            while(cache.size>n)
            {
                kpool->push_back( cache.pop_front() );
            }

            while(cache.size<n&&kpool->size)
            {
                cache.push_back(kpool->pop_front());
            }

            while(cache.size<n)
            {
                cache.push_back( knot::create_alive() );
            }

        }

    }

}

