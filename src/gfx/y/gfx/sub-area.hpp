
//! \file

#ifndef Y_GFX_SUB_AREA_INCLUDED
#define Y_GFX_SUB_AREA_INCLUDED 1

#include "y/gfx/area.hpp"
#include "y/gfx/object.hpp"
#include "y/ptr/arc.hpp"
#include "y/ptr/auto.hpp"


namespace upsylon
{
    namespace GFX
    {
        //______________________________________________________________________
        //
        //
        //! basic sub-area
        //
        //______________________________________________________________________
        class SubArea : public Object, public Area
        {
        public:
            typedef arc_ptr<SubArea> Pointer;
            explicit SubArea(const Area &area, const unsigned flag);
            virtual ~SubArea() throw();
            SubArea(const SubArea &) throw();

            const unsigned position;

        private:
            Y_DISABLE_ASSIGN(SubArea);
        };

        typedef auto_ptr<const SubArea::Pointer> AreaHandle;

    }

}

#endif
