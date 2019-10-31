
//! \file

#ifndef Y_MATH_ADJUST_CONTEXT_INCLUDED
#define Y_MATH_ADJUST_CONTEXT_INCLUDED 1

#include "y/math/adjust/sample.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            enum ContextStatus
            {
                ContextSuccess,
                ContextChanged,
                ContextFailure
            };



            template <typename T>
            class Context
            {
            public:
                const SampleType<T>     &sample;
                const accessible<bool>  &used;
                addressable<T>          &value;

                inline   ~Context() throw() {}

                inline   Context(const SampleType<T>    &sample_,
                                 const accessible<bool> &used_,
                                 addressable<T>         &value_) throw() :
                sample( sample_ ),
                used(   used_   ),
                value(  value_  )
                {
                }


                typedef functor<ContextStatus,TL2(Context<T>&,size_t)> Control;
                
                struct Controls
                {
                    Control *step;
                    Control *atry;
                };


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Context);
            };


        }

    }
}

#endif

