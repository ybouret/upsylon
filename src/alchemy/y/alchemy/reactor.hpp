
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
            typedef matrix<double>                Matrix;       //!< alias
            typedef matrix<long>                  iMatrix;      //!< alias
            typedef accessible<double>            Accessible;   //!< alias
            typedef addressable<double>           Addressable;  //!< alias

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

            //! compute initial state, return regularity
            bool isRegular(const Accessible &C, const double t) throw();

            //! compute regularity for pre-computed K
            bool isRegular(const Accessible &C)                 throw();

            //! display state, used for debug
            void display_state() const;


            //! solve
            bool   solve(Addressable &C, const double t) throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Library    &lib;   //!< support library
            const Equilibria &eqs;   //!< support equlibria
            const size_t      N;     //!< number of equilibria
            const size_t      M;     //!< number of components
            const Vector      K;     //!< [N]   constants
            const Vector      Gam;   //!< [N]   indicators
            const Vector      xi;    //!< [N]   extents
            const Vector      dC;    //!< [M]   delta C
            const iMatrix     Nu;    //!< [NxM] topology matrix
            const iMatrix     NuT;   //!< [MxN] transposed Nu
            const Matrix      Phi;   //!< [NxM] jacobian
            const Matrix      J;     //!< [NxN] PhiNuT
            const Matrix      W;     //!< [NxN] LU::build(J)
            const Flags       swept; //!< [N] for sweep
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
            const Freezer lfrz;
            const Freezer efrz;

            bool  checkRegular() const  throw(); //!< compute J and W
            bool  sweep(Addressable &C) throw(); //!< try to solve a not swept equilibrium

        };

    }

}

#endif
