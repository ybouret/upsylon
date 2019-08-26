//! \file
#ifndef Y_MULTI_LOOP_INCLUDED
#define Y_MULTI_LOOP_INCLUDED 1

#include "y/type/args.hpp"

namespace upsylon
{
    namespace core
    {
        template <typename T>
        class multi_loop
        {
        public:
            Y_DECL_ARGS(T,type);

            const size_t  dimensions; //!< how many dimensions
            const_type   *lower;      //!< lower bounds[0..dimensions-1]
            const_type   *upper;      //!< upper bounds[0..dimensions-1]
            const size_t  index;      //!< current index/count
            const size_t  count;      //!< product of all width

            inline explicit multi_loop(const size_t  nd,
                                       const_type   *lo,
                                       const_type   *up,
                                       mutable_type *in) throw() :
            dimensions(nd),
            lower(lo),
            upper(up),
            index(1),
            count(extent()),
            indices(in)
            {
                assert(dimensions>0);
                assert(NULL!=upper);
                assert(NULL!=lower);
                assert(NULL!=indices);
                start();
            }

            inline virtual ~multi_loop() throw()
            {

            }

            inline const_type *start() throw()
            {
                (size_t &)index = 1;
                for(size_t i=0;i<dimensions;++i)
                {
                    indices[i] = lower[i];
                }
                return indices;
            }

            const_type *next() throw()
            {
                assert(index<count);
                recursive_update(0);

                ++( (size_t&)index );
                return indices;
            }

            const_type *current() const throw()
            {
                return indices;
            }


        private:
            Y_DISABLE_ASSIGN(multi_loop);

            mutable_type *indices;    //!< indices[0..dimensions-1]

            inline size_t extent() const throw()
            {
                size_t ans = 1;
                for(size_t i=0;i<dimensions;++i)
                {
                    assert(lower[i]<=upper[i]);
                    const size_t ni = static_cast<size_t>(1+upper[i]-lower[i]);
                    ans *= ni;
                }
                return ans;
            }

            inline void recursive_update(size_t dim) throw()
            {
                mutable_type &i = indices[dim];
                if(++i>upper[dim])
                {
                    i = lower[dim];
                    if(++dim>=dimensions) dim=0;
                    recursive_update(dim);
                }
            }

        public:
        };
    }
}

#endif


