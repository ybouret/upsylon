
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
        typedef memory::dyadic                Allocator;    //!< alias
        typedef vector<double,Allocator>      Vector;       //!< alias

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
            static  const char                    CLID[];       //!< "Reactor"
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
            bool isValid(const Accessible &C) const; //!< check active C are >0, display error
            bool balance(Addressable &C)    throw(); //!< balance current concentration
            void project(Addressable &delta, const Accessible &C)  throw();
            void project(Addressable &delta, const Accessible &C, const Accessible &C0) throw();
            void complete();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Library    &lib;    //!< support library
            const Equilibria &eqs;    //!< support equlibria
            const size_t      N;      //!< number of equilibria
            const size_t      M;      //!< number of components
            const Flags       active; //!< [M]   active flags
            const size_t      NA;     //!< number of active species
            const Vector      K;      //!< [N]   constants
            const Vector      Gam;    //!< [N]   indicators
            const Vector      xi;     //!< [N]   extents
            const Vector      aux1;   //!< [N]
            const Vector      aux2;   //!< [N]
            const Vector      Caux;   //!< [M]   auxiliary C
            const Vector      dC;     //!< [M]   delta C
            const Vector      Ctry;   //!< [M]   trial C
            const iMatrix     Nu;     //!< [NxM] topology matrix
            const iMatrix     NuT;    //!< [MxN] transposed Nu
            const Flags       Nu1;    //!< [N]   single component in NuT
            const iMatrix     aNu2;   //!< [NxN] adjoint Nu*Nu'
            const long        dNu2;   //!<       determinant if Nu*Nu'
            const Matrix      Phi;    //!< [NxM] jacobian
            const Matrix      J;      //!< [NxN] PhiNuT
            const Matrix      W;      //!< [NxN] LU::build(J)
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
            const Freezer lfrz;
            const Freezer efrz;

            

        };

    }

}

#endif
