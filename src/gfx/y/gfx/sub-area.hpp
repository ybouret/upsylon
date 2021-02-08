
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
            typedef auto_ptr<const SubArea> Handle; //!< alias

            //! setup
            explicit SubArea(const Area &area, const unsigned flag);
            //! cleanup
            virtual ~SubArea() throw();
            
            const unsigned position; //!< flag

            //! base class to hold a set of sub-areas
            class Holder
            {
            public:
                virtual ~Holder() throw(); //!< cleanup
                const unit_t items;        //!< sum of hold items
                
            protected:
                explicit Holder() throw(); //!< setup
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Holder);
            };

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SubArea);
        };

        

    }

}

#endif
