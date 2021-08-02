
//! \file

#ifndef Y_ALCHEMY_ACTOR_INCLUDED
#define Y_ALCHEMY_ACTOR_INCLUDED 1

#include "y/alchemy/species.hpp"
#include "y/type/authority.hpp"

namespace upsylon
{
    namespace Alchemy
    {

        //______________________________________________________________________
        //
        //
        //! actor for equilibrium: species and coefficient
        //
        //______________________________________________________________________
        class Actor : public authority<const Species>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef hash_set<string,Actor> Set;  //!< alias
            typedef Set::node_type         Node; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Actor(const Species &, const unsigned long) throw(); //!< setup
            virtual ~Actor()    throw();                                  //!< cleanup
            Actor(const Actor&) throw();                                  //!< copy
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const string & key() const throw();  //!< species key
            
            //! display
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Actor &a)
            {
                if(a.nu>1)
                {
                    const string coef = vformat("%lu",a.nu);
                    os << coef << ' ';
                }
                return os << *a;
            }

            //! return max extent for a given concentration
            double maxExtent(const double C) const throw();
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unsigned long nu; //!< coefficient

        private:
            Y_DISABLE_ASSIGN(Actor);
        };

    }
}

#endif
