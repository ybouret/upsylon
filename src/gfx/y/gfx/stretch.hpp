

//! \file

#ifndef Y_GFX_STRETCH_INCLUDED
#define Y_GFX_STRETCH_INCLUDED 1

#include "y/gfx/area.hpp"
#include "y/gfx/object.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace GFX
    {
        //______________________________________________________________________
        //
        //
        //! computation of a stretch of area for parallel computation
        //
        //______________________________________________________________________
        class Stretch
        {
        public:

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! cleanup
            ~Stretch() throw(); //!< cleanup

            //! setup, MPI style
            Stretch(const Area &area, const size_t size, const size_t rank) throw();



            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! display
            friend std::ostream & operator<<(std::ostream &, const Stretch &);
            unit_t operator[](const unit_t j) const throw(); //!< width[lower.y<=j<=upper.y]

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unit_t items; //!< items to work with
            const Point  lower; //!< position of first item
            const Point  upper; //!< position of last item
            
            
        private:
            size_t  count;
            size_t  bytes;
            unit_t *width;
            
            Y_DISABLE_COPY_AND_ASSIGN(Stretch);
        };

    }

}

#endif
