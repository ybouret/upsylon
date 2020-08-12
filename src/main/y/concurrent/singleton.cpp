
#include "y/concurrent/singleton.hpp"
#include <iostream>
#include <iomanip>

namespace upsylon
{
    namespace concurrent
    {
        
        bool singleton::verbose = false;

        singleton:: ~singleton() throw()
        {
            if(verbose)
            {
                std::cerr << "[-singleton<" << uuid << "> life_time=" << std::setw(6) << span << "]" << std::endl;
            }
        }

        singleton::  singleton(const char              *u,
                               const at_exit::longevity s) throw() :
        uuid(u),
        span(s)
        {
            if(verbose)
            {
                std::cerr << "[+singleton<" << uuid << "> life_time=" << std::setw(6) << span << "]" << std::endl;
            }
        }
        

    }
    
}

