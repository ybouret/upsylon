

#include "y/alchemy/actors.hpp"
#include "y/exception.hpp"

#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace alchemy
    {

        const char actors:: clid[] = "alchemy::actors";

        actors:: actors() : actors_(), cw(0)
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

            const string tmp = vformat("%lu",nu);
            aliasing::_(cw) = max_of(cw,tmp.size());
        }

        std::ostream & actors:: display(std::ostream &os, const library &lib) const
        {
            if(size()>=1)
            {
                const_iterator it = begin();
                (*it).display(os,lib,cw);
                while( ++it != end() )
                {
                    os << '+';
                    (*it).display(os,lib,cw);
                }
            }
            return os;
        }

    }

}

