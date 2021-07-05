//! \file

#ifndef Y_GFX_AREA_INCLUDED
#define Y_GFX_AREA_INCLUDED 1

#include "y/gfx/types.hpp"

namespace upsylon
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! basic area
        //
        //______________________________________________________________________
        class area
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const coord delta[4+4]; //!< neighbour coords

            //__________________________________________________________________________
            //
            //! special for transposition
            //__________________________________________________________________________
            struct transpose_t {};

            //__________________________________________________________________________
            //
            //! helper for transposition
            //__________________________________________________________________________
            static const transpose_t transpose;


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! setup
            explicit area(const unit_t W, const unit_t H, const unit_t X=0, const unit_t Y=0);
            area(const area &) throw();                      //!< copy
            area(const area &, const transpose_t &) throw(); //!< copy/transposed
            virtual ~area()    throw();                      //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool owns(const unit_t x, const unit_t y) const throw();        //!< in area ?
            bool owns(const coord p)                  const throw();        //!< in area ?
            bool owns(const area &sub)                const throw();        //!< lower and upper in area
            friend std::ostream & operator<<(std::ostream &, const area &); //!< display
            bool has_same_metrics_than(const area &other) const throw();    //!< check metrics

            coord   coord_of(const unit_t i) const throw();                 //!< lower+( i%w, i/w )
            unit_t  index_of(coord        p) const throw();                 //!< (p-lower).x + (p-lower).y * w
            unit_t  index_of(const unit_t X, const unit_t Y) const throw(); //!< index_of( coord(X,Y) )

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unit_t w;       //!< width  >= 0
            const unit_t h;       //!< height >= 0
            const unit_t items;   //!< items = w*h
            const coord  lower;   //!< lower coordinate
            const coord  upper;   //!< upper coordinate

        private:
            Y_DISABLE_ASSIGN(area);
        };
        
        
    }
}

//! local bounds extractions
#define Y_GFX_AREA_LIMITS(A) \
const unit_t xmin = (A).lower.x; const unit_t xmax= (A).upper.x;\
const unit_t ymin = (A).lower.y; const unit_t ymax= (A).upper.y


#endif

