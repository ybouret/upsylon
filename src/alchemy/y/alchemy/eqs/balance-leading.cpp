
#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/code/textual.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {

        bool Reactor:: balanceLeading(Addressable &C) throw()
        {
            assert(N>0);
            assert(NA>0);

            if(Verbosity)
            {
                std::cerr << "<Balance Leading>" << std::endl;
                lib.display(std::cerr << "C=", C) << std::endl;
                showConditions(std::cerr,C);
            }

            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            bool        balanced = true;
            Flags       &OK      = aliasing::_(ok); assert(M==ok.size());
            Addressable &Xi      = aliasing::_(xi); assert(N==xi.size());
            tao::ld(OK,true);

            //------------------------------------------------------------------
            //
            // apply guard
            //
            //------------------------------------------------------------------
            Y_ALCHEM_PRINTLN("  <Guard>");
            for(const Equilibrium::Node *node = eqs->head();node;node=node->next)
            {
                const Equilibrium &eq = ***node;
                if (Verbosity) eqs.print(std::cerr << "    ",eq) << std::endl;
                const Guard::State st = guards[eq.indx]->solve(C,NuT,Xi,OK);
                if (Verbosity) eqs.print(std::cerr << "    ",eq) << ' ' << Guard::StateText(st) << std::endl;
                if(Guard::IsJammed==st)
                {
                    balanced = false;
                }
                if(Verbosity&&node->next) std::cerr << std::endl;
            }
            Y_ALCHEM_PRINTLN("  <Guard/>");


            //------------------------------------------------------------------
            //
            // done
            //
            //------------------------------------------------------------------
            if(Verbosity)
            {
                std::cerr << "  ==> [" <<textual::boolean(balanced) << "] <==" << std::endl;
                lib.display(std::cerr << "balanced=",ok) << std::endl;
                lib.display(std::cerr << "Cleading=",  C) << std::endl;
                std::cerr << "<Balance Leading/>"  << std::endl;
            }
            return balanced;
        }

    }

}

