//! \file
#ifndef Y_CHEM_REACTIONS_INCLUDED
#define Y_CHEM_REACTIONS_INCLUDED 1

#include "y/chem/reaction.hpp"
#include "y/chem/library.hpp"

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
            inline Reaction & create( const string &name, const double K )
            {
                return enroll( new ConstReaction(name,K) );
            }

            //! fast const reaction enroll
            inline Reaction & create( const char *name, const double K )
            {
                return enroll( new ConstReaction(name,K) );
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactions);
        };
    }
}

#endif

