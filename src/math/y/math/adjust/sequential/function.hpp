//! \file

#ifndef Y_MATH_ADJUST_SEQUENTIAL_FUNCTION_INCLUDED
#define Y_MATH_ADJUST_SEQUENTIAL_FUNCTION_INCLUDED 1

#include "y/math/adjust/sequential.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            //! lightweight proxy for regular function
            template <typename T>
            class SequentialFunction : public Sequential<T>
            {
            public:
                typedef typename Type<T>::Function   Function;   //!< alias

                inline explicit SequentialFunction( Function &F ) throw() : host(F) {} //!< setup
                inline virtual ~SequentialFunction() throw() {}                        //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SequentialFunction);
                Function &host; //!< reference to external function
                inline virtual T onStart(const T x, const accessible<T> &aorg, const Variables &vars) { return host(x,aorg,vars); }
                inline virtual T onReach(const T x, const accessible<T> &aorg, const Variables &vars) { return host(x,aorg,vars); }
            };
            
        }
    }
}

#endif

