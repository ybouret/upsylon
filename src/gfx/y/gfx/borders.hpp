//! \file

#ifndef Y_GFX_BORDERS_INCLUDED
#define Y_GFX_BORDERS_INCLUDED 1

#include "y/gfx/sub-area.hpp"

namespace upsylon
{
    namespace GFX
    {
        //______________________________________________________________________
        //
        //
        //! pre-computed borders
        //
        //______________________________________________________________________
        class Borders : public SubArea::Holder
        {
        public:
            explicit Borders(const Area &area); //!< setup
            virtual ~Borders() throw();         //!< cleanup

            const SubAreaHandle bottom;
            const SubAreaHandle top;
            const SubAreaHandle left;
            const SubAreaHandle right;

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Borders);
        };
    }

}
#endif

