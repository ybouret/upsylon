
#include "y/aqua/equilibrium.hpp"
#include "y/exception.hpp"
#include "y/type/utils.hpp"
#include "y/ios/align.hpp"
#include "y/core/ipower.hpp"
#include <iomanip>

namespace upsylon {

    namespace Aqua
    {
       

        Equilibrium:: ~Equilibrium() throw() {}


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

            c = aliasing::_(components).push_back( new Component(sp,nu) );
            if(nu>0)
            {
                try
                {
                    aliasing::_(products).push_back( new Component(sp,nu) );
                }
                catch(...) { delete aliasing::_(components).pop_back(); throw; }
                aliasing::_(dp) += nu;
            }
            else
            {
                assert(nu<0);
                try {
                    aliasing::_(reactants).push_back( new Component(sp,-nu) );
                }
                catch(...) { delete aliasing::_(components).pop_back(); throw; }
                aliasing::_(dr) -= nu;
            }
            aliasing::_(dn) += nu;
            aliasing::_(idn) = dn!=0 ? 1.0/dn : 1;
            assert(dp-dr==dn);
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
            os << " | dn=" << dn;
            os << " | dr=" << dr;
            os << " | dp=" << dp;

            return os;
        }

        std::ostream & operator<<(std::ostream &os, const Equilibrium &eq)
        {
            return eq.display(os,0);
        }

        double Equilibrium:: K(const double t) const
        {
            const double K0 = getK(t);
            if(K0<=0) throw exception("<%s> negative constante",*name);
            return K0;
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



        void Equilibrium:: fillNu(addressable<Int> &nu) const throw()
        {
            for(size_t i=nu.size();i>0;--i)
            {
                nu[i] = 0;
            }
            for(const Component *c=components.head;c;c=c->next)
            {
                //std::cerr << c->sp.name << " @" << c->sp.indx << std::endl;
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

#include "y/mkl/tao.hpp"
#include "y/mkl/solve/zbis.hpp"

namespace upsylon
{

    namespace Aqua
    {
        using namespace mkl;
        void   Equilibrium:: evolve(addressable<double>      &Ctry,
                                    const accessible<double> &C0, const double x) const throw()
        {
            tao::set(Ctry,C0);
            for(const Component *c=components.head;c;c=c->next)
            {
                const size_t j = c->sp.indx;
                Ctry[j] = max_of(0.0,Ctry[j]+c->nu*x);
            }
        }

        namespace {
            
            struct EQZ
            {
                const Equilibrium        *self;
                const accessible<double> *pC0;
                addressable<double>      *pC1;
                double                    K0;

                inline double operator()(const double x) throw()
                {
                    self->evolve(*pC1,*pC0,x);
                    return self->computeQ(K0,*pC1);
                }

            };
        }

        static const char fn[] = "[ sweep ] ";

#define Y_AQUA_PRINTLN(MSG) do { if(verbose) { std::cerr << fn << MSG << std::endl; } } while(false)


        bool Equilibrium:: sweep(const double         K0,
                                 addressable<double> &C0,
                                 double               arr[],
                                 const bool           verbose) const
        {
            Y_AQUA_PRINTLN("<" << name << ">");

            const Extents ex(*this,C0,arr);
            Y_AQUA_PRINTLN( "\t" << ex );



            double xmin = 0;
            double xmax = 0;
            lightweight_array<double> C1(arr,C0.size());

            EQZ     F    = { this, &C0, &C1, K0 };
            double  Fmin = 0;
            double  Fmax = 0;

            switch(ex.reaches)
            {
                case Extents::ForwardLimited |  Extents::ReverseLimited:
                    if(ex.forward.maximum<=0&&ex.reverse.maximum<=0)
                    {
                        return false;
                    }
                    else
                    {
                        xmin = -ex.reverse.maximum; Fmin = F(xmin);
                        xmax =  ex.forward.maximum; Fmax = F(xmax);
                    }
                    break;

                case Extents::ForwardLimited |  Extents::ReverseEndless:
                    xmax = ex.forward.maximum; Fmax = F(xmax);
                    while( (Fmin=F(xmin))*Fmax > 0 ) xmin -= 1.0;
                    break;

                case Extents::ForwardEndless | Extents::ReverseLimited:
                    xmin = -ex.reverse.maximum; Fmin = F(xmin);
                    while( (Fmax=F(xmax))*Fmin > 0 ) xmax += 1.0;
                    break;

                default:
                    Y_AQUA_PRINTLN("\t!!corrupted!!");
                    return false;
            }

            Y_AQUA_PRINTLN("\tF(" << xmin << ")=" << Fmin << " | F(" << xmax << ")=" << Fmax);

            triplet<double>   x = { xmin, 0, xmax };
            triplet<double>   f = { Fmin, 0, Fmax };
            zbis<double> zsolve;

            if( !zsolve.run(F,x,f) )
            {
                Y_AQUA_PRINTLN("failure");
                return false;
            }

            tao::set(C0,C1);
            Y_AQUA_PRINTLN("<" << name << "> = " << f.b << " @ " << C0);


            return true;
        }

    }

}

