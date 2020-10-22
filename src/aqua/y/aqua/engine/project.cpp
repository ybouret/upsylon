#include "y/aqua/engine.hpp"
#include "y/exception.hpp"
#include "y/mkl/kernel/apk.hpp"

namespace upsylon
{

    namespace Aqua
    {
        using namespace mkl;

        Int Engine:: Project(iMatrix       &Proj,
                             const iMatrix &Span,
                             const char    *when)
        {
            assert(when);
            assert(Proj.rows==Proj.cols);
            assert(Span.cols==Proj.cols);
            const size_t m = Proj.rows;
            const size_t n = Span.rows;

            apz         Det = 0;
            {
                matrix<apz> P(m,m);
                {
                    // build the simplify adjoint of Gram's matrix
                    matrix<apz> Adj(n,n);
                    Det = apk::adjoint_gram(Adj,Span);

                    std::cerr << "Span = " << Span << std::endl;
                    std::cerr << "Adj  = " << Adj  << std::endl;
                    std::cerr << "Det  = " << Det  << std::endl;

                    if(0==Det)
                    {
                        throw exception("Aqua::Engine: invalid %s",when);
                    }

                    {
                        matrix<apz> AdjSpan(n,m);
                        tao::mmul(AdjSpan,Adj,Span);
                        tao::mmul_ltrn(P,Span,AdjSpan);
                    }
                }
                std::cerr << "P   = " << P   << std::endl;
                std::cerr << "D   = " << Det << std::endl;
                apk::simplify(P,Det,NULL);
                std::cerr << "P1  = " << P   << std::endl;
                std::cerr << "D1  = " << Det << std::endl;
                apk::convert(Proj,P, "Aqua::Engine::Project(matrix)");
            }
            return Det.cast_to<Int>( "Aqua::Engine::Project(determinant)" );
        }
    }


}

