

//! \file

#ifndef Y_GFX_STRETCH_INCLUDED
#define Y_GFX_STRETCH_INCLUDED 1

#include "y/gfx/area.hpp"


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
            virtual ~Stretch() throw(); //!< cleanup

            //! setup, MPI style
            explicit Stretch(const Area &area, const size_t size, const size_t rank) throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! display
            friend std::ostream & operator<<(std::ostream &, const Stretch &);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t items; //!< items to work with
            const Point  lower; //!< position of first item
            const Point  upper; //!< position of last item
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Stretch);
        };

    }

}

#endif
