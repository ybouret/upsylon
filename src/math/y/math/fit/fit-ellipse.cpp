#include "y/math/fit/fit-ellipse.hpp"
#include "y/math/kernel/lu.hpp"
#include "y/math/kernel/tao.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            Ellipse:: ~Ellipse() throw()
            {

            }

            Ellipse:: Ellipse() : Conic()
            {
                C.ld(0);
                C[2][2] = -1;
                C[1][3] = C[3][1] = 2;
            }

            void Ellipse:: factorize( point2d<double> &center )
            {

                matrix<double> Q(2,2);
                const double   a = q[1];
                const double   b = q[2];
                const double   c = q[3];
                const double   d = q[4];
                const double   e = q[5];

                Q[1][1] = a;
                Q[1][2] = Q[2][1] = b/2;
                Q[2][2] = c;
                std::cerr << "Q=" << Q << std::endl;

                matrix<double> iQ(Q);
                if(!LU::build(iQ))
                {
                    throw exception("Unexpected Singular Ellipse");
                }

                vector<double> L(2,0);
                L[1] = d;
                L[2] = e;
                vector<double> J(2,0);
                tao::mulset(J,-0.5,L);
                LU::solve(iQ,J);
                center.x = J[1];
                center.y = J[2];
                std::cerr << "J=" << J << std::endl;
            }

        }
    }
}
