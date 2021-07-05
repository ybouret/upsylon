
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

            mask();
            mask(const mask&);
            virtual ~mask() throw();

            void insert(const coord p);

            mask & operator += (const coord);
            mask & operator += (const mask&);

        private:
            Y_DISABLE_ASSIGN(mask);

        };

    }

}

#endif

