
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
        
        equilibria:: equilibria() : db(), tdisp(0)
        {
        }
        
        equilibrium & equilibria:: operator()(equilibrium *eq)
        {
            assert(eq);
            const equilibrium::pointer p(eq);
            if(!db.insert(p)) throw exception("multipliple equilibrium '%s'", *(eq->name) );
            return *eq;
        }
        
        

        const equilibrium::db  & equilibria:: bulk()  const throw()
        {
            return db;
        }

        

        void equilibria:: on_compile()
        {
            assert(!compiled);
            size_t w = 0;
            for(type::iterator it=db.begin();it!=db.end();++it)
            {
                equilibrium &eq = **it;
                eq.compile();
                w = max_of(w,eq.name.size());
            }
            
            for(type::iterator it=db.begin();it!=db.end();++it)
            {
                equilibrium &eq = **it;
                aliasing::_(eq.width) = w;
            }
            
        }
        
    }
}

