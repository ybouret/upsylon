

//! \file

#ifndef Y_GFX_STRETCH_INCLUDED
#define Y_GFX_STRETCH_INCLUDED 1

#include "y/gfx/area.hpp"


namespace upsylon
{
    namespace GFX
    {
        //______________________________________________________________________
        //
        //
        //! computation of a stretch of area for parallel computation
        //
        //______________________________________________________________________
        class Stretch
        {
        public:
            virtual ~Stretch() throw();
            explicit Stretch(const Area &area, const size_t size, const size_t rank) throw();

            const size_t items;
            const Point  lower;
            const Point  upper;

            friend std::ostream & operator<<(std::ostream &, const Stretch &);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Stretch);
        };

    }

}

#endif
