//! \file

#ifndef Y_GRAPHIC_FILTER_GRADIENT_INCLUDED
#define Y_GRAPHIC_FILTER_GRADIENT_INCLUDED 1

#include "y/graphic/ops/filter.hpp"

namespace upsylon {

    namespace Graphic {

        class Gradients : public Object
        {
        public:
            typedef arc_ptr<Gradients> Pointer;
            const Kernel::Filter::Pointer X;
            const Kernel::Filter::Pointer Y;

            explicit Gradients(const Kernel::Filter::Pointer &GX,
                               const Kernel::Filter::Pointer &GY) throw();
            virtual ~Gradients() throw();
            Gradients(const Gradients &) throw();


        private:
            Y_DISABLE_ASSIGN(Gradients);
        };

    }

}


#endif


