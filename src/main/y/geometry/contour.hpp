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
        struct Contour
        {
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------

            typedef memory::pooled                             Allocator;   //!< local memory allocator
            typedef increasing_comparator<double>              Comparator;  //!< local comparator
            typedef sorted_vector<double,Comparator,Allocator> LevelsType; //!< for levels
            typedef ordered_multiple<LevelsType>               Levels;      //!< set of multiple ordered values

            //! machine precision sign
            enum SignType
            {
                IsNegative, //!< value<0
                IsZero,     //!< value==0
                IsPositive  //!< value>0
            };

            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------

            //! convert value to sign_type
            static SignType SignOf( const double value ) throw();

            //! get the most precise 4-average
            static double Average( const double, const double, const double, const double ) throw();
        };
    }
}
#endif

