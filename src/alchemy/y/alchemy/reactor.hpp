
//! \file

#ifndef Y_ALCHEMY_REACTOR_INCLUDED
#define Y_ALCHEMY_REACTOR_INCLUDED 1

#include "y/alchemy/library.hpp"
#include "y/alchemy/equilibria.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/dyadic.hpp"

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
            static  const char                    CLID[];       //!< Reactor
            typedef memory::dyadic                Allocator;    //!< alias
            typedef vector<double,Allocator>      Vector;       //!< alias
            typedef vector<bool,Allocator>        Flags;        //!< alias
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            explicit Reactor(Library    &,
                             Equilibria &);

            //! cleanup
            virtual ~Reactor() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void displayState()               const; //!< info to debug
            bool isValid(const Accessible &C) const; //!< check active are >0, display error

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Library    &lib;    //!< support library
            const Equilibria &eqs;    //!< support equlibria
            const size_t      N;      //!< number of equilibria
            const size_t      M;      //!< number of components
            const size_t      dof;    //!< number of active species
            const Flags       active; //!< [M]   active flags
            const Vector      K;      //!< [N]   constants
            const Vector      Gam;    //!< [N]   indicators
            const Vector      xi;     //!< [N]   extents
            const Vector      dC;     //!< [M]   delta C
            const Vector      Ctry;   //!< [M]   trial C
            const iMatrix     Nu;     //!< [NxM] topology matrix
            const iMatrix     NuT;    //!< [MxN] transposed Nu
            const Matrix      Phi;    //!< [NxM] jacobian
            const Matrix      J;      //!< [NxN] PhiNuT
            const Matrix      W;      //!< [NxN] LU::build(J)
            const Flags       moved;  //!< [N] for sweep
            const Flags       isBad;  //!< [M] for balance

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
            const Freezer lfrz;
            const Freezer efrz;

            

        };

    }

}

#endif
