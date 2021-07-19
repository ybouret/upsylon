#include "y/gfx/ops/blob.hpp"

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

        void knots:: reserve(size_t n)
        {
            while(n-- > 0)
                push_back( knot::create_alive() );
        }

        void knots:: ensure(const size_t n)
        {
            if(n>size) reserve(n-size);
        }

    }
}

#include "y/comparison.hpp"
namespace upsylon
{

    namespace graphic
    {

        blob:: blob(const size_t        id,
                    const shared_knots &ks) throw() :
        label(id),
        cache(ks)
        {
        }

        blob:: ~blob() throw()
        {
            cache->merge_back(*this);
        }

        blob * blob:: change_label(const size_t    value,
                                   blob_map       &bmask) throw()
        {
            aliasing::_(label) = value;
            dispatch(value,bmask);
            return this;
        }

        blob  * blob:: remove_from(blob_map &bmask) throw()
        {
            return change_label(0,bmask);
        }

        int blob:: increasing_size(const blob *lhs, const blob *rhs, void*) throw()
        {
            return comparison::increasing(lhs->size,rhs->size);
        }


        int blob:: decreasing_size(const blob *lhs, const blob *rhs,void*) throw()
        {
            return comparison::decreasing(lhs->size,rhs->size);
        }

        bool blob:: is_smaller_than(const blob &b, void *args) throw()
        {
            assert(NULL!=args);
            return b.size <= *(const size_t *)args;
        }

    }

}
