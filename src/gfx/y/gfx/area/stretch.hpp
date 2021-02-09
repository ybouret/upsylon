

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
        //! Horizontal Scan
        //
        //______________________________________________________________________
        
        class HScan
        {
        public:
            const Point  begin; //!< x,y
            const unit_t width; //!< x<=i<x+w
            const unit_t x_top; //!< x <=i<=x_top
            
            Point end() const throw(); //!< x_top, y
            
           
            HScan(const unit_t x, const unit_t y, const unit_t w) throw(); //!< setup
            ~HScan() throw();                                              //!< cleanup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(HScan);
        };
        
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
            
            //! HTile in [lower.y,upper.y]
            const HScan &operator[](const unit_t j) const throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unit_t items; //!< items to work with
            const Point  lower; //!< position of first item
            const Point  upper; //!< position of last item
            
            
        private:
            size_t     count;
            size_t     bytes;
            HScan     *hscan;
            
            Y_DISABLE_COPY_AND_ASSIGN(Stretch);
        };

    }

}

#endif
