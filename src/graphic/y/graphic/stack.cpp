
#include "y/graphic/stack.hpp"

namespace upsylon {

    namespace Graphic {


        template <>
        void Stack<float>:: average(Pixmap<float> &target, const size_t ini, const size_t end) const
        {
            average_<float,1>(target, ini, end);
        }

        template <>
        void Stack<uint8_t>:: average(Pixmap<uint8_t> &target, const size_t ini, const size_t end) const
        {
            average_<uint8_t,1>(target, ini, end);
        }

        template <>
        void Stack<rgb>:: average(Pixmap<rgb> &target, const size_t ini, const size_t end) const
        {
            average_<uint8_t,3>(target, ini, end);
        }

        template <>
        void Stack<rgba>:: average(Pixmap<rgba> &target, const size_t ini, const size_t end) const
        {
            average_<uint8_t,3>(target, ini, end);
        }

    }

}

