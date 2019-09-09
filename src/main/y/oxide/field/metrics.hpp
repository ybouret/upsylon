//! \file
#ifndef Y_OXIDE_FIELD_METRICS_INCLUDED
#define Y_OXIDE_FIELD_METRICS_INCLUDED 1

#include "y/oxide/types.hpp"

namespace upsylon
{
    namespace Oxide
    {
        //! metrics information
        template <size_t DIM> struct Metrics;

        //! 1D Metrics
        template <> struct  Metrics<1>
        {
            static const size_t LocalNodes  = 3;            //!< [-1:0:1]
            static const size_t Neighbours  = LocalNodes-1; //!< exclude center=hub
            static const size_t AtLevel1    = 2;            //!< along 2 main axis
            static const size_t AtLevel2    = 0;            //!< N/A
            static const size_t AtLevel3    = 0;            //!< N/A
        };

        //! 2D Metrics
        template <> struct  Metrics<2>
        {
            static const size_t LocalNodes  = 9;            //!< [-1:0:1]^2
            static const size_t Neighbours  = LocalNodes-1; //!< exclude center=hub
            static const size_t AtLevel1    = 4;            //!< along 4 mains
            static const size_t AtLevel2    = 4;            //!< along 4 diagonals
            static const size_t AtLevel3    = 0;            //!< N/A
        };

        //! 3D Metrics
        template <> struct  Metrics<3>
        {
            static const size_t LocalNodes  = 27;           //!< [-1:0:1]^3
            static const size_t Neighbours  = LocalNodes-1; //!< exclude center=hub
            static const size_t AtLevel1    = 6;            //!< across 6  axis
            static const size_t AtLevel2    = 12;           //!< across 12 edges
            static const size_t AtLevel3    = 8;            //!< across 8  vertices
        };

    }

}

#endif

