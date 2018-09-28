#include "y/chem/equilibria.hpp"
#include "y/exception.hpp"
#include "y/math/kernel/tao.hpp"
#include "y/math/kernel/adjoint.hpp"
#include "y/math/utils.hpp"
#include "y/sort/sorted-sum.hpp"

namespace upsylon
{
    using namespace math;

    namespace Chemical
    {

        namespace
        {

            enum BootStatus
            {
                BootInitialize, //!< first run
                BootFindingOut, //!< waiting for a decrease
                BootDecreasing  //!< ok till decreasing
            };

        }
        bool Equilibria:: boot(array<double>        &C,
                               const Boot::Loader   &loader )
        {

            //__________________________________________________________________
            //
            // checking dimensions
            //__________________________________________________________________
            const size_t Nc = loader.size();
            if( N+Nc!=M )
            {
                throw exception("#species=%u != (#equation=%u+#constraint=%u)=%u", unsigned(M), unsigned(N), unsigned(Nc), unsigned(N+Nc) );
            }



            //__________________________________________________________________
            //
            // building contraints
            //__________________________________________________________________
            matrix<int>    P(Nc,M);
            vector<double> L(Nc);
            {
                size_t ic = 0;
                for(Boot::Constraints::const_iterator k=loader.begin();k!=loader.end();++k)
                {
                    ++ic;
                    const Boot::Constraint &constraint = **k;
                    constraint.fill(P[ic]);
                    L[ic] = constraint.value;
                }
            }
            std::cerr << "P=" << P << std::endl;
            std::cerr << "L=" << L << std::endl;

            //__________________________________________________________________
            //
            // check special cases
            //__________________________________________________________________
            if(Nc==M)
            {
                const int   dP = ideterminant(P);
                if(!dP)
                {
                    throw exception("invalid set of exact constraints");
                }

                matrix<int> aP(M,M);
                iadjoint(aP,P);
                for(size_t j=M;j>0;--j)
                {
                    C[j] = tao::_dot<double>(aP[j],L)/dP;
                }

                return true;
            }

            //__________________________________________________________________
            //
            // construct projection matrix
            //__________________________________________________________________
            matrix<int> U2C(M,Nc);
            int         detP2 = 0;
            {
                matrix<int> P2(Nc,Nc);
                tao::_mmul_rtrn(P2,P,P);
                detP2 = ideterminant(P2);
                if(!detP2)
                {
                    throw exception("singular set of constraints");
                }
                matrix<int> adjP2(Nc,Nc);
                iadjoint(adjP2,P2);
                {
                    matrix<int> tP(P,matrix_transpose);
                    tao::_mmul(U2C,tP,adjP2);
                }
            }

            vector<double> C0(M);
            vector<double> C1(M);
            vector<double> U(Nc);
            vector<double> R(M);

            BootStatus status = BootInitialize;
            double     R0     = 0;
            while(true)
            {
                //______________________________________________________________
                //
                // Pojection
                //______________________________________________________________
                //std::cerr << "C0=" << C0 << std::endl;
                // U = PC-L;
                for(size_t i=Nc;i>0;--i)
                {
                    U[i] = L[i] - tao::_dot<double>(P[i],C0);
                }

                // C1=C0+(U2C*U)/detP2
                for(size_t j=M;j>0;--j)
                {
                    C1[j] = C0[j] + tao::_dot<double>(U2C[j],U)/detP2;
                }

                //______________________________________________________________
                //
                // normalize, with included balancing
                //______________________________________________________________
                if(!normalize(C1))
                {
                    std::cerr << "cannot normalize" << std::endl;
                    return false;
                }

                //std::cerr << "C1=" << C1 << std::endl;

                for(size_t j=M;j>0;--j)
                {
                    R[j]  = fabs(C0[j]-C1[j]);
                }


                const double R1 = sorted_sum(R);

                switch (status)
                {
                    case BootInitialize:
                        std::cerr << "BootInitialize @" << R1 << std::endl;
                        status = BootFindingOut;
                        break;

                    case BootFindingOut:
                        if(R1<R0)
                        {
                            status = BootDecreasing;
                            std::cerr << "BootDecreasing @ " << R0 << " -> " << R1 << std::endl;
                        }
                        break;

                    case BootDecreasing:
                        if(R1>=R0)
                        {
                            std::cerr << "BootDecreasing done" << std::endl;
                            for(size_t j=M;j>0;--j)
                            {
                                C[j] = C0[j];
                            }
                            return true;
                        }
                        break;
                }

                R0 = R1;
                tao::_set(C0,C1);
            }


        }

    }

}

