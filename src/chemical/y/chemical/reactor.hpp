//! \file

#ifndef Y_CHEMICAL_REACTOR_INCLUDED
#define Y_CHEMICAL_REACTOR_INCLUDED 1

#include "y/chemical/equilibria.hpp"
#include "y/chemical/leading.hpp"
#include "y/chemical/seeking.hpp"

namespace upsylon
{
    
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! managing equilibria
        //
        //______________________________________________________________________
        class Reactor : public Object
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char CLID[]; //!< "Chemical::Reactor"

            enum State
            {
                Jam,
                Nil,
                All,
                Cut
            };

            static const char * StateText(const State) throw();

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            
            //! cleanup
            virtual ~Reactor() throw();
            
            //! setup
            explicit Reactor(Library       &usrLib,
                             Equilibria    &usrEqs,
                             const unsigned flags);


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! balance leading conditions
            bool balanceLeading(Addressable &C) throw();

            //! balance seeking conditions
            bool balanceSeeking(Addressable &C) throw();

            //! full algorithm
            bool balance(Addressable &C) throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            Library             &lib;     //!< user's library
            Equilibria          &eqs;     //!< user's equilibria
            const size_t         M;       //!< lib size
            const size_t         N;       //!< eqs size
            const size_t         NW;      //!< number of working species
            const size_t         NL;      //!< number of leading species
            const size_t         NS;      //!< number of seeking species
            const iMatrix        Nu;      //!< [NxM] topology matrix
            const iMatrix        NuT;     //!< [MxN] Nu'
            const iMatrix        NuL;     //!< [NxM] leading topology matrix
            const iMatrix        NuLT;    //!< [MxN] NuLT'
            const iMatrix        NuS;     //!< [NxM] seaking topology matrix
            const iMatrix        NuST;    //!< [MxN] NuST'
            const Leading::Array leading; //!< [N]  leading conditions
            const Seeking::Array seeking; //!< [NS] seeking conditions
            Vector               xi;      //!< [N] extent
            Matrix               Vs;     //!< [NSxN] numerical
            Matrix               VsT;    //!< [NxNS] NuS'
            Matrix               IV2;    //!< [NSxNS] LU of numerical NuS * NuS'
            Vector               Cs;     //!< [Ns] excess concentrations
            Vector               Rs;     //!< [Ns] used to solve system
            Vector               xs;     //!< [N]  excess extents
            Flags                ok;     //!< [N]  active/jammed
            Indices              ix;     //!< [N]  solving order indices
            
            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________

            //! show all leading conditions
            template <typename OSTREAM> inline
            OSTREAM & showLeading(OSTREAM &os) const
            {
                os << "  <Leading>\n";
                for(size_t i=1;i<=N;++i)
                {
                    os << *leading[i];
                }
                os << "  <Leading/>\n";
                return os;
            }


            //! show all seeking conditions
            template <typename OSTREAM> inline
            OSTREAM & showSeeking(OSTREAM &os) const
            {
                os << "  <Seeking>\n";
                for(size_t i=1;i<=NS;++i)
                {
                    os << "    ";
                    os << *seeking[i] << '\n';
                }
                os << "  <Seeking/>\n";
                return os;
            }



        private:
            const Freezable::Latch lockLib;
            const Freezable::Latch lockEqs;
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);

            bool   seekingQuery(const Accessible &C) throw(); //!< create Cs, return true upon bad value
            void   seekingBuild()                    throw(); //!< initialize Vs, set ok to true
            bool   seekingRaise(const Accessible &C) throw(); //!< raise set of valid direction and compute xs
            bool   seekingExtra()                    throw(); //!< compute xs from Cs and current Vs
            size_t seekingJammd(const Accessible &C) throw(); //!< equilibria that cannot move
            void   seekingBlock(const size_t i)      throw(); //!< block Vs ans VsT
            void   seekingIndex()                    throw(); //!< compute indices in ix
            bool   seekingSolve(Addressable &C)      throw(); //!< from ok/xs/ix full step?
        };
    }
}

#endif

