
#include "y/aqua/equilibria.hpp"
#include "y/exception.hpp"
#include "y/type/utils.hpp"

namespace upsylon {

    namespace Aqua
    {

        Equilibria:: ~Equilibria() throw()
        {
        }

        Equilibria:: Equilibria() :
        suffix_tree<Equilibrium::Pointer>(),
        maxNameSize(0)
        {}

        Equilibrium & Equilibria:: operator()( Equilibrium *eq )
        {
            assert(eq);
            const Equilibrium::Pointer p(eq);
            if(!insert_by(p->name,p))
            {
                throw exception("multiple equilibrium <%s>",*(p->name));
            }
            maxNameSize = max_of(maxNameSize,p->name.size());
            return *eq;
        }

        std::ostream & Equilibria:: display(std::ostream &os) const
        {
            os << "<equilibria>" << std::endl;
            for(const_iterator it=begin();it!=end();++it)
            {
                const Equilibrium &eq = **it;
                eq.display(os << ' ',maxNameSize) << std::endl;
            }
            os << "<equilibria/>";
            return os;
        }

        std::ostream & operator<<(std::ostream &os, const Equilibria &eqs)
        {
            return eqs.display(os);
        }

        
    }

}


