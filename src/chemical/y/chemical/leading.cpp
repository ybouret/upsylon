#include "y/chemical/leading.hpp"

namespace upsylon
{
    namespace Chemical
    {
        Leading:: ~Leading() throw()
        {
        }

        Leading:: Leading(const Equilibrium &eq) :
        reac( eq.reac.tellLeading(), as_capacity),
        prod( eq.prod.tellLeading(), as_capacity)
        {
        }
        
    }
}
