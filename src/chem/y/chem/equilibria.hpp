//! \file
#ifndef Y_CHEM_EQUILIBRIA_INCLUDED
#define Y_CHEM_EQUILIBRIA_INCLUDED 1

#include "y/chem/equilibrium.hpp"
#include "y/chem/library.hpp"
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
            vector<double> excess; //!< negative concentraton

            //__________________________________________________________________
            //
            // reaction dependent only
            //__________________________________________________________________
            vector<Equilibrium::Pointer> rxn;   //!< the equilibria linearly stored
            matrix<int>                  Nu;    //!< topology     [NxM]
            matrix<double>               Phi;   //!< Jacobian     [NxN]
            matrix<double>               W;     //!< [NxN]
            vector<double>               K;     //!< constants    [N]
            vector<double>               Gamma; //!< Gamma values [N]

            //! evaluate constants for a given time
            void computeK(const double t);
            //! compute Gamma from a set of concentrations
            void computeGamma(const array<double> &C);
            //! compute Gamma and Phi from a set of concentrations
            void computeGammaAndPhi(const array<double> &C);

            //! compute excess
            size_t computeExcess(const array<double> &C);

            bool   computeW();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
        };
    }
}

#endif

