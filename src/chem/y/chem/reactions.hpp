//! \file
#ifndef Y_CHEM_REACTIONS_INCLUDED
#define Y_CHEM_REACTIONS_INCLUDED 1

#include "y/chem/reaction.hpp"
#include "y/chem/library.hpp"
#include "y/container/matrix.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace Chemical
    {
        typedef set<string,Reaction::Pointer> ReactionsType; //!< alias for reactions databse

        //! set of reactions
        class Reactions : public ReactionsType
        {
        public:
            //! destructor
            virtual ~Reactions() throw();

            //! initialize
            explicit Reactions(const size_t n=8);

            //!check and insert, deleted on error
            Reaction & enroll( Reaction *rxn );

            //! fast const reaction enroll
            inline Reaction & operator()( const string &name, const double K )
            {
                return enroll( new ConstReaction(name,K) );
            }

            //! fast const reaction enroll
            inline Reaction & operator()( const char *name, const double K )
            {
                return enroll( new ConstReaction(name,K) );
            }

            //! for output
            size_t max_name_length() const throw();

            //! output
            friend std::ostream & operator<<( std::ostream &os, const Reactions &cs );

            const size_t N; //!< number of reactions
            const size_t M; //!< number of components

            void compile_for( Library &lib );
            void release_all() throw();


            vector<bool> active;

            // reaction dependent only
            vector<Reaction::Pointer> rxn;
            matrix<int>               Nu;    //!< topology [NxM]
            matrix<double>            Phi;   //!< Jacobian [NxN]
            vector<double>            K;     //!< constants [N]
            vector<double>            Gamma; //!< Gamma values

            void computeK(const double t);
            void computeGamma(const array<double> &C);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactions);
        };
    }
}

#endif

