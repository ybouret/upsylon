//! \file

#ifndef Y_MKL_FIT_SEQUENTIAL_INCLUDED
#define Y_MKL_FIT_SEQUENTIAL_INCLUDED 1

#include "y/mkl/fit/types.hpp"
#include "y/mkl/fit/variables.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace mkl {

        namespace Fit {

            //======================================================================
            //
            //
            //! interface for sequential calls
            //
            //
            //======================================================================
            template <typename T> class Sequential
            {
            public:

                const T current; //!< currently called position

                inline virtual ~Sequential() throw() { aliasing::_(current)=0; } //!< cleanup

                //! make a first call
                inline T start(const T x, const accessible<T> &aorg, const Variables &vars)
                {
                    const  T  ans = onStart(x,aorg,vars);
                    aliasing::_(current) = x;
                    return ans;
                }

                //! update value from previous call
                inline T reach(const T x, const accessible<T> &aorg, const Variables &vars)
                {
                    const T  ans = onReach(x,aorg,vars);
                    aliasing::_(current) = x;
                    return ans;
                }


            protected:
                //! setup
                inline explicit Sequential() throw() : current(0)
                {
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Sequential);
                virtual T onStart(const T, const accessible<T> &,const Variables &) = 0;
                virtual T onReach(const T, const accessible<T> &,const Variables &) = 0;
            };


        }

    }

}

#endif

