

#ifndef Y_MATH_ADJUST_SEQUENTIAL_GRADIENT_INCLUDED
#define Y_MATH_ADJUST_SEQUENTIAL_GRADIENT_INCLUDED 1

#include "y/math/adjust/sequential.hpp"
#include "y/math/fcn/derivatives.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            template <typename T>
            class Gradient : public derivative<T>
            {
            public:
                explicit Gradient() : derivative<T>()
                {
                }

                virtual ~Gradient() throw()
                {
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Gradient);
            };

        }

    }

}

#endif

