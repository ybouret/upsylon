
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
        
        void  equilibria:: look_up(size_t &rlen, size_t &plen) const throw()
        {
            rlen = plen = 0;
            for(const_iterator it=db.begin();it!=db.end();++it)
            {
                const equilibrium &eq = **it;
                rlen = max_of(rlen,eq.reac.cwidth);
                plen = max_of(plen,eq.prod.cwidth);
            }
        }

        const equilibria::db_type & equilibria:: operator*()  const throw()
        {
            return db;
        }

        const equilibria::db_type * equilibria:: operator->()  const throw()
        {
            return &db;
        }

        
    }
}

