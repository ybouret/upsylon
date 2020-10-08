#ifndef Y_AQUA_ENGINE_INCLUDED
#define Y_AQUA_ENGINE_INCLUDED 1

#include "y/aqua/equilibria.hpp"
#include "y/aqua/library.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/arrays.hpp"
#include "y/core/temporary-acquire.hpp"

namespace upsylon
{
    namespace Aqua
    {
        typedef arrays<double>                     Arrays;     //!< alias
        typedef array<double>                      Array;      //!< alias
        typedef lightweight_array<bool>            Booleans;   //!< alias
        typedef vector<const Equilibrium::Pointer> EqVector;   //!< alias
        class                                      Boot;       //!< forward

        //! solver
        class Engine
        {
        public:
            //__________________________________________________________________
            //
            // types and defintiions
            //__________________________________________________________________
            typedef core::temporary_acquire<16> Collector; //!< alias


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Engine();          //!< setup
            virtual ~Engine() throw();  //!< cleanup

            void quit() throw();
            void init(Library &lib, const Equilibria &eqs);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t   N;          //!< equilibria
            const size_t   M;          //!< species
            const size_t   Ma;         //!< active species
            const size_t   Nc;         //!< parameters = M-N
            const EqVector equilibria; //!< validated equilibria [N]
            const iMatrix  Nu;         //!< topology             [NxM]
            const iMatrix  tNu;        //!< transposed           [MxN]
            const iMatrix  Nu2;        //!< Nu*tNu, Gram matrix  [NxN]
            const Int      det;        //!< det(Nu2), check independant equilibria
        private:
            Arrays         aM;         //!< linear memory
        public:
            Array         &Corg;       //!< hold  C [M]
            Array         &Ctry;       //!< trial C [M]
            Array         &Cstp;       //!< step  C [M]
            Array         &Caux;       //!< aux   C [M]
        private:
            Array         &Cact;      //!< for active [M]
            Array         &Cill;      //!< for illegal [M]
        public:
            const Booleans active;
            Booleans       illegal;
            size_t         illness;
            
        private:
            Arrays         aN;        //!< linear memory
        public:
            Array         &xi;        //!< extent [N]
            Array         &nu2;       //!< nu2    [N]


            bool   balance(addressable<double> &C) throw();


        private:
            Collector keep;
            Y_DISABLE_COPY_AND_ASSIGN(Engine);
            double BalanceInit(addressable<double> &C) throw(); //!< initialize step
            double BalanceCall(const double x)         throw(); //!< constraint forward
            
            struct BalanceProxy { Engine *self; double operator()(const double) throw(); };

        public:
            bool   balanceVerbose;
            size_t balanceCycles;
        };
    }
}

#endif
