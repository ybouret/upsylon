

//! \file

#ifndef Y_GFX_AREA_TILE_INCLUDED
#define Y_GFX_AREA_TILE_INCLUDED 1

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
        //! computation of a Tile of area for parallel computation
        //
        //______________________________________________________________________
        class Tile : public accessible<HScan>
        {
        public:

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! cleanup
            ~Tile() throw(); //!< cleanup

            //! setup, MPI style
            Tile(const Area  &area,
                 const size_t sz,
                 const size_t rk) throw();



            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual size_t size() const throw();
            virtual const HScan &operator[](const size_t) const throw();

            //! display
            friend std::ostream & operator<<(std::ostream &, const Tile &);
            
            

            unit_t items() const throw(); //!< count items
            Point  lower() const throw(); //!< lower point
            Point  upper() const throw(); //!< upper point
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________

        private:
            const size_t height;
            size_t       count;
            size_t       bytes;
            HScan       *hscan;
            
            Y_DISABLE_COPY_AND_ASSIGN(Tile);
        };

    }

}

#endif
