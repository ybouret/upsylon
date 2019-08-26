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
        //! base class for looping over multiple indices
        template <typename T>
        class multi_loop
        {
        public:
            Y_DECL_ARGS(T,type); //!< aliases

            const size_t  dimensions; //!< how many dimensions
            const_type   *lower;      //!< lower bounds[0..dimensions-1]
            const_type   *upper;      //!< upper bounds[0..dimensions-1]
            const size_t  index;      //!< current index/count
            const size_t  count;      //!< product of all width
            const size_t  bytes;      //!< indices workspace bytes

            //! setup from user data, must be persistent
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
                start();
            }

            //! destructor
            inline virtual ~multi_loop() throw()
            {
                (size_t &)index = 0;
                memset(indices,0,bytes);
            }

            //! initialize indices
            inline void start() throw()
            {
                (size_t &)index = 1;
                memcpy(indices,lower,bytes);
            }

            //! compute next set of indices
            inline void  next() throw()
            {
                assert(index<=count);
                recursive_update(0);
                ++( (size_t&)index );
            }

            //! return current indices
            const_type *current() const throw()
            {
                return indices;
            }

            //! check if the loop is active
            inline bool active() const throw()
            {
                return (index<=count);
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

        //! data for embedded loop
        template <typename COORD>
        class multi_loop_for
        {
        public:
            //! destructor
            inline virtual ~multi_loop_for() throw()
            {
                clear();
            }

            //! setup
            inline explicit multi_loop_for() throw() :
            wksp(),
            value( * memory::io::__force<COORD>( wksp ) )
            {
                clear();
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(multi_loop_for);
            uint64_t wksp[ Y_U64_FOR_ITEM(COORD) ];
            inline void     clear() throw() { memset(wksp,0,sizeof(wksp)); }
        public:
            COORD   &value; //!< current indices
        };

    }


    //! embedding loop for multidimensial indices
    template <
    typename T,
    typename COORD>
    class multi_loop :
    public core::multi_loop_for<COORD>,
    public core::multi_loop<T>
    {
    public:
        Y_DECL_ARGS(T,type); //!< aliases
        
        //! destructor
        inline virtual ~multi_loop() throw()
        {
        }

        //! user data must be persistent
        inline explicit multi_loop(const COORD &lo,
                                   const COORD &up) throw() :
        core::multi_loop_for<COORD>(),
        core::multi_loop<T>(sizeof(COORD)/sizeof(T),
                            (const_type  *)   &lo,
                            (const_type  *)   &up,
                            (mutable_type*)   &(this->value) )
        {
        }



    private:
        Y_DISABLE_COPY_AND_ASSIGN(multi_loop);
    };

}

#endif


