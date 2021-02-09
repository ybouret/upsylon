//! \file

#ifndef Y_GFX_BORDERS_INCLUDED
#define Y_GFX_BORDERS_INCLUDED 1

#include "y/gfx/crate.hpp"

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
        class Borders : public virtual Crate
        {
        public:
            explicit Borders(const Area &area); //!< setup
            virtual ~Borders() throw();         //!< cleanup
            virtual unit_t items() const throw();

            friend std::ostream & operator <<(std::ostream &, const Borders &);


            const SubArea::Handle bottom; //!< bottom area
            const SubArea::Handle top;    //!< top    area
            const SubArea::Handle left;   //!< left   area
            const SubArea::Handle right;  //!< right  area

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Borders);
        };
    }

}
#endif

