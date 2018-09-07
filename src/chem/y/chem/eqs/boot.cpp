#include "y/chem/equilibria.hpp"
#include "y/exception.hpp"
#include "y/math/kernel/tao.hpp"
#include "y/math/kernel/adjoint.hpp"
#include "y/math/utils.hpp"

namespace upsylon
{
    using namespace math;

    namespace Chemical
    {
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
                throw exception("unhandled special case");
            }

            matrix<int> P2(Nc,Nc);
            tao::_mmul_rtrn(P2,P,P);
            std::cerr << "P2=" << P2 << std::endl;

            const double detP2 = ideterminant(P2);
            std::cerr << "detP2=" << detP2 << std::endl;
            if(!detP2)
            {
                throw exception("singular set of constraints");
            }
            matrix<int> adjP2(Nc,Nc);
            iadjoint(adjP2,P2);
            std::cerr << "adjP2=" << adjP2 << std::endl;
            matrix<int> U2C(M,Nc);
            {
                matrix<int> tP(P,matrix_transpose);
                tao::_mmul(U2C,tP,adjP2);
            }
            std::cerr << "U2C=" << U2C << std::endl;


            return false;
        }

    }

}

