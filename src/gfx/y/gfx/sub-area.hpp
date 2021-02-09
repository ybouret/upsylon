
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
        //! named sub-area fron an area and a position flag
        //
        //______________________________________________________________________
        class SubArea : public object, public Area
        {
        public:
            //__________________________________________________________________
            //
            // Types and definitions
            //__________________________________________________________________
            typedef auto_ptr<const SubArea> Handle; //!< alias
            

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit SubArea(const Area &area, const unsigned flag);  //!< setup
            virtual ~SubArea() throw();                               //!< cleanup


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unsigned position; //!< flag



        private:
            Y_DISABLE_COPY_AND_ASSIGN(SubArea);
        };

        

    }

}

#endif
