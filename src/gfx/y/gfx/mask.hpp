
//! \file

#ifndef Y_GFX_MASK_INCLUDED
#define Y_GFX_MASK_INCLUDED 1

#include "y/gfx/types.hpp"
#include "y/associative/suffix/graph.hpp"

namespace upsylon
{

    namespace graphic
    {

        typedef suffix_graph<uint8_t,coord> mask_;

        class mask : public mask_
        {
        public:
            static coord coord_to_key(const coord &) throw();

            explicit mask();
            virtual ~mask() throw();

            void insert(const coord p);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(mask);
        };

    }

}

#endif

