#include "y/aqua/engine.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/adjoint.hpp"
#include "y/exception.hpp"
#include "y/mkl/simplify.hpp"

namespace upsylon
{

    namespace Aqua
    {
        using namespace mkl;

        Int Engine:: Project(iMatrix       &Proj,
                             const iMatrix &Span,
                             const iMatrix &Strn,
                             const char    *when)
        {
            assert(when);
            assert(Proj.rows==Proj.cols);
            assert(Span.cols==Proj.cols);
            const size_t m = Proj.rows;
            const size_t n = Span.rows;

            Int     Det = 0;
            iMatrix Adj(n,n);
#if 0
            {
                iMatrix Gram(n,n);
                tao::gram(Gram,Span);
                Det = ideterminant(Gram);
                if(0==Det)
                {
                    throw exception("Aqua::Engine: invalid %s",when);
                }
                iadjoint(Adj,Gram);
                (void) simplify<Int>::on(Adj,Det);
            }

            {
                iMatrix      AdjSpan(n,m);
                tao::mmul(AdjSpan,Adj,Span);
                (void) simplify<Int>::on(AdjSpan,Det);

                tao::mmul(Proj,Strn,AdjSpan);
                (void) simplify<Int>::on(Proj,Det);
            }
#endif
            return Det;
        }
    }


}

