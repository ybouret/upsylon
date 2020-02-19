
#include "y/information/filter/byte.hpp"
#include "y/information/shift/echo.hpp"

namespace upsylon {

    namespace information {

        bytewise_filter:: ~bytewise_filter() throw()
        {
        }

        bytewise_filter:: bytewise_filter( const shift::pointer &p ) throw() :
        transform(p)
        {
        }

        bytewise_filter:: bytewise_filter( shift *p )  :
        transform(p?p:new echo_modulation())
        {

        }

        void bytewise_filter:: write(char C)
        {
            push_back( transform->fetch(C) );
        }

        void bytewise_filter:: flush()
        {
        }

        void bytewise_filter:: reset() throw()
        {
            transform->reset();
            free();
        }

    }

}

