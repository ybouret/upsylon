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

            //__________________________________________________________________
            //
            // Build balancing info:
            // outer loop over species
            //__________________________________________________________________
            std::cerr << "  <Balancing>" << std::endl;

            if(NA>0)
            {
                for(const Species::Node *snode = lib->head(); snode; snode=snode->next)
                {
                    const Species &sp = ***snode;
                    const size_t   si = sp.indx;

                    //__________________________________________________________
                    //
                    // initialize
                    //__________________________________________________________
                    const accessible<unit_t> &v   = NuT[si];
                    addressable<unit_t>      &l   = aliasing::_(NuLT[si]);
                    addressable<unit_t>      &s   = aliasing::_(NuST[si]);
                    size_t                    nok = 0;
                    const Equilibrium        *pEq = 0;
                    unit_t                    snu = 0;

                    //__________________________________________________________
                    //
                    // inner loop over equilibria
                    //__________________________________________________________
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
                                //______________________________________________
                                //
                                // Create a new GEQ
                                //______________________________________________
                                const Leading leading(*pEq,sp, static_cast<size_t>(snu), Leading::GEQ);
                                leading.print(std::cerr << "   | ",lib,eqs) << std::endl;
                                guard.addGEQ(leading);
                            }
                            else
                            {
                                //______________________________________________
                                //
                                // Create a new LEQ
                                //______________________________________________
                                const Leading leading(*pEq,sp, static_cast<size_t>(-snu), Leading::LEQ);
                                leading.print(std::cerr << "   | ",lib,eqs) << std::endl;
                                guard.addLEQ(leading);
                            }
                            aliasing::incr(NL);
                            tao::set(l,v);
                            aliasing::_(leading).push_back_(**snode);
                        }
                        else
                        {
                            //__________________________________________________
                            //
                            // seeking condidition
                            //__________________________________________________
                            aliasing::incr(NS);
                            tao::set(s,v);
                            aliasing::_(seeking).push_back_(**snode);
                        }

                    }
                    else
                    {
                        assert(0==nok);
                        assert(false==active[sp.indx]);
                        tao::set(s,v);
                    }

                }
            }

            assert(NS==seeking.size());
            assert(NL==leading.size());
            
            aliasing::_(NuL).assign_transpose(NuLT);
            aliasing::_(NuS).assign_transpose(NuST);


            if(NS>0)
            {
                aliasing::_(Vs).make(NS,N);
                aliasing::_(Vs2).make(NS,0);
                for(size_t i=1;i<=seeking.size();++i)
                {
                    const array<unit_t> &s = NuT[seeking[i]->indx];
                    
                    tao::set( aliasing::_(Vs[i]),s);
                    aliasing::_(Vs2[i]) = tao::mod2<unit_t>::of(Vs[i]);
                }
            }

            std::cerr << "    NuLT  = " << NuLT << std::endl;
            std::cerr << "    NuL   = " << NuL  << std::endl;
            std::cerr << "    NuST  = " << NuST << std::endl;
            std::cerr << "    NuS   = " << NuS  << std::endl;
            std::cerr << "    NEqs  = " << N    << std::endl;
            std::cerr << "    NAct  = " << NA   << std::endl;
            std::cerr << "      NLead = " << NL   << " => " << leading << std::endl;
            std::cerr << "      NSeek = " << NS   << " => " << seeking << std::endl;
            std::cerr << "      Srank = " << apk::rank(NuS) << std::endl;
            std::cerr << "      Vs    = " << Vs      << std::endl;
            std::cerr << "      Vs2   = " << Vs2     << std::endl;
            assert(NL+NS==NA);
            std::cerr << "  <Balancing/>" << std::endl;
        }
    }

}


