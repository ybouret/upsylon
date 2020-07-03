#include "y/chem/equilibria.hpp"
#include "y/exception.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/mkl/kernel/determinant.hpp"
#include "y/sort/heap.hpp"

namespace upsylon
{
    using namespace mkl;

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
        beta(),
        beta2(),
        Cini(),
        Ctry(),
        dC(),
        Corg(),
        step(),
        Cnew(),
        eqs(),
        Nu(),
        tNu(),
        Phi(),
        W(),
        K(),
        Gamma(),
        nu2(),
        xi(),
        sorted()
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
            _bzset(N);
            _bzset(M);
            sorted.release();
            xi.    release();
            nu2.   release();
            Gamma. release();
            K.     release();
            W.     release();
            Phi.   release();
            tNu.   release();
            Nu.    release();
            eqs.   release();
            Cnew.  release();
            step.  release();
            Corg.  release();
            dC.    release();
            Ctry.  release();
            Cini.  release();
            beta2. release();
            beta.  release();
            active.release();
        }

        void Equilibria:: compile_for( Library &lib )
        {

            lib.update();
            for(iterator i=begin();i!=end();++i)
            {
                (*i)->compile();
            }
            
            try
            {
                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________
                release_all();
                size_t &n = aliasing::_(N);
                size_t &m = aliasing::_(M);
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
                beta.  make(M,0);
                beta2. make(M,0);
                Cini.  make(M,0);
                Ctry.  make(M,0);
                dC.    make(M,0);
                Corg.  make(M,0);
                step.  make(M,0);
                Cnew.  make(M,0);

                //______________________________________________________________
                //
                // construct data from equilibria
                //______________________________________________________________
                if(N>0)
                {
                    eqs.free(); eqs.ensure(N);
                    Nu.    make(N,M).ld(0);
                    tNu.   make(M,N).ld(0);
                    Phi.   make(N,M).ld(0);
                    W.     make(N,N).ld(0);
                    K.     make(N,0);
                    Gamma. make(N,0);
                    nu2.   make(N,0);
                    xi.    make(N,0);
                    sorted.free(); sorted.ensure(N);

                    //__________________________________________________________
                    //
                    // register equilibria, build Nu and nu2
                    //__________________________________________________________
                    size_t k=1;
                    for(iterator i=begin();i!=end();++i,++k)
                    {
                        Equilibrium::Pointer &p = *i;
                        eqs.push_back_(p);
                        sorted.push_back_(p);
                        addressable<int> &nu = Nu[k];
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
                        nu2[k] = p->sum_nu2();
                    }
                    //__________________________________________________________
                    //
                    // check linearly independant eqs
                    //__________________________________________________________
                    matrix<int> gram(N,N);
                    quark::mmul_rtrn(gram, Nu, Nu);
                    const int   g = ideterminant(gram);
                    if(0==g)
                    {
                        throw exception("Equilibria: found dependency!");
                    }
                    //__________________________________________________________
                    //
                    // prepare auxiliary data
                    //__________________________________________________________
                    tNu.assign_transpose(Nu);
                    hsort(sorted,Equilibrium::CompareDecreasingSumNu);
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
                Equilibrium &eq = *eqs[i];
                const double eqK = eq(t);
                if(eq.rescale)
                {
                    K[i] = pow(eqK,eq.kpower);
                }
                else
                {
                    K[i] = eqK;
                }
            }
        }

        void Equilibria:: computeGamma(const array<double> &C)
        {
            for(size_t i=N;i>0;--i)
            {
                Gamma[i] = eqs[i]->Gamma(K[i],C);
            }
        }

        void Equilibria:: computeGammaAndPhi(const array<double> &C)
        {
            Phi.ld(0);
            for(size_t i=N;i>0;--i)
            {
                Gamma[i] = eqs[i]->GammaAndPhi(Phi[i],K[i],C);
            }
        }

      
        bool Equilibria:: computeW()
        {
            quark::mmul_rtrn(W,Phi,Nu);
            return true;
        }

    }

}
