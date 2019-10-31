
//! \file

#ifndef Y_MATH_ADJUST_FRAME_INCLUDED
#define Y_MATH_ADJUST_FRAME_INCLUDED 1

#include "y/math/adjust/sample.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            class Frame
            {
            public:
                enum Status
                {
                    Success, //!< go on
                    Changed, //!< value has changed
                    Failure  //!< emergency exit
                };
                virtual ~Frame() throw();

                const accessible<bool>  &used;

            protected:
                explicit Frame(const accessible<bool> &used_) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Frame);
            };


            template <typename T>
            class FrameOf : public Frame
            {
            public:
                SampleType<T> &sample;

                inline virtual ~FrameOf() throw() {}

                inline explicit FrameOf(const accessible<bool> &used_,
                                        const SampleType<T>    &sample_,
                                        addressable<T>         &value_) throw() :
                Frame(  used_   ),
                sample( sample_ ),
                value(  value_  )
                {
                }

                addressable<T> &value;

                typedef functor<Frame::Status,TL2(FrameOf<T>&,size_t)> Process;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(FrameOf);
            };


        }

    }
}

#endif

