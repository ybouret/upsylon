
#include "y/alchemy/equilibria.hpp"
#include "y/exception.hpp"
#include "y/type/utils.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace alchemy
    {
        equilibria:: ~equilibria() throw()
        {
            
        }
        
        equilibria:: equilibria() : db(), max_name(0)
        {
        }
        
        equilibrium & equilibria:: operator()(equilibrium *eq)
        {
            assert(eq);
            const equilibrium::pointer p(eq);
            if(!db.insert(p)) throw exception("multipliple equilibrium '%s'", *(eq->name) );
            aliasing::_(max_name) = max_of(max_name,eq->name.size());
            return *eq;
        }
        
        std::ostream & equilibria:: display(std::ostream &os, const library &lib, const double t) const
        {
            for(db_type::const_iterator it=db.begin();it!=db.end();++it)
            {
                (**it).display(os,lib,t,max_name) << std::endl;
            }
            return os;
        }

    }
}

