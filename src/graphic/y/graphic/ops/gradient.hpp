//! \file

#ifndef Y_GRAPHIC_GRADIENT_INCLUDED
#define Y_GRAPHIC_GRADIENT_INCLUDED 1

#include "y/graphic/parallel/ops.hpp"

namespace upsylon {

    namespace  Graphic {

        //! fast gradient computation
        class Gradient : public Pixmap<float>
        {
        public:
            Pixmap<float> field;

            //! setup
            explicit Gradient(const size_t W, const size_t H);

            //! cleanup
            virtual ~Gradient() throw();

            //! compute
            void compute( Tiles &tiles, bool doNormalize=true);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Gradient);
            void normalize(Tiles &tiles) throw();

        };

    }
}

#endif
