
#include "y/chem/equilibrium.hpp"
#include "y/exception.hpp"
#include "y/sort/merge.hpp"

namespace upsylon
{
    namespace Chemical
    {
        Equilibrium:: Component:: ~Component() throw() {}

        Equilibrium::Component:: Component( Species &s, const int n) throw() :
        next(0), prev(0),
        sp( &s ),
        nu( n  )
        {
            assert( s.refcount() > 1 );

        }

        int Equilibrium::Component::Compare(const Component *lhs, const Component *rhs,void*)
        {
            return comparison::increasing(lhs->sp->indx,rhs->sp->indx);
        }
    }
}

#include "y/type/utils.hpp"

namespace upsylon
{
    namespace Chemical
    {

        std::ostream & operator<<( std::ostream &os, const Equilibrium &rxn )
        {
            os << rxn.name << " : ";
            {
                size_t count = 0;
                for(const Equilibrium::Component *r=rxn.r_list.head;r;r=r->next)
                {
                    const string name = r->sp->name;
                    const int    coef = r->nu;
                    assert(coef<0);
                    if(count>0)
                    {
                        os << '+';
                    }
                    if(coef<-1)
                    {
                        os << -coef;
                    }
                    os << '(' << name << ')';
                    ++count;
                }
            }
            os << " <=> ";
            {
                size_t count = 0;
                for(const Equilibrium::Component *p=rxn.p_list.head;p;p=p->next)
                {
                    const string name = p->sp->name;
                    const int    coef = p->nu;
                    assert(coef>0);
                    if(count>0)
                    {
                        os << '+';
                    }
                    if(coef>1)
                    {
                        os << -coef;
                    }
                    os << '(' << name << ')';
                    ++count;
                }
            }
            os << " | K=" << rxn(0);
            return os;
        }


        Equilibrium:: ~Equilibrium() throw()
        {
        }

        const string & Equilibrium:: key() const throw()
        {
            return name;
        }

        const Equilibrium::Component::List  & Equilibrium:: reactants() const throw()
        {
            return r_list;
        }

        const Equilibrium::Component::List  & Equilibrium:: products() const throw()
        {
            return p_list;
        }

        Equilibrium::Component * Equilibrium:: extract(const Species &sp) throw()
        {
            for(Component *r=r_list.head;r;r=r->next)
            {
                if( & *(r->sp) == &sp )
                {
                    return r_list.unlink(r);
                }
            }

            for(Component *p=p_list.head;p;p=p->next)
            {
                if( & *(p->sp) == &sp )
                {
                    return p_list.unlink(p);
                }
            }

            return 0;
        }

        void Equilibrium:: add( Species &sp, const int nu )
        {
            Component *c = extract(sp);
            if(c)
            {
                (int &)(c->nu) += nu;
                if(0==c->nu)
                {
                    delete c;
                    return;
                }
                // else will insert
            }
            else
            {
                if(nu!=0)
                {
                    c = new Component(sp,nu);
                }
                else
                {
                    return;
                }
            }
            
            assert(c!=NULL);
            assert(c->nu!=0);
            if(c->nu>0)
            {
                p_list.push_back(c);
                merging<Component>::sort(p_list,Component::Compare,NULL);
            }
            else
            {
                assert(c->nu<0);
                r_list.push_back(c);
                merging<Component>::sort(r_list,Component::Compare,NULL);
            }
        }

        void Equilibrium:: validate()
        {
            if(p_list.size+r_list.size<=0) throw exception("%s: no component!", *name);
            merging<Component>::sort(p_list,Component::Compare,NULL);
            merging<Component>::sort(r_list,Component::Compare,NULL);
            int dz = 0;
            for(const Component *c=p_list.head;c;c=c->next)
            {
                dz += (c->sp->z*c->nu);
            }
            for(const Component *c=r_list.head;c;c=c->next)
            {
                dz += (c->sp->z*c->nu);
            }
            if(dz!=0) throw exception("%s does NOT conserve charge (deltaZ=%d)", *name, dz);
        }


