//! \file
#ifndef Y_MULTI_LOOP_INCLUDED
#define Y_MULTI_LOOP_INCLUDED 1

#include "y/type/args.hpp"
#include "y/code/round.hpp"
#include "y/memory/io.hpp"

#include <cstring>

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
            const size_t  bytes;

            inline explicit multi_loop(const size_t  nd,
                                       const_type   *lo,
                                       const_type   *up,
                                       mutable_type *in) throw() :
            dimensions(nd),
            lower(lo),
            upper(up),
            index(1),
            count(extent()),
            bytes( sizeof(T) * dimensions ),
            indices(in)
            {
                assert(dimensions>0);
                assert(NULL!=upper);
                assert(NULL!=lower);
                assert(NULL!=indices);
                initialize();
            }

            inline virtual ~multi_loop() throw()
            {
                (size_t &)index = 0;
                memset(indices,0,bytes);
            }

            inline const_type *initialize() throw()
            {
                (size_t &)index = 1;
                memcpy(indices,lower,bytes);
                return indices;
            }

            const_type *compute_next() throw()
            {
                assert(index<=count);
                recursive_update(0);

                ++( (size_t&)index );
                return indices;
            }

            const_type *current() const throw()
            {
                return indices;
            }

            inline bool active() const throw()
            {
                return index<=count;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(multi_loop);

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

        };

        template <typename COORD>
        class multi_loop_for
        {
        public:
            inline virtual ~multi_loop_for() throw() {}

            inline explicit multi_loop_for() throw() :
            wksp(),
            value( * memory::io::__force<COORD>( wksp ) )
            {
                memset(wksp,0,sizeof(wksp));
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(multi_loop_for);
            uint64_t wksp[ Y_U64_FOR_ITEM(COORD) ];
        public:
            COORD   &value;
        };

    }

    template <
    typename T,
    typename COORD>
    class multi_loop :
    public core::multi_loop_for<COORD>,
    public core::multi_loop<T>
    {
    public:
        Y_DECL_ARGS(T,type);

        inline virtual ~multi_loop() throw()
        {
        }

        inline explicit multi_loop(const COORD &lo,
                                   const COORD &up) throw() :
        core::multi_loop_for<COORD>(),
        core::multi_loop<T>(sizeof(COORD)/sizeof(T),
                            (const_type  *)   &lo,
                            (const_type  *)   &up,
                            (mutable_type*)   &(this->value) )
        {
        }

        inline void start() throw()
        {
            (void) this->initialize();
        }

        inline void next() throw()
        {
            (void) this->compute_next();
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(multi_loop);
    };

}

#endif


