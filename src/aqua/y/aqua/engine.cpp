
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
        Prj(),
        aM( 16 ),
        Corg( aM.next() ),
        Cbad( aM.next() ),
        Ctry( aM.next() ),
        Cstp( aM.next() ),
        Caux( aM.next() ),
        Cact( aM.next() ),
        Cill( aM.next() ),
        active(),
        illegal(),
        aN(8),
        xi( aN.next() ),
        nu2( aN.next() ),
        keep(),
        balanceVerbose(false)
        {
            keep << aliasing::_(equilibria);
            keep << aliasing::_(Nu);
            keep << aliasing::_(tNu);
            keep << aliasing::_(Nu2);
            keep << aliasing::_(Prj);
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
                            aliasing::_(equilibria).push_back(eq);
                            addressable<Int> &nu_i = aliasing::_(Nu[i]);
                            eq->fillNu(nu_i);
                            int sq = 0;
                            for(size_t j=M;j>0;--j)
                            {
                                const Int nu_ij = nu_i[j];
                                if( nu_ij !=0 )
                                {
                                    aliasing::_(active[j]) = true;
                                    sq += nu_ij * nu_ij;
                                }
                            }
                            nu2[i] = sq;
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
                    aliasing::_(Prj).make(M,M);
                    {
                        iMatrix aNu2(N,N);
                        iadjoint(aNu2,Nu2);
                        iMatrix aNu2Nu(N,M);
                        quark::mmul(aNu2Nu,aNu2,Nu);
                        quark::mmul( aliasing::_(Prj), tNu, aNu2Nu);
                    }
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
