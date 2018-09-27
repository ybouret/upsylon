//! \file
#ifndef Y_CHEM_EQUILIBRIA_INCLUDED
#define Y_CHEM_EQUILIBRIA_INCLUDED 1

#include "y/chem/equilibrium.hpp"
#include "y/chem/boot.hpp"
#include "y/container/matrix.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace Chemical
    {
        typedef set<string,Equilibrium::Pointer> EquilibriaType; //!< alias for reactions databse

        //! set of reactions
        class Equilibria : public EquilibriaType
        {
        public:
            //! destructor
            virtual ~Equilibria() throw();

            //! initialize
            explicit Equilibria(const size_t n=8);

            //!check and insert, deleted on error
            Equilibrium & enroll( Equilibrium *rxn );

            //! fast const reaction enroll
            inline Equilibrium & operator()( const string &name, const double K )
            {
                return enroll( new ConstEquilibrium(name,K) );
            }

            //! fast const reaction enroll
            inline Equilibrium & operator()( const char *name, const double K )
            {
                return enroll( new ConstEquilibrium(name,K) );
            }

            //! for output
            size_t max_name_length() const throw();

            //! output
            friend std::ostream & operator<<( std::ostream &os, const Equilibria &cs );

            const size_t N; //!< number of reactions
            const size_t M; //!< number of components

            //! prepare system metrics and memory
            void compile_for( Library &lib );

            //! cleanup
            void release_all() throw();

            //__________________________________________________________________
            //
            // variables dependent
            //__________________________________________________________________

            vector<bool>   active; //!< active species flags
            vector<double> beta;   //!< negative concentraton
            vector<double> beta2;  //!< squared negative concentrations
            vector<double> Cini;   //!< for initial concentrations
            vector<double> Ctry;   //!< for trial concentrations
            vector<double> dC;     //!< some delta C
            vector<double> Corg;   //!< for normalizing
            vector<double> step;   //!< for normalizing
            vector<double> Cnew;   //!< for normalizing
            
            //__________________________________________________________________
            //
            // reaction dependent only
            //__________________________________________________________________
            vector<Equilibrium::Pointer> eqs;    //!< the equilibria linearly stored
            matrix<int>                  Nu;     //!< topology     [NxM]
            matrix<int>                  tNu;    //!< Nu'          [MxM]
            matrix<double>               Phi;    //!< Jacobian     [NxN]
            matrix<double>               W;      //!< [NxN]
            vector<double>               K;      //!< constants    [N]
            vector<double>               Gamma;  //!< Gamma values [N]
            vector<int>                  nu2;    //!< |Nu_i|^2       [N]
            vector<double>               xi;     //!< a chemical extent [N]
            vector<Equilibrium::Pointer> sorted; //!< sorted by decreasing delta_nu [N]

            //! evaluate constants for a given time
            void computeK(const double t);
            //! compute Gamma from a set of concentrations
            void computeGamma(const array<double> &C);
            //! compute Gamma and Phi from a set of concentrations
            void computeGammaAndPhi(const array<double> &C);

            //! compute excess, cleaning too small values
            double computeExcess(array<double> &C);

            //! toto
            bool   computeW();

            //! solve single, K must be computed
            bool   solveSingle(const size_t iEq, array<double> &C);

            //! try shifting a least one equilibrium, K must be computed
            bool   tryShift(array<double> &C);

            //! balance possible negative active concentrations
            bool   balance( array<double> &C );

            //! normalize, K must be computed
            bool normalize( array<double> &C );

            //! boot a solution, K must be computed
            bool boot( array<double> &C, const Boot::Loader &loader );

            //! mix some solutons, K must be computed
            bool mix( array<double> &Cout, const array<double> &CA, const double WA, const array<double> &CB, const double WB);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
        };
    }
}

#endif

