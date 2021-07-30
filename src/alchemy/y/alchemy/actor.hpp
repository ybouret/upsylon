
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
        //! actor for equilibrium: species and coefficient
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
            // methods
            //__________________________________________________________________
            const string & key() const throw();  //!< species key
            
            //! display
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const actor &a)
            {
                if(a.nu>1)
                {
                    const string coef = vformat("%lu",a.nu);
                    for(size_t i=coef.size();i<a.cw;++i) os << ' ';
                    os << coef;
                }
                else
                {
                    for(size_t i=0;i<a.cw;++i) os << ' ';
                }
                os << ' ';
                return os << *a;
            }
            
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unsigned long nu; //!< coefficient
            const size_t        cw; //!< coefficient width
            
        private:
            Y_DISABLE_ASSIGN(actor);
        };

    }
}

#endif
