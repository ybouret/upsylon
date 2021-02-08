
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
        class SubArea : public object, public Area
        {
        public:
            explicit SubArea(const Area &area, const unsigned flag);
            virtual ~SubArea() throw();
            SubArea(const SubArea &) throw();

            const unsigned position; //!< flag

            //! base class to hold data
            class Holder
            {
            public:
                virtual ~Holder() throw();

                const unit_t count;
                
            protected:
                explicit Holder() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Holder);
            };

        private:
            Y_DISABLE_ASSIGN(SubArea);
        };

        typedef auto_ptr<const SubArea> SubAreaHandle;

    }

}

#endif
