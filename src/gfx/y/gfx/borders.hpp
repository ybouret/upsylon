

//! \file

#ifndef Y_GFX_BORDERS_INCLUDED
#define Y_GFX_BORDERS_INCLUDED 1

#include "y/gfx/sub-area.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace GFX
    {

        class Borders
        {
        public:
            explicit Borders(const Area &area); //!< setup
            Borders(const Borders &);           //!< setup
            virtual ~Borders() throw();         //!< cleanup

            const Area bottom;
            const Area top;
            const Area left;
            const Area right;

        private:
            Y_DISABLE_ASSIGN(Borders);
        };
    }

}
#endif

