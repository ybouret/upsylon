

#include "y/alchemy/actors.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace alchemy
    {

        const char actors:: clid[] = "alchemy::actors";

        actors:: actors() : actors_()
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
            
        }

        std::ostream & actors:: display(std::ostream  &os,
                                        const library &lib) const
        {
            if(size()>=1)
            {
                const_iterator it = begin();
                (*it).display(os,lib);
                while( ++it != end() )
                {
                    os << '+';
                    (*it).display(os,lib);
                }
            }
            return os;
        }

    }

}

