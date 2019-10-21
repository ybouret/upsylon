//! \file

#ifndef Y_MATH_ADJUST_SEQUENTIAL_INCLUDED
#define Y_MATH_ADJUST_SEQUENTIAL_INCLUDED 1

#include "y/math/adjust/types.hpp"
#include "y/math/adjust/variables.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {


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
                typedef typename Type<T>::Array Array;

                const T current; //!< currently called position

                inline virtual ~Sequential() throw() { aliasing::_(current)=0; } //!< cleanup

                //! make a first call
                inline T initialize(const T x, const Array &aorg, const Variables &vars)
                {
                    const T  ans = on_initialize(x,aorg,vars);
                    (T &)current = x;
                    return ans;
                }

                //! update value from previous call
                inline T compute_to(const T x, const Array &aorg, const Variables &vars)
                {
                    const T  ans = on_compute_to(x,aorg,vars);
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
                virtual T on_initialize(const T, const Array &,const Variables &) = 0;
                virtual T on_compute_to(const T, const Array &,const Variables &) = 0;
            };


        }

    }

}

#endif

