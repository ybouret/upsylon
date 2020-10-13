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

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void quit() throw();                            //!< release/clean all data
            void init(Library &lib, const Equilibria &eqs); //!< setup

            //! balance set of concentrations with Nu
            bool balance(addressable<double> &C) throw();

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
            const iMatrix  Prj;        //!< Nu'*adj(Nu2)*Nu      [MxM]
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
            const Booleans active;    //!< active concentrations [M]
            Booleans       illegal;   //!< illegal concentrations [M]
            size_t         illness;   //!< number of illegal concentrations <= M
            
        private:
            Arrays         aN;        //!< linear memory
        public:
            Array         &xi;        //!< extent [N]
            Array         &nu2;       //!< nu2    [N]


         private:
            Collector keep;
            Y_DISABLE_COPY_AND_ASSIGN(Engine);
            double BalanceInit(addressable<double> &C) throw(); //!< initialize step
            double BalanceCall(const double x)         throw(); //!< constraint forward
            
            struct BalanceProxy { Engine *self; double operator()(const double) throw(); };

        public:
            bool   balanceVerbose; //!< balance verbosity
            size_t balanceCycles;  //!< last balance cycles
        };
    }
}

#endif
