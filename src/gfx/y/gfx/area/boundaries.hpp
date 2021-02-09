//!\file

#ifndef Y_GFX_BOUNDARIES_INCLUDED
#define Y_GFX_BOUNDARIES_INCLUDED 1

#include "y/gfx/area/borders.hpp"
#include "y/gfx/area/corners.hpp"

namespace upsylon
{
    namespace GFX
    {

        class Boundaries : public Borders, public Corners
        {
        public:
            explicit Boundaries(const Area &area);
            virtual ~Boundaries() throw();
            virtual  unit_t items() const throw();

            friend std::ostream & operator<<(std::ostream &, const Boundaries &);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Boundaries);
        };

    }
}

#endif

