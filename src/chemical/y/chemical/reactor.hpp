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
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            
            //! cleanup
            virtual ~Reactor() throw();
            
            //! setu
            explicit Reactor(Library       &usrLib,
                             Equilibria    &usrEqs,
                             const unsigned flags);


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

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
            const Leading::Array leading; //!< [N]  leading conditions
            const Seeking::Array seeking; //!< [NS] seeking conditions
            Vector               xi;      //!< [N] extent


            //! balance leading conditions
            bool balanceLeading(Addressable &C) throw();

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
                }
                os << "  <Seeking/>\n";
                return os;
            }


        private:
            const Freezable::Latch lockLib;
            const Freezable::Latch lockEqs;
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
        };
    }
}

#endif

