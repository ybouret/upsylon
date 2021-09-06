
#include "y/chemical/system.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {

        const char System::CLID[] = "Chemical::System";

        const char System:: PrimaryEnter[] = "<Primary>";
        const char System:: PrimaryLeave[] = "<Primary/>";


        System:: ~System() throw()
        {
        }


        static inline
        size_t checkValidity(const Library &lib, const Equilibria &eqs)
        {
            const size_t M = lib->size();
            if(M<eqs->size()) throw exception("%s has too many equilibria",System::CLID);
            return M;
        }

        System:: System(const Library    &usrLib,
                        const Equilibria &usrEqs,
                        const unsigned    flags ) :
        lib(usrLib),
        eqs(usrEqs),
        N(eqs->size()),
        M( checkValidity(lib,eqs) ),
        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),
        primary(N,as_capacity),
        NP(0),
        libLatch( aliasing::_(lib) ),
        eqsLatch( aliasing::_(eqs) )
        {

            Y_CHEMICAL_PRINTLN("<System>");
            Y_CHEMICAL_PRINTLN("  N   = " << N);
            Y_CHEMICAL_PRINTLN("  M   = " << M);

            if(N>0)
            {
                // checking equilibria
                eqs.verify(flags);
                eqs.fill( aliasing::_(Nu) );
                aliasing::_(NuT).assign_transpose(Nu);
                Y_CHEMICAL_PRINTLN("  Nu  = " << Nu);
                Y_CHEMICAL_PRINTLN("  NuT = " << NuT);

                {
                    Matrix Nu2(N,N);
                    tao::gram(Nu2,Nu);
                    if(!LU::build(Nu2))
                    {
                        throw exception("%s has singular equilibria",CLID);
                    }
                }

                // building primary
                Y_CHEMICAL_PRINTLN("  " << PrimaryEnter);;
                for(const ENode *node=eqs->head();node;node=node->next)
                {
                    const Primary::Pointer p = new Primary(***node);
                    aliasing::_(primary).push_back_(p);
                    p->display(std::cerr,4);
                    aliasing::_(NP) += p->count();
                }
                Y_CHEMICAL_PRINTLN("  " << PrimaryLeave);;


                Y_CHEMICAL_PRINTLN("  NP  = " << NP);

            }

            Y_CHEMICAL_PRINTLN("<System/>");

        }


    }

}
