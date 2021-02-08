//! \file

#ifndef Y_GFX_TYPES_INCLUDED
#define Y_GFX_TYPES_INCLUDED 1

#include "y/type/point2d.hpp"
#include "y/core/zero-flux-index.hpp"

namespace upsylon
{
    namespace GFX
    {
        //______________________________________________________________________
        //
        //
        // global types
        //
        //______________________________________________________________________
        typedef point2d<unit_t>          Point;        //!< logical coordinate
        typedef core::zero_flux_index<0> ZeroFlux;     //!< for operators


        //______________________________________________________________________
        //
        //
        //! internal checking
        //
        //______________________________________________________________________
        struct Check
        {
            static unit_t GTZ(const unit_t  w, const char *id, const char *fn); //!< raise exception if w<=0
            static unit_t GEQZ(const unit_t w, const char *id, const char *fn); //!< raise exception if w<0
            static const char Width[];  //!< "width"
            static const char Height[]; //!< "height"
        };


        //______________________________________________________________________
        //
        //
        //! position flags
        //
        //______________________________________________________________________
        struct Position
        {
            static const unsigned Core   = 0x00; //!< core
            static const unsigned Top    = 0x01; //!< top
            static const unsigned Bottom = 0x02; //!< bottom
            static const unsigned Left   = 0x04; //!< left
            static const unsigned Right  = 0x08; //!< right
            static const char *   Text(const unsigned) throw(); //!< human readable position
            static const unsigned At[9]; //!< possible positions
        };

        //! declare a named position
#define Y_GFX_POSITION_DECL(FLAG,NAME) typedef int2type<Position::FLAG> NAME##_; extern const NAME##_ NAME

        //! implement a named position
#define Y_GFX_POSITION_IMPL(NAME) const NAME##_ NAME = {}

        Y_GFX_POSITION_DECL(Core,InCore);        //!< InCore
        Y_GFX_POSITION_DECL(Top,AtTop);          //!< AtTop
        Y_GFX_POSITION_DECL(Bottom,AtBottom);    //!< AtBottom
        Y_GFX_POSITION_DECL(Left,AtLeft);        //!< AtLeft
        Y_GFX_POSITION_DECL(Right,AtRight);      //!< AtRight

        
        
    }
}

#endif
