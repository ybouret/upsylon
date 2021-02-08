//! \file

#ifndef Y_GFX_TYPES_INCLUDED
#define Y_GFX_TYPES_INCLUDED 1

#include "y/type/point2d.hpp"
#include "y/core/zero-flux-index.hpp"

namespace upsylon
{
    namespace GFX
    {

        typedef point2d<unit_t>          Point;        //!< logical coordinate
        typedef core::zero_flux_index<0> ZeroFlux;     //!< for operators


        //! internal checking
        struct Check
        {
            static unit_t GTZ(const unit_t  w, const char *id, const char *fn); //!< raise exception if w<=0
            static unit_t GEQZ(const unit_t w, const char *id, const char *fn); //!< raise exception if w<0
            static const char Width[];  //!< "width"
            static const char Height[]; //!< "height"
        };

        struct Position
        {
            static const unsigned Core   = 0x00;
            static const unsigned Top    = 0x01;
            static const unsigned Bottom = 0x02;
            static const unsigned Left   = 0x04;
            static const unsigned Right  = 0x08;
            static const char *   Text(const unsigned) throw();
        };

#define Y_GFX_POSITION_DECL(FLAG,NAME) typedef int2type<Position::FLAG> NAME##_; extern const NAME##_ NAME
#define Y_GFX_POSITION_IMPL(NAME) const NAME##_ NAME = {}

        Y_GFX_POSITION_DECL(Core,InCore);
        Y_GFX_POSITION_DECL(Top,AtTop);
        Y_GFX_POSITION_DECL(Bottom,AtBottom);
        Y_GFX_POSITION_DECL(Left,AtLeft);
        Y_GFX_POSITION_DECL(Right,AtRight);


        
    }
}

#endif
