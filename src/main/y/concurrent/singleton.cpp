
#include "y/concurrent/singleton.hpp"
#include "y/type/aliasing.hpp"
#include <iostream>
#include <iomanip>
#include <cstring>

namespace upsylon
{
    namespace concurrent
    {

        static inline
        std::ostream &  __disp( std::ostream &os, const char *uuid, const size_t size, const size_t width)
        {
            if(width>size)
            {
                const size_t n=(width-size);
                const size_t h=n>>1;

                for(size_t i=0;i<h;++i)     os << ' ';
                for(size_t i=0;i<size;++i)  os << uuid[i];
                for(size_t i=h;i<n;++i)     os << ' ';
            }
            else
            {
                for(size_t i=0;i<size;++i)  os << uuid[i];
            }
            return os;
        }

        bool   singleton::verbose = false;
        mutex  singleton::gateway;
        size_t singleton::breadth = 0;


        singleton:: ~singleton() throw()
        {
            if(verbose)
            {
                __disp(std::cerr << "[-{" , uuid, _len, breadth) << "} life_time=" << std::setw(6) << span << "]" << std::endl;
            }
        }

        
        singleton::  singleton(const char              *u,
                               const at_exit::longevity s) throw() :
        uuid(0),
        span(s),
        _len(0)
        {

            Y_LOCK(gateway);
            assert(u);
            aliasing::_(uuid) = u;
            aliasing::_(_len) = strlen(uuid);
            if(_len>breadth) breadth=_len;

            if(verbose)
            {
                __disp(std::cerr << "[+{" , uuid, _len, breadth) << "} life_time=" << std::setw(6) << span << "]" << std::endl;
            }
        }
        

    }
    
}

