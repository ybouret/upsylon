#include "y/aqua/engine.hpp"
#include "y/exception.hpp"
#include "y/mkl/kernel/apk.hpp"

namespace upsylon
{

    namespace Aqua
    {
        using namespace mkl;

        void Engine:: Project(iMatrix          &proj,
                              iAddressable     &scal,
                              const iMatrix    &span,
                              const char       *when)
        {
            assert(when);
            assert(proj.rows==proj.cols);
            assert(span.cols==proj.cols);
            const size_t m = proj.rows;
            const size_t n = span.rows;

            apz         Det = 0;
            {
                matrix<apz> Proj(m,m);
                {
                    //----------------------------------------------------------
                    // build the simplified adjoint of Gram's matrix
                    //----------------------------------------------------------
                    matrix<apz> Adj(n,n);
                    Det = apk::adjoint_gram(Adj,span);

                    std::cerr << "span = " << span << std::endl;
                    std::cerr << "iAdj  = " << Adj  << std::endl;
                    std::cerr << "iDet  = " << Det  << std::endl;

                    if(0==Det)
                    {
                        throw exception("Aqua::Engine:Project(invalid %s)",when);
                    }

                    //----------------------------------------------------------
                    // build the projection matrix
                    //----------------------------------------------------------
                    {
                        matrix<apz> AdjSpan(n,m);
                        tao::mmul(AdjSpan,Adj,span);       // AdjSpan = Adj * Span
                        tao::mmul_ltrn(Proj,span,AdjSpan); // Proj    = Span' * AdjSpan
                    }
                }

                //--------------------------------------------------------------
                // first simplification
                //--------------------------------------------------------------
                apk::simplify(Proj,Det,NULL);
                std::cerr << "Proj  = " << Proj << std::endl;
                std::cerr << "iDet  = " << Det  << std::endl;

                //--------------------------------------------------------------
                // build individual scalings
                //--------------------------------------------------------------
                addressable<apz> &Scal = Proj.c_aux1;
                apk::simplify(Scal,Proj,Det);
                std::cerr << "Scal=" << Scal << std::endl;
                std::cerr << "Proj=" << Proj << std::endl;
                apk::convert(proj,Proj, "Aqua::Engine::Project(<proj>)");
                apk::convert(scal,Scal, "Aqua::Engine::Project(<scal>)");
            }
        }
    }


}

