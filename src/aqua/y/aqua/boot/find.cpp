
#include "y/aqua/boot.hpp"
#include "y/aqua/engine.hpp"
#include "y/exception.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/utils.hpp"

namespace upsylon
{

    namespace Aqua
    {

        using namespace mkl;

        static const char fn[] = "[ boot  ] ";

#define Y_AQUA_PRINTLN(MSG) do { if(verbose) { std::cerr << fn << MSG << std::endl; } } while(false)


        void  Boot:: find(addressable<double> &C,
                          Engine              &engine)
        {
            assert(C.size()>=engine.M);

            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            const size_t   M  = engine.M;
            const size_t   Nc = size;
            vector<double> Corg(M,0);
            vector<double> Cnew(M,0);
            vector<double> dC(M,0);
            vector<double> Lambda(Nc,0);
            vector<double> RC(Nc,0);
            cycles = 0;

            fill(Lambda);
            tao::mul(Cnew,pL,Lambda);
            tao::divall(Cnew,dL);
            Y_AQUA_PRINTLN("R      = " << R);
            Y_AQUA_PRINTLN("S      = " << S);
            Y_AQUA_PRINTLN("Lambda = " << Lambda);
            Y_AQUA_PRINTLN("Cstar  = " << Cnew  );

            //------------------------------------------------------------------
            //
            // initial balance
            //
            //------------------------------------------------------------------
            if( !engine.balance_(Cnew, pS, dS) )
            {
                throw exception("%s: no possible initial balance",*name);
            }

            //------------------------------------------------------------------
            //
            // initial equilibrium
            //
            //------------------------------------------------------------------
            Y_AQUA_PRINTLN("Cbal   = " << Cnew);
            if(!engine.forward(Cnew))
            {
                throw exception("%s: no possible initial forward",*name);
            }

            Y_AQUA_PRINTLN("Cnew   = " << Cnew);

            while(true)
            {
                //--------------------------------------------------------------
                // find projection intent
                //--------------------------------------------------------------
                ++cycles;
                Y_AQUA_PRINTLN(" <cycle " << cycles << ">" );
                tao::mul(RC,R,Cnew);
                tao::subp(RC,Lambda);
                tao::mul(dC,pL,RC);
                tao::divall(dC,dL);
                Y_AQUA_PRINTLN("dC     = " << dC);
                tao::set(Corg,Cnew);
                engine.feed(Cnew,dC);
                Y_AQUA_PRINTLN("Cnew   = " << Cnew);
                const bool converged = __find<double>::convergence(Corg,Cnew);
                if(converged)
                {
                    Y_AQUA_PRINTLN("#converged="<<converged);
                    break;
                }
            }

            tao::upload(C,Corg);



        }

    }

}

