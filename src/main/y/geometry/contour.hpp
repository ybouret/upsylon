//! \file
#ifndef Y_GEOM_CONTOUR_INCLUDED
#define Y_GEOM_CONTOUR_INCLUDED 1

#include "y/memory/pooled.hpp"
#include "y/ordered/sorted-vector.hpp"

namespace upsylon {

    namespace geometry {

        //======================================================================
        //
        //! helper for any contour
        //
        //======================================================================
        struct contour
        {
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------

            typedef memory::pooled                             allocator;   //!< local memory allocator
            typedef increasing_comparator<double>              comparator;  //!< local comparator
            typedef sorted_vector<double,comparator,allocator> levels_type; //!< for levels
            typedef ordered_multiple<levels_type>              levels;      //!< set of multiple ordered values

            //! machine precision sign
            enum sign_type
            {
                is_negative, //!< value<0
                is_zero,     //!< value==0
                is_positive  //!< value>0
            };

            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------

            //! convert value to sign_type
            static sign_type sign_of( const double value ) throw();

            //! get the most precise 4-average
            static double average( const double, const double, const double, const double ) throw();
        };
    }
}
#endif

