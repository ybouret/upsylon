
#include "y/aqua/equilibrium.hpp"
#include "y/exception.hpp"
#include "y/type/utils.hpp"
#include <iomanip>
#include "y/ios/align.hpp"

namespace upsylon {

    namespace Aqua
    {
        Component:: ~Component() throw()
        {
        }

        Component::Component(const Species &sp_, const int nu_) throw() :
        sp(sp_),
        nu(nu_)
        {
        }

        Equilibrium:: ~Equilibrium() throw()
        {}

        void Equilibrium:: operator()(const Species &sp, const int nu)
        {
            if(nu==0) return;

            Component *c = components.head;
            while(c)
            {
                if( &sp == &(c->sp) )
                {
                    throw exception("<%s>: multiple species '%s'", *name, *(sp.name));
                }
                c=c->next;
            }

            c = components.push_back( new Component(sp,nu) );
            if(nu>0)
            {
                try {
                    products.push_back( new Component(sp,nu) );
                }
                catch(...) { delete components.pop_back(); throw; }
                aliasing::_(d_nu_p) += nu;
            }
            else
            {
                assert(nu<0);
                try {
                    reactants.push_back( new Component(sp,-nu) );
                }
                catch(...) { delete components.pop_back(); throw; }
                aliasing::_(d_nu_r) -= nu;
            }
            aliasing::_(d_nu) += nu;
            assert(d_nu_p-d_nu_r==d_nu);
            maxCompSize = max_of(maxCompSize,sp.name.size());
        }

        void Equilibrium:: display_list(std::ostream &os, const Components &l) const
        {
            for(const Component *c=l.head;c;c=c->next)
            {
                const int nu = c->nu; assert(nu>0);
                os << ' ';

                if(c!=l.head) os << '+' << ' ';
                if(nu>1)
                {
                    os << std::setw(2) << nu << '*';
                }
                os << '\'' << c->sp.name << '\'';
            }
        }


        std::ostream & Equilibrium:: display(std::ostream &os, const size_t width) const
        {
            os << '<' << name << '>'; for(size_t i=name.size();i<=width;++i) os << ' ';
            os << ':';
            display_list(os,reactants);
            os << " <=>";
            display_list(os,products);
            os << " (" << K(0) << ")";
            os << " | d_nu=" << d_nu;
            os << " | d_nu_r=" << d_nu_r;
            os << " | d_nu_p=" << d_nu_p;

            return os;
        }

        std::ostream & operator<<(std::ostream &os, const Equilibrium &eq)
        {
            return eq.display(os,0);
        }

        double Equilibrium:: K(const double t) const
        {
            const double value = getK(t);
            if(value<=0) throw exception("<%s> negative constante",*name);
            return value;
        }

        ConstEquilibrium:: ~ConstEquilibrium() throw()
        {
            aliasing::_(Keq) = 0;
        }

        double ConstEquilibrium:: getK(const double) const
        {
            return Keq;
        }

    }

}

