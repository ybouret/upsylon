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
            // setup methods
            //__________________________________________________________________
            void quit() throw();                            //!< release/clean all data
            void init(Library &lib, const Equilibria &eqs); //!< setup

            //__________________________________________________________________
            //
            //! balance set of concentrations with Nu
            //__________________________________________________________________
            bool balance(addressable<double> &C) throw();

            //__________________________________________________________________
            //
            // methods to forward to equilibrium
            //__________________________________________________________________
            void computeK(const double t); //!< compute all constants at a given time
            void updateKs() throw();       //!< compute scaling...
            void computeQ(const accessible<double> &C) throw(); //!< compute Q only
            void computeJ(const accessible<double> &C) throw(); //!< compute Q and J

            //! forward, need precomputed K
            bool forward(addressable<double> &C) throw();

            //! sweep equilibria[i], need precomputed K
            bool sweep(addressable<double> &C, const size_t i) throw();

            //! sweep possible equilibria, need K
            bool sweep(addressable<double> &C) throw();

            //! damp dC for precomputed K and Q(C)=0
            bool damp(addressable<double> &dC, const accessible<double> &C) throw();

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
            const iMatrix  iNu;        //!< adj(Nu2)*Nu : C->xi  [NxM]
            const iMatrix  Prj;        //!< Nu'*adj(Nu2)*Nu      [MxM]
            Matrix         J;          //!< Jacobian             [NxM]
            Matrix         W;          //!< system matrix        [NxN]
            
        private:
            Arrays         aM;         //!< linear memory
        public:
            Array         &Corg;       //!< hold   C [M]
            Array         &Cxs;        //!< excess C [M]
            Array         &Ctry;       //!< trial  C [M]
            Array         &Cstp;       //!< step   C [M]
            Array         &Caux;       //!< aux    C [M]
        private:
            Array         &Cact;      //!< for active  [M]
            Array         &Cill;      //!< for illegal [M]
        public:
            const Booleans active;    //!< active concentrations [M]
            Booleans       illegal;   //!< illegal concentrations [M]

            Array         &Cini;      //!< initial concentration
            Array         &Cend;      //!< end concentration
            Array         &step;      //!< Newton's step
            Array         &Cswp;      //!< for sweeping
            Array         &Cdmp;      //!< for damping/updating
            
        private:
            Arrays         aN;        //!< linear memory
        public:
            Array         &xi;        //!< extent    [N]
            Array         &K;         //!< constants [N]
            Array         &Q;         //!< indicator [N]
            Array         &Ks;        //!< scaling   [N]
            
         private:
            Collector keep;           //!< memory management
            size_t    maxNameLength;  //!< max equilibria name lengths

            Y_DISABLE_COPY_AND_ASSIGN(Engine);

            struct BalanceProxy { Engine *self; double operator()(const double) throw(); };
            double BalanceValue() throw();               //! at Corg
            bool   BalanceDelta() throw();               //! at Corg/Cbad => Cstp, false if Cstp=0
            double BalanceCheck(const double x) throw(); //!< Ctry=Corg+x*Ctry
            double sumCaux(const size_t m)      throw();

            double QValue() throw();               //!< current |Q|^2
            double QCheck(const double x) throw(); //!< |Q|^2 at Ctry=Cini+x*step
            struct QProxy { Engine *self; double operator()(const double) throw(); };

        public:
            bool   balanceVerbose; //!< balance verbosity
            size_t balanceCycles;  //!< last balance cycles
            bool   forwardVerbose; //!< forward verbosity
            size_t forwardCycles;  //!< last forward cycles
            size_t totalBalances;  //!< during a forward
        };
    }
}

#endif