#include "y/oxide/field/split.hpp"

#include "y/mpl/rational.hpp"

namespace upsylon
{
    namespace Oxide
    {
        mpq __Split:: Cost(const Coord1D *P, const Coord1D *M, const size_t D )
        {
            assert(P);
            assert(M);
            assert(D>=1);
            assert(D<=3);

            unit_t  num = 0;
            size_t  den = 0;
            for(size_t d=0;d<D;++d)
            {
                const unit_t p = P[d];
                const size_t m = M[d]; assert(m>0);
                num += p*m;
                den += m*m;
            }
            const mpq alpha(num,den);

            mpq cost = 0;
            for(size_t d=0;d<D;++d)
            {
                const mpq p = unit_t(P[d]);
                const mpq m = size_t(M[d]);
                const mpq delta = p - alpha * m;
                cost += delta*delta;
            }
            return cost;
        }

        size_t __Split:: Find(const array<mpq> &costs)
        {
            const size_t n = costs.size(); assert(n>0);
            size_t       min_indx = 1;
            mpq          min_cost = costs[1];

            for(size_t i=2;i<=n;++i)
            {
                const mpq &tmp_cost = costs[i];
                if(tmp_cost<min_cost)
                {
                    min_cost = tmp_cost;
                    min_indx = i;
                }
            }

            return min_indx;
        }


    }

}


