
//! \file

#ifndef Y_ALCHEMY_ACTOR_INCLUDED
#define Y_ALCHEMY_ACTOR_INCLUDED 1

#include "y/alchemy/species.hpp"
#include "y/type/authority.hpp"

namespace upsylon
{
    namespace alchemy
    {

        //______________________________________________________________________
        //
        //
        //! actor for equilibrium
        //
        //______________________________________________________________________
        class actor : public authority<const species>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit actor(const species &, const unsigned) throw(); //!< setup
            virtual ~actor()    throw();                             //!< cleanup
            actor(const actor&) throw();                             //!< copy

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unsigned nu; //!< coefficient

        private:
            Y_DISABLE_ASSIGN(actor);
        };

    }
}

#endif
