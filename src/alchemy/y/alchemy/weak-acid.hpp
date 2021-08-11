
//! \file

#ifndef Y_ALCHEMY_WEAK_ACID_INCLUDED
#define Y_ALCHEMY_WEAK_ACID_INCLUDED 1

#include "y/alchemy/equilibria.hpp"
#include "y/alchemy/library.hpp"

namespace upsylon
{
    namespace Alchemy
    {

        //! helper to describe weak-acid to eqs/lib
        struct WeakAcid
        {
            //! add name: acid <=> base + H+
            template <typename NAME, typename ACID, typename BASE> static inline
            Equilibrium & Add(Equilibria  &eqs,
                              Library     &lib,
                              const NAME  &name,
                              const ACID  &acid,
                              const BASE  &base,
                              const double Ka)
            {
                Equilibrium &eq = eqs(name,Ka);
                eq( 1, lib.get("H+") );
                eq(-1, lib.get(acid) );
                eq( 1, lib.get(base) );
                return eq;
            }

        };

    }

}

#endif
