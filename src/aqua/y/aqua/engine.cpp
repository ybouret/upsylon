
#include "y/aqua/engine.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/block/zset.hpp"
#include "y/exception.hpp"
#include "y/mkl/kernel/quark.hpp"
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
        det(),
        iNu(),
        Prj(),
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
        aN(8),
        xi( aN.next() ),
        K( aN.next() ),
        Q( aN.next() ),
        keep(),
        maxNameLength(0),
        balanceVerbose(false),
        balanceCycles(0)
        {
            keep << aliasing::_(equilibria);
            keep << aliasing::_(Nu);
            keep << aliasing::_(tNu);
            keep << aliasing::_(Nu2);
            keep << aliasing::_(iNu);
            keep << aliasing::_(Prj);
            keep << aliasing::_(J);
            keep << aliasing::_(W);
            keep << aliasing::_(aM);
            keep << aliasing::_(aN);
        }

        void Engine::quit() throw()
        {
            keep.release_all();
            _bzset(det);
            _bzset(Nc);
            _bzset(Ma);
            _bzset(M);
            _bzset(N);
            _bzset(maxNameLength);
            _bzset(balanceCycles);
            new ( & aliasing::_(active) ) Booleans();
            new ( &illegal )              Booleans();
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
                    new ( & aliasing::_(active) ) Booleans(aliasing::as<bool>(*Cact),M);
                    new ( &illegal)               Booleans(aliasing::as<bool>(*Cill),M);
                }

                if(N>0)
                {
                    aliasing::_(equilibria).ensure(N);
                    aliasing::_(Nu).  make(N,M);
                    aliasing::_(tNu). make(M,N);
                    aliasing::_(Nu2). make(N,N);
                    aliasing::_(aN).acquire(N);
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
                    for(size_t j=M;j>0;--j)
                    {
                        if(active[j]) ++aliasing::_(Ma);
                    }
                    aliasing::_(tNu).assign_transpose(Nu);
                    quark::mmul_rtrn(aliasing::_(Nu2),Nu,Nu);
                    aliasing::_(det) = ideterminant(Nu2);
                    if(det<=0) throw exception("%ssingular set of equilibria",fn);
                    aliasing::_(iNu).make(N,M);
                    aliasing::_(Prj).make(M,M);
                    {
                        iMatrix aNu2(N,N); iadjoint(aNu2,Nu2);
                        quark::mmul( aliasing::_(iNu), aNu2,Nu);
                        quark::mmul( aliasing::_(Prj), tNu, iNu);
                    }

                    J.make(N,M);
                    W.make(N,N);
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
