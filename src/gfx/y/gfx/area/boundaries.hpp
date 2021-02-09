//!\file

#ifndef Y_GFX_BOUNDARIES_INCLUDED
#define Y_GFX_BOUNDARIES_INCLUDED 1

#include "y/gfx/area/borders.hpp"
#include "y/gfx/area/corners.hpp"

namespace upsylon
{
    namespace GFX
    {
        //______________________________________________________________________
        //
        //
        //! pre-computed boundaries
        //
        //______________________________________________________________________
        class Boundaries : public Borders, public Corners
        {
        public:
            explicit Boundaries(const Area &area); //!< setup
            virtual ~Boundaries() throw();         //!< cleanup
            const    unit_t outer;                 //!< all items on outer shell

            //! display
            friend std::ostream & operator<<(std::ostream &, const Boundaries &);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Boundaries);
        };

    }
}

#endif

