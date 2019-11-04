
//! \file

#ifndef Y_MATH_ADJUST_FRAME_INCLUDED
#define Y_MATH_ADJUST_FRAME_INCLUDED 1

#include "y/math/adjust/sample.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {


            //! result of a frame control function
            enum ControlResult
            {
                LeftUntouched, //!< nothing was done
                ModifiedValue, //!< value has changed
                EmergencyExit  //!< OK, I'm out!
            };


            //! transient sample+used+value to monitor
            /**
             Frames are internally created by the LeastSquares algorithm
             to allow user's interaction with the computed step and the
             proposed full-step trial parameters
             */
            template <typename T>
            class Frame
            {
            public:
                const SampleType<T>     &sample; //!< user's sample
                const accessible<bool>  &used;   //!< currently used flags
                addressable<T>          &value;  //!< value to monitor

                inline   ~Frame() throw() {}  //!< cleanup

                //! setup
                inline   Frame(const SampleType<T>    &sample_,
                               const accessible<bool> &used_,
                               addressable<T>         &value_) throw() :
                sample( sample_ ),
                used(   used_   ),
                value(  value_  )
                {
                }

                //! control functor type
                typedef functor<ControlResult,TL2(Frame<T>&,size_t)> Control;

                //! pair of control functions, to carry information
                class Controls
                {
                public:
                    //! cleanup
                    inline ~Controls() throw() { step=atry=0; }

                    //! setup
                    inline  Controls(Control *step_=0,Control *atry_=0) throw() :
                    step(step_),atry(atry_) {}

                    Control *step; //!< optional step-controlling function
                    Control *atry; //!< optional atry-controlling function
                    
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Controls);
                };


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Frame);
            };


        }

    }
}

#endif

