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
                typedef typename Type<T>::Array      Array;      //!< alias
                typedef typename Type<T>::Parameters Parameters; //!< alias
                typedef typename Type<T>::Function   Function;   //!< alias

                inline explicit SequentialFunction( Function &F ) throw() : host(F) {} //!< setup
                inline virtual ~SequentialFunction() throw() {}                        //!< cleanup
                Function &host; //!< reference to external function

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SequentialFunction);
                inline virtual T on_initialize(const T x, const Parameters &aorg, const Variables &vars) { return host(x,aorg,vars); }
                inline virtual T on_compute_to(const T x, const Parameters &aorg, const Variables &vars) { return host(x,aorg,vars); }
            };
            
        }
    }
}

#endif

