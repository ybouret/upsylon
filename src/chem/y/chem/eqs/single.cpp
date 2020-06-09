#include "y/chem/equilibria.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/mkl/fcn/zfind.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {
        namespace
        {
            struct __single_solver
            {
                const Equilibrium        *pEq;
                double                    K;
                const accessible<double> *pCini;
                const accessible<int>    *pNu;
                addressable<double>      *pCtry;
                
                double operator()(double xi)
                {
                    assert(pEq);
                    assert(K>0);
                    assert(pCini);
                    assert(pNu);
                    assert(pCtry);
                    addressable<double> &C = *pCtry;
                    quark::muladd(C, *pCini, xi, *pNu );
                    return pEq->Gamma(K,C);
                }
            };
        }

        bool Equilibria::solveSingle(const size_t iEq, array<double> &C)
        {
            //__________________________________________________________________
            //
            // initialize data and function to be zeroed
            //__________________________________________________________________
            for(size_t i=M;i>0;--i)
            {
                assert( !(active[i]&&C[i]<0) );
                Cini[i] = C[i];
            }
            Equilibrium           &eq = *eqs[iEq];
            const accessible<int> &nu =  Nu[iEq];
            __single_solver   F  = { &eq, K[iEq], &Cini, &nu, &Ctry };

            double F0 = F(0);
            //std::cerr << "|_Gamma[" << eq.name << "]=" << F0 << std::endl;

            triplet<double> xx = {  0, 0, 0 };
            triplet<double> FF = { F0,F0,F0 };
            if(F0>0)
            {
                //______________________________________________________________
                //
                //
                // Forward
                //
                //______________________________________________________________
                //std::cerr << "|_forward" << std::endl;
                if(eq.reactants().size>0)
                {
                    //std::cerr << " |_has limit" << std::endl;
                    FF.c = F( (xx.c = eq.computeMaximumForward(C)) );
                }
                else
                {
                    //std::cerr << " |_NO  limit" << std::endl;
                    xx.c = 1;
                    while( (FF.c=F(xx.c)) > 0 )
                    {
                        xx.c += 1;
                    }
                }
                goto FIND;
            }
            else if(F0<0)
            {
                //______________________________________________________________
                //
                //
                // Reverse
                //
                //______________________________________________________________
                //std::cerr << "|_reverse" << std::endl;
                if(eq.products().size>0)
                {
                    //std::cerr << " |_has limit" << std::endl;
                    FF.c = F( (xx.c = -eq.computeMaximumReverse(C)) );
                }
                else
                {
                    //std::cerr << " |_NO  limit" << std::endl;
                    xx.c = -1;
                    while( (FF.c=F(xx.c)) < 0 )
                    {
                        xx.c -= 1;
                    }
                }
                goto FIND;
            }
            else
            {
                // ok, already solved!
                //std::cerr << "|_already solved" << std::endl;
                return true;
            }
            assert( die("never get here") );
        FIND:
            //std::cerr << "xx=" << xx << ", FF=" << FF << std::endl;
            if( zfind::run(F,xx,FF) )
            {
                //std::cerr << "|_success" << std::endl;
                F(xx.b);
                for(size_t j=M;j>0;--j)
                {
                    if(active[j])
                    {
                        C[j] = max_of<double>(Ctry[j],0);
                    }
                    else
                    {
                        assert( fabs(C[j]-Ctry[j])<=0 ); // left untouched...
                    }
                }
                return true;
            }
            else
            {
                //std::cerr << "|_failure" << std::endl;
                return false;
            }
        }

        bool   Equilibria:: tryShift( array<double> &C )
        {
            for(size_t i=1;i<=N;++i)
            {
                if( solveSingle(i,C) )
                {
                    return true;
                }
            }
            return false;
        }

    }
}
