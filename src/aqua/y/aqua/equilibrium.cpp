
#include "y/aqua/equilibrium.hpp"
#include "y/exception.hpp"
#include "y/type/utils.hpp"
#include <iomanip>
#include "y/ios/align.hpp"
#include "y/core/ipower.hpp"

namespace upsylon {

    namespace Aqua
    {
        Component:: ~Component() throw()
        {
        }

        Component::Component(const Species &sp_, const int nu_) throw() :
        sp(sp_),
        nu(nu_),
        p( abs_of(nu) ),
        pm1( p-1 )
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

            for(Component *c=components.head;c;c=c->next)
            {
                aliasing::_(c->sp.indx) = 0;
            }

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

        void Equilibrium:: validate() const
        {
            const char *id = *name;
            if(components.size<=0) throw exception("<%s> no components!",id);
            {
                int Z = 0;
                for(const Component *c=components.head;c;c=c->next)
                {
                    Z += c->nu * c->sp.Z;
                }
                if( Z!= 0 ) throw exception("<%s> does not conserve charge!",id);
            }

            {
                // todo: gcd
            }
        }


        ConstEquilibrium:: ~ConstEquilibrium() throw()
        {
            aliasing::_(Keq) = 0;
        }

        double ConstEquilibrium:: getK(const double) const
        {
            return Keq;
        }

      

        void Equilibrium:: fillNu(addressable<int> &nu) const throw()
        {
            for(size_t i=nu.size();i>0;--i)
            {
                nu[i] = 0;
            }
            for(const Component *c=components.head;c;c=c->next)
            {
                assert(c->sp.indx>0);
                assert(c->sp.indx<=nu.size());
                nu[c->sp.indx] = c->nu;
            }
        }

        double Equilibrium:: computeQ(const double K0, const accessible<double> &C) const throw()
        {
            double rhs = 1;
            
            if(products.size)
            {
                for(const Component *c = products.head;c;c=c->next)
                {
                    assert(c->sp.indx>0);
                    assert(c->sp.indx<=C.size());
                    assert(c->nu>0);
                    assert(c->p>0);
                    rhs *= ipower<double>(C[c->sp.indx],c->p);
                }
            }

            double lhs = K0;
            if(reactants.size)
            {
                for(const Component *c = reactants.head;c;c=c->next)
                {
                    assert(c->sp.indx>0);
                    assert(c->sp.indx<=C.size());
                    assert(c->nu>0);
                    assert(c->p>0);
                    lhs *= ipower<double>(C[c->sp.indx],c->p);
                }
            }

            return lhs-rhs;
        }


        double  Equilibrium:: computeQ(addressable<double>      &Phi,
                                       const double              K0,
                                       const accessible<double> &C) const throw()
        {
            assert(C.size()>=Phi.size());
            for(size_t j=Phi.size();j>0;--j)
            {
                Phi[j] = 0;
            }

            double rhs=1;

            for(const Component *c=products.head;c;c=c->next)
            {
                assert(c->sp.indx>0);
                assert(c->sp.indx<=Phi.size());
                assert(c->nu>0);
                assert(c->p>0);
                const size_t j     = c->sp.indx;
                const double Cj    = C[j];
                double       prod  = ipower<double>(Cj,c->pm1);
                rhs  *= prod * Cj; //!< Cj^nu
                prod *= c->p;

                // sub loops
                for(const Component *s=products.head;s!=c;s=s->next)
                {
                    prod *= ipower<double>(C[s->sp.indx],c->p);
                }
                
                for(const Component *s=c->next;s;s=s->next)
                {
                    prod *= ipower<double>(C[s->sp.indx],c->p);
                }
                Phi[j] = -prod;
            }

            double lhs = K0;
            for(const Component *c=reactants.head;c;c=c->next)
            {
                assert(c->sp.indx>0);
                assert(c->sp.indx<=Phi.size());
                assert(c->nu>0);
                assert(c->p>0);
                const size_t j     = c->sp.indx;
                const double Cj    = C[j];
                double       prod  =  ipower<double>(Cj,c->pm1);
                lhs  *= prod*Cj;
                prod *= K0 * (c->p);

                // sub loop
                for(const Component *s=reactants.head;s!=c;s=s->next)
                {
                    prod *= ipower<double>(C[s->sp.indx],c->p);
                }

                for(const Component *s=c->next;s;s=s->next)
                {
                    prod *= ipower<double>(C[s->sp.indx],c->p);
                }
                Phi[j] = prod;
            }

            return lhs-rhs;

        }

    }

}

