//! \file

#ifndef Y_ALCHEMY_ACTORS_INCLUDED
#define Y_ALCHEMY_ACTORS_INCLUDED 1

#include "y/alchemy/actor.hpp"
#include "y/associative/hash/set.hpp"

namespace upsylon
{
    namespace alchemy
    {
        //______________________________________________________________________
        //
        //! base class for actors
        //______________________________________________________________________
        typedef hash_set<string,actor> actors_;

        //______________________________________________________________________
        //
        //
        //! collection of actors
        //
        //______________________________________________________________________
        class actors : public actors_
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char clid[]; //!< identifier

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit actors();
            virtual ~actors() throw();


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! register a new actor
            void operator()(const species &sp, const unsigned long nu);

            //! display with lib prefix()
            std::ostream & display(std::ostream  &os,
                                   const library &lib) const;
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(actors);
        };

    }

}

#endif

