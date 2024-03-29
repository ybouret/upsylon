//! \file

#ifndef Y_ALCHEMY_REACTOR_INCLUDED
#define Y_ALCHEMY_REACTOR_INCLUDED 1

#include "y/alchemy/guard.hpp"

namespace upsylon
{
    namespace Alchemy
    {

        
        //______________________________________________________________________
        //
        //
        //! chemical reactor
        //
        //______________________________________________________________________
        class Reactor
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static  const char                     CLID[];       //!< "Reactor"
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            explicit Reactor(Library    &,
                             Equilibria &,
                             const unsigned);

            //! cleanup
            virtual ~Reactor() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void   displayState()               const; //!< info to debug
            bool   isValid(const Accessible &C) const; //!< check active C are >0, display error
            bool   balance(Addressable &C)    throw(); //!< balance current concentration
            
            
            //__________________________________________________________________
            //
            // methods/display
            //__________________________________________________________________


            //! show all conditions
            template <typename OSTREAM> inline
            OSTREAM & showConditions(OSTREAM &os, const Accessible &C) const
            {
                os << "<Conditions>\n";
#if 0
                os << "  <General>\n";
                for(const Species::Node *node=lib->head();node;node=node->next)
                {
                    const Species &sp = ***node;
                    showCondition(os << "    ",sp,C);
                }
                os << "  <General/>\n";
#endif
                showLeading(os,C);
                showSeeking(os,C);
                os << "<Conditions/>\n";
                return os;
            }

            


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Library        &lib;    //!< support library
            const Equilibria     &eqs;    //!< support equlibria
            const size_t          N;      //!< number of equilibria
            const size_t          M;      //!< number of components
            const Flags           active; //!< [M]   active flags
            
            const size_t          NA;     //!< number of active species
            const Vector          K;      //!< [N]   constants
            const Vector          Gam;    //!< [N]   indicators
            const Guard::Array    guards; //!< [N]
            const Vector          xi;     //!< [N]
            const Flags           ok;     //!< [M] to check balance1 error(s)
            const iMatrix         Nu;     //!< [NxM] topology matrix
            const iMatrix         NuT;    //!< [MxN] transposed Nu
            const iMatrix         NuLT;   //!< [MxN] leading topology, transposed
            const iMatrix         NuL;    //!< [NxM] leading topology
            const iMatrix         NuST;   //!< [MxN] seeking topology, transposed
            const iMatrix         NuS;    //!< [NXM] transposed of NuST
            const size_t          NL;     //!< number of leading     species
            const size_t          NS;     //!< number of secondaries species
            const Species::Array  leading; //!< [0..M]
            const Species::Array  seeking; //!< [0..M]
            const Matrix          Phi;    //!< [NxM] jacobian
            const Matrix          J;      //!< [NxN] PhiNuT
            const Matrix          W;      //!< [NxN] LU::build(J)
            const iMatrix         Vs;     //!< [NSxN]...


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
            const   Freezer lfrz;
            const   Freezer efrz;

            void   checkTopology();
            void   makeBalancing();
            bool   balanceLeading(Addressable &C) throw();
            bool   balanceSeeking(Addressable &C);
            size_t seekingSpecies(Addressable &Cbad, const Accessible &C) const throw();

            static const char *Qualify(const double) throw();
            
            //! output condition for one species
            template <typename OSTREAM> inline
            OSTREAM & showCondition(OSTREAM &os, const Species &sp, const Accessible &C) const
            {
                static const ios::scribe &writeNu = ios::scribe::query<unit_t>();
                static const ios::scribe &writeC  = ios::scribe::query<double>();

                const size_t j = sp.indx;
                lib.print(os,sp);
                if(sp.active)
                {
                    os << " :";
                    bool first = true;
                    for(size_t i=1;i<=N;++i)
                    {
                        const unit_t nu = NuT[j][i];
                        if(nu==0) continue;
                        os << ' ';
                        if(nu<0)
                        {
                            if(nu < -1) os << writeNu.write(&nu); else os << '-';
                        }
                        else
                        {
                            assert(nu>0);
                            if(!first) os << '+';
                            if(nu>1)
                            {
                                os << writeNu.write(&nu);
                            }
                        }
                        first = false;
                        os << Leading::EXT_ << '<' << eqs(i).name << '>';

                    }
                    const double tmp = -C[j];
                    os << " >= " << writeC.write(&tmp);
                }
                else
                {
                    os << " = " << writeC.write(&C[j]);
                }
                return os << '\n';
            }

            
            template <typename OSTREAM> inline
            OSTREAM & showLeading(OSTREAM &os, const Accessible &C) const
            {
                os << "  <Leading>\n";
                for(const Equilibrium::Node *node=eqs->head();node;node=node->next)
                {
                    const Equilibrium &eq    = ***node;
                    const Guard       &guard = *guards[eq.indx];

                    eqs.print(os << "    ",eq) << ' ' << '[' << guard.classText() << ']' << '\n';
                    guard.print(os,lib,eqs,C,"    | ");
                    os << "    <" << eq.name << "/>\n";
                    if(node->next) os << '\n';
                }

                os << "  <Leading>\n";
                return os;
            }

            //! show seeking conditions
            template <typename OSTREAM> inline
            OSTREAM & showSeeking(OSTREAM &os, const Accessible &C) const
            {
                os << "  <Seeking>\n";
                for(size_t j=1;j<=NS;++j)
                {
                    const Species &sp = *seeking[j];
                    assert(sp.active);
                    assert(sp.rating>1);
                    
                    showCondition(os << "     " << Qualify(C[sp.indx]) << ' ',sp,C);
                }
                os << "  <Seeking/>\n";
                return os;
            }

            
        };

    }

}

#endif