        double Equilibrium:: Gamma( const double K, const array<double> &C ) const throw()
        {
            double r_prod = 1;
            double p_prod = 1;
            for(const Component *r = r_list.head;r;r=r->next)
            {
                const size_t id = r->sp->indx; assert(id>0); assert(id<C.size());
                const int    nu = r->nu;       assert(nu<0);
                r_prod *= ipower<double>(C[id],-nu);
            }
            for(const Component *p = p_list.head;p;p=p->next)
            {
                const size_t id = p->sp->indx; assert(id>0); assert(id<C.size());
                const int    nu = p->nu;       assert(nu>0);
                p_prod *= ipower<double>(C[id],nu);
            }
            return r_prod * K - p_prod;
        }

        double Equilibrium:: GammaAndPhi( array<double> &grad, const double K, const array<double> &C) const throw()
        {
            double r_prod = 1;
            double p_prod = 1;
            for(const Component *r=r_list.head;r;r=r->next)
            {
                const size_t id = r->sp->indx; assert(id>0); assert(id<C.size());
                const int    nu = r->nu;       assert(nu<0);
                const int    q  = -nu;
                const double CC = C[id];
                r_prod *= ipower<double>(CC,q);

                double g=K*q*ipower(CC,q-1);
                for(const Component *s=r_list.head;s;s=s->next)
                {
                    if(s!=r)
                    {
                        g *= ipower<double>(C[s->sp->indx],-(s->nu));
                    }
                }
                grad[id] = g;
            }
            
            for(const Component *p = p_list.head;p;p=p->next)
            {
                const size_t id = p->sp->indx; assert(id>0); assert(id<C.size());
                const int    nu = p->nu;       assert(nu>0);
                const double CC = C[id];
                p_prod *= ipower<double>(CC,nu);

                double g = nu * ipower(CC,nu-1);

                for(const Component *s=p_list.head;s;s=s->next)
                {
                    if(s!=p)
                    {
                        g *= ipower<double>(C[s->sp->indx],(s->nu));
                    }
                }
                grad[id] = -g;

            }
            return r_prod * K - p_prod;
        }

        double Equilibrium:: computeMaximumReverse(const array<double> &C) const throw()
        {
            assert(p_list.size>0);
            bool   init  = true;
            double xiRev = 0;
            size_t idRev = 0;
            for(const Equilibrium::Component *c =p_list.head;c;c=c->next)
            {
                const size_t id = c->sp->indx;
                const double CC = max_of<double>(C[id],0);
                const int    nu = c->nu; assert(nu>0);
                const double xi = CC/nu;
                //std::cerr << "  |_[" << c->sp->name << "]=" << C[c->sp->indx] << " => " << xi << std::endl;
                if(init||xi<xiRev)
                {
                    init = false;
                    xiRev = xi;
                    idRev = id;
                }
            }
            //std::cerr << "   |_xiRev=" << xiRev << "@" << idRev << std::endl;
            return xiRev;
        }

        double Equilibrium:: computeMaximumForward(const array<double> &C) const throw()
        {
            assert(r_list.size>0);
            bool   init  = true;
            double xiFwd = 0;
            size_t idFwd = 0;
            for(const Equilibrium::Component *c =r_list.head;c;c=c->next)
            {
                const size_t id = c->sp->indx;
                const double CC = max_of<double>(C[id],0);
                const int    nu = c->nu; assert(nu<0);
                const double xi = CC/(-nu);
                //std::cerr << "  |_[" << c->sp->name << "]=" << C[c->sp->indx] << " => " << xi << std::endl;
                if(init||xi<xiFwd)
                {
                    init = false;
                    xiFwd = xi;
                    idFwd = id;
                }
            }
            //std::cerr << "   |_xiFwd=" << xiFwd << "@" << idFwd << std::endl;
            return xiFwd;
        }

    }
}

namespace upsylon
{
    namespace Chemical
    {

        void ConstEquilibrium::check() const
        {
            if(K<=0) throw exception("%s.K<=0", *name);
        }

    }
}
