#include "y/alchemy/actors.hpp"
#include "y/exception.hpp"

#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"
#include "y/code/decimal-chars.hpp"

namespace upsylon
{
    namespace alchemy
    {

        const char actors:: clid[] = "alchemy::actors";

        actors:: actors() : actors_(), max_coef(0)
        {
        }

        actors:: ~actors() throw()
        {
        }
        
        void actors:: operator()(const species &sp, const unsigned long nu)
        {
            if(nu<=0) throw exception("%s(nul coefficient for '%s')", clid, *sp.name);
            const actor a(sp,nu);
            if( !insert(a) ) throw exception("%s(multiple '%s')", clid, *sp.name);
            aliasing::_(max_coef) = max_of(max_coef,decimal_chars_for(nu));
        }
        

    }

}

