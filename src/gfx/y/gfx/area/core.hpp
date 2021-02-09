
//!\file

#ifndef Y_GFX_AREA_CORE_INCLUDED
#define Y_GFX_AREA_CORE_INCLUDED 1

#include "y/gfx/area/crate.hpp"

namespace upsylon
{
    namespace GFX
    {
        //______________________________________________________________________
        //
        //
        //! pre-computed core
        //
        //______________________________________________________________________
        class Core : public virtual Crate
        {
        public:
            explicit Core(const Area &); //!< setup
            virtual ~Core() throw();    //!< cleanup
            
            const SubArea::Handle core;   //!< optional core from area
            const unit_t          inner;  //!< all inner items

            //! display
            friend std::ostream & operator<<(std::ostream &, const Core &);



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Core);
        };

    }

}

#endif
