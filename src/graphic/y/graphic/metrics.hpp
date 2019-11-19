
//! \file

#ifndef Y_GRAPHIC_METRICS_INCLUDED
#define Y_GRAPHIC_METRICS_INCLUDED 1

#include "y/graphic/area.hpp"


namespace upsylon {

    namespace Graphic {

        class Metrics : public Area
        {
        public:
            virtual ~Metrics() throw();
            explicit Metrics(const size_t W,
                             const size_t H,
                             const size_t D);

            explicit Metrics(const size_t W,
                             const size_t H,
                             const size_t D,
                             const size_t S);

            const size_t depth;
            const size_t scanline;
            const size_t stride;



        private:
            Y_DISABLE_ASSIGN(Metrics);
        };

    }

}

#endif

