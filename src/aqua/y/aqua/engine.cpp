
#include "y/aqua/engine.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/block/zset.hpp"
#include "y/exception.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/adjoint.hpp"

namespace upsylon
{
    namespace Aqua
    {
        using namespace mkl;

        Engine:: ~Engine() throw()
        {
        }

        Engine:: Engine():
        N(0),
        M(0),
        Ma(0),
        Nc(0),
        equilibria(),
        Nu(),
        tNu(),
        Nu2(),
        dNu(0),
        pNu(),
        J(),
        W(),
        aM( 16 ),
        Corg( aM.next() ),
        Cxs( aM.next() ),
        Ctry( aM.next() ),
        Cstp( aM.next() ),
        Caux( aM.next() ),
        Cact( aM.next() ),
        Cill( aM.next() ),
        active(),
        illegal(),
        Cini( aM.next() ),
        Cend( aM.next() ),
        step( aM.next() ),
        Cswp( aM.next() ),
        Cdmp( aM.next() ),
        aN(8),
        xi( aN.next() ),
        K(  aN.next() ),
        Q(  aN.next() ),
        Ks( aN.next() ),
        keep(),
        maxNameLength(0),
        balanceVerbose(false),
        balanceCycles(0),
        forwardVerbose(false),
        forwardCycles(0),
        totalBalances(0)
        {
            keep << aliasing::_(equilibria);
            keep << aliasing::_(Nu);
            keep << aliasing::_(tNu);
            keep << aliasing::_(Nu2);
            keep << aliasing::_(pNu);
            keep << aliasing::_(J);
            keep << aliasing::_(W);
            keep << aliasing::_(aM);
            keep << aliasing::_(aN);
        }

        void Engine::quit() throw()
        {
            keep.release_all();
            _bzset(dNu);
            _bzset(Nc);
            _bzset(Ma);
            _bzset(M);
            _bzset(N);
            _bzset(maxNameLength);
            _bzset(balanceCycles);
            new ( & aliasing::_(active)  ) Booleans();
            new ( & aliasing::_(illegal) ) Booleans();
        }

       

        void Engine:: init(Library &lib, const Equilibria &eqs)
        {
            static const char fn[] = "Aqua::Engine::init: ";
            quit();
            try
            {
                lib.buildIndices();
                aliasing::_(M) = lib.entries();
                aliasing::_(N) = eqs.entries();
                if(N>M) throw exception("%stoo many equilibria",fn);
                aliasing::_(Nc) = M-N;

                if(M>0)
                {
                    aM.acquire(M);
                    new ( & aliasing::_(active) )  Booleans(aliasing::as<bool>(*Cact),M);
                    new ( & aliasing::_(illegal) ) Booleans(aliasing::as<bool>(*Cill),M);
                    tao::ld( aliasing::_(active), false);
                }

                if(N>0)
                {
                    aliasing::_(equilibria).ensure(N);
                    aliasing::_(Nu).  make(N,M);
                    aliasing::_(tNu). make(M,N);
                    aliasing::_(Nu2). make(N,N);
                    aliasing::_(pNu). make(M,M);
                    aliasing::_(aN).  acquire(N);
                    J.make(N,M);
                    W.make(N,N);

                    // load from equilibria
                    {
                        size_t i=1;
                        for(Equilibria::const_iterator it=eqs.begin();it!=eqs.end();++it,++i)
                        {
                            const Equilibrium::Pointer &eq  = *it;
                            maxNameLength = max_of(maxNameLength,eq->name.size());
                            aliasing::_(equilibria).push_back(eq);
                            addressable<Int> &nu_i = aliasing::_(Nu[i]);
                            eq->fillNu(nu_i);
                        }
                    }

                    // check all active species
                    for(size_t i=N;i>0;--i)
                    {
                        const array<Int> &Nu_i = Nu[i];
                        for(size_t j=M;j>0;--j)
                        {
                            if(Nu_i[j]!=0) aliasing::_(active[j]) = true;
                        }
                    }

                    for(size_t j=M;j>0;--j)
                    {
                        if(active[j]) ++aliasing::_(Ma);
                    }

                    //! compute tNu and pNu
                    aliasing::_(tNu).assign_transpose(Nu);
                    aliasing::_(dNu) = Project( aliasing::_(pNu), Nu, tNu, "chemical topology");
                    

                }



            }
            catch(...)
            {
                quit();
                throw;
            }
        }


     

    }

}
