
//! \file

#ifndef Y_ALCHEMY_ACTOR_INCLUDED
#define Y_ALCHEMY_ACTOR_INCLUDED 1

#include "y/alchemy/species.hpp"
#include "y/type/authority.hpp"

namespace upsylon
{
    namespace alchemy
    {

        class library;

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


            const string & key() const throw();  //!< species key

            //! display with width for coefficient
            std::ostream & display(std::ostream  &os,
                                   const library &lib) const;


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unsigned long nu; //!< coefficient
            
        private:
            Y_DISABLE_ASSIGN(actor);
        };

    }
}

#endif
