
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


            const string & key() const throw();  //!< species key

            //! display with widths for names and coefficients
            template <typename OSTREAM> inline
            OSTREAM & display(OSTREAM &os, const size_t name_width=0, const size_t coef_width=0) const
            {
                size_t imin = 0;
                if(nu>1)
                {
                    const string coef = vformat("%lu",nu);
                    os << coef;
                    imin = coef.size();
                }
                for(size_t i=imin;i<coef_width;++i) os << ' ';
                os << ' ';
                return (**this).display(os,name_width);
            }
            

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
