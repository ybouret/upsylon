#include "y/alchemy/reactor.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/exception.hpp"


namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {
        void Reactor:: makeBalancing()
        {
            vector<Species::Pointer> seeking;

            //__________________________________________________________________
            //
            // Build balancing info:
            // outer loop over species
            //__________________________________________________________________
            std::cerr << "  <Balancing>" << std::endl;
            for(const Species::Node *snode = lib->head(); snode; snode=snode->next)
            {
                const Species &sp = ***snode;
                const size_t   si = sp.indx;

                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________
                const accessible<unit_t> &v   = NuT[si];
                addressable<unit_t>      &l   = aliasing::_(NuLT[si]);
                addressable<unit_t>      &s   = aliasing::_(NuST[si]);
                size_t                    nok = 0;
                const Equilibrium        *pEq = 0;
                unit_t                    snu = 0;

                //______________________________________________________________
                //
                // inner loop over equilibria
                //______________________________________________________________
                for(const Equilibrium::Node *enode=eqs->head();enode;enode=enode->next)
                {
                    const Equilibrium &eq  = ***enode;
                    const size_t       ei  = eq.indx;
                    const unit_t       cof = v[ei];
                    if(0!=cof)
                    {
                        ++nok;
                        pEq = &eq;
                        snu = cof;
                    }
                }

                if(nok!=sp.rating) throw exception("%s invalid rating for '%s'",CLID,*(sp.name));

                if(nok>0)
                {
                    assert(true==active[sp.indx]);
                    assert(snu!=0);
                    assert(pEq!=NULL);

                    if(1==nok)
                    {
                        // leading condition
                        Guard &guard = aliasing::_(*guards[pEq->indx]);
                        if(snu>0)
                        {
                            //__________________________________________________
                            //
                            // Create a new GEQ
                            //__________________________________________________
                            const Leading leading(*pEq,sp, static_cast<size_t>(snu), Leading::GEQ);
                            leading.print(std::cerr << "   | ",lib,eqs) << std::endl;
                            guard.addGEQ(leading);
                        }
                        else
                        {
                            //__________________________________________________
                            //
                            // Create a new LEQ
                            //__________________________________________________
                            const Leading leading(*pEq,sp, static_cast<size_t>(-snu), Leading::LEQ);
                            leading.print(std::cerr << "   | ",lib,eqs) << std::endl;
                            guard.addLEQ(leading);
                        }
                        aliasing::incr(NL);
                        tao::set(l,v);
                    }
                    else
                    {
                        // subs condidition
                        aliasing::incr(NS);
                        tao::set(s,v);
                        seeking.push_back(**snode);
                    }

                }
                else
                {
                    assert(0==nok);
                    assert(false==active[sp.indx]);
                    tao::set(s,v);
                }

            }

            assert(NS==seeking.size());

            aliasing::_(NuL).assign_transpose(NuLT);
            aliasing::_(NuS).assign_transpose(NuST);


            if(NS>0)
            {
                aliasing::_(Vs).make(NS,N);
                for(size_t i=1;i<=seeking.size();++i)
                {
                    tao::set( aliasing::_(Vs[i]),NuT[seeking[i]->indx]);
                }
            }

            std::cerr << "    NuLT  = " << NuLT << std::endl;
            std::cerr << "    NuL   = " << NuL  << std::endl;
            std::cerr << "    NuST  = " << NuST << std::endl;
            std::cerr << "    NuS   = " << NuS  << std::endl;
            std::cerr << "    NEqs  = " << N    << std::endl;
            std::cerr << "    NActv = " << NA   << std::endl;
            std::cerr << "      NLead = " << NL   << std::endl;
            std::cerr << "      NSeek = " << NS   << std::endl;
            std::cerr << "      Srank = " << apk::rank(NuS) << std::endl;
            std::cerr << "      Seek  = " << seeking << std::endl;
            std::cerr << "      Vs    = " << Vs      << std::endl;
            assert(NL+NS==NA);
            std::cerr << "  <Balancing/>" << std::endl;
        }
    }

}


