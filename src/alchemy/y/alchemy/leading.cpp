#include "y/alchemy/leading.hpp"

namespace upsylon
{
    namespace Alchemy
    {

        const char Leading:: EXT_[] = "@";
        const char Leading:: LEQ_[] = " <= -";
        const char Leading:: GEQ_[] = " >=  ";


        Leading:: ~Leading() throw()
        {
        }

        Leading:: Leading(const Equilibrium &eq_,
                          const Species     &sp_,
                          const size_t       nu_,
                          const Kind         id_) throw() :
        eq(eq_),
        sp(sp_),
        nu(nu_),
        id(id_)
        {
            assert( eq.weight(sp) == nu );
        }

        Leading:: Leading(const Leading &other) throw() :
        eq(other.eq),
        sp(other.sp),
        nu(other.nu),
        id(other.id)
        {
        }
        


    }

}

