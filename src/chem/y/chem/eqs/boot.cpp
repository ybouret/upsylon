#include "y/chem/equilibria.hpp"
#include "y/exception.hpp"
#include "y/math/kernel/tao.hpp"

namespace upsylon
{
    using namespace math;

    namespace Chemical
    {
        bool Equilibria:: boot(array<double>        &C,
                               const Boot::Loader   &loader )
        {

            
            const size_t Nc = loader.size();
            if( N+Nc!=M )
            {
                throw exception("#species=%u != (#equation=%u+#constraint=%u)=%u", unsigned(M), unsigned(N), unsigned(Nc), unsigned(N+Nc) );
            }

            if(Nc==M)
            {
                throw exception("unhandled special case");
            }

            matrix<double> P(Nc,M);
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
            matrix<double> P2(Nc,Nc);
            tao::mmul_rtrn(P2,P,P);
            std::cerr << "P2=" << P2 << std::endl;
            
            return false;
        }

    }

}

