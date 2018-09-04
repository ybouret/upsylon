#include "y/chem/equilibria.hpp"
#include "y/exception.hpp"
#include "y/math/kernel/tao.hpp"
#include "y/math/kernel/determinant.hpp"

namespace upsylon
{
    using namespace math;

    namespace Chemical
    {
        Equilibria:: ~Equilibria() throw()
        {
        }

        Equilibria:: Equilibria(const size_t n) :
        EquilibriaType(n,as_capacity),
        N(0),
        M(0),
        active(),
        rxn(),
        Nu(),
        Phi(),
        W(),
        K(),
        Gamma()
        {
        }

        Equilibrium & Equilibria:: enroll( Equilibrium *rxn )
        {
            assert(rxn);
            Equilibrium::Pointer p(rxn);
            if( !insert(p) )
            {
                throw exception("multiple reaction '%s'",*(p->name));
            }
            return *p;
        }

        size_t Equilibria:: max_name_length() const throw()
        {
            size_t ans = 0;
            for(const_iterator i=begin();i!=end();++i)
            {
                const size_t tmp = (*i)->name.size();
                if(tmp>ans)
                {
                    ans = tmp;
                }
            }
            return ans;
        }

        std::ostream & operator<<( std::ostream &os, const Equilibria &cs )
        {
            const size_t sz = cs.max_name_length();
            const size_t num = cs.size();
            size_t       idx = 1;
            for(Equilibria::const_iterator i=cs.begin();i!=cs.end();++i,++idx)
            {
                os << " (*)";
                for(size_t j=(*i)->name.size();j<=sz;++j) os << ' ';
                os << (**i);
                if(idx<num) os << std::endl;
            }
            return os;
        }

        void Equilibria:: release_all() throw()
        {
            (size_t &)N = 0;
            (size_t &)M = 0;
            rxn.free();
            excess.free();
            active.free();
        }

        void Equilibria:: compile_for( Library &lib )
        {

            lib.update();
            for(iterator i=begin();i!=end();++i)
            {
                (*i)->validate();
            }
            
            try
            {
                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________
                release_all();
                size_t &n = (size_t &)N;
                size_t &m = (size_t &)M;
                n = this->size();
                m = lib.size();
                if(M<=0)
                {
                    if(N>0)
                    {
                        throw exception("unexpected %u reactions with no species!", unsigned(N));
                    }
                    return;
                }

                assert(M>0);
                active.make(M,false);
                excess.make(M,0);
                Cini.make(M,0);
                Ctry.make(M,0);
                
                //______________________________________________________________
                //
                // construct data from equilibria
                //______________________________________________________________
                if(N>0)
                {
                    rxn.free();
                    rxn.ensure(N);
                    Nu.    make(N,M).ld(0);
                    Phi.   make(N,M).ld(0);
                    W.     make(N,N).ld(0);
                    K.     make(N,0);
                    Gamma. make(N,0);
                    // build Nu
                    size_t k=1;
                    for(iterator i=begin();i!=end();++i,++k)
                    {
                        Equilibrium::Pointer &p = *i;
                        rxn.push_back(p);
                        array<int> &nu = Nu[k];
                        for( const Equilibrium::Component *c=p->reactants().head;c;c=c->next)
                        {
                            const size_t j = c->sp->indx;
                            nu[j] = c->nu;
                            active[j] = true;
                        }
                        for( const Equilibrium::Component *c=p->products().head;c;c=c->next)
                        {
                            const size_t j = c->sp->indx;
                            nu[j] = c->nu;
                            active[j] = true;
                        }
                    }
                    matrix<int> Nu2(N,N);
                    tao::mmul_rtrn(Nu2, Nu, Nu);
                    const int   d = ideterminant(Nu2);
                    if(0==d)
                    {
                        throw exception("Equilibria: found dependency!");
                    }
                }


            }
            catch(...)
            {
                release_all();
                throw;
            }
        }

        void Equilibria:: computeK(const double t)
        {
            for(size_t i=N;i>0;--i)
            {
                K[i] = (*rxn[i])(t);
            }
        }

        void Equilibria:: computeGamma(const array<double> &C)
        {
            for(size_t i=N;i>0;--i)
            {
                Gamma[i] = rxn[i]->Gamma(K[i],C);
            }
        }

        void Equilibria:: computeGammaAndPhi(const array<double> &C)
        {
            Phi.ld(0);
            for(size_t i=N;i>0;--i)
            {
                Gamma[i] = rxn[i]->GammaAndPhi(Phi[i],K[i],C);
            }
        }

        size_t Equilibria:: computeExcess(const array<double> &C)
        {
            size_t nxs = 0;
            for(size_t i=M;i>0;--i)
            {
                if(C[i]<0)
                {
                    ++nxs;
                    excess[i] = -C[i];
                }
            }
            return nxs;
        }

        bool Equilibria:: computeW()
        {
            tao::_mmul_rtrn(W,Phi,Nu);
            return true;
        }

    }

}
