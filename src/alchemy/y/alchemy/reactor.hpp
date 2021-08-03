
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
            typedef vector<double,memory::dyadic> Vector;       //!< alias
            typedef matrix<double>                Matrix;       //!< alias
            typedef matrix<long>                  iMatrix;      //!< alias
            typedef accessible<double>            Accessible;   //!< alias

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
            bool initialize(const Accessible &C, const double t) throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Library    &lib; //!< support library
            const Equilibria &eqs; //!< support equlibria
            const size_t      N;   //!< number of equilibria
            const size_t      M;   //!< number of components
            const Vector      K;   //!< [N]   constants
            const Vector      Gam; //!< [N]   indicators
            const iMatrix     Nu;  //!< [NxM] topology matrix
            const iMatrix     NuT; //!< [MxN] transposed Nu
            const Matrix      Phi; //!< [NxM] jacobian
            const Matrix      W;   //!< [NxN] inv(PhiNuT)

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
            const Freezer lfrz;
            const Freezer efrz;
        };

    }

}

#endif
