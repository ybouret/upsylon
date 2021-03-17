
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
            while(size)
            {
                knot *k = pop_back();
                knot::destruct(k);
            }
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
        kpool(ks)
        {
        }

        blob:: ~blob() throw()
        {
            kpool->merge_back(*this);
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


        blobs:: blobs(const shared_marks &M,
                      const shared_knots &K) :
        probe(M),
        kpool(K)
        {
        }

        void blobs:: initialize(size_t n)
        {
            release();
            probe->ldz();
            while(kpool->size<n) kpool->push_back( knot::create_alive() );
        }

        knot * blobs:: fetch_knot()
        {
            return kpool->size ? kpool->pop_front() : knot::create_alive();
        }

    }

}

