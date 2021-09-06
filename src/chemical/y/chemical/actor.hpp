
//! \file

#ifndef Y_CHEMICAL_ACTOR_INCLUDED
#define Y_CHEMICAL_ACTOR_INCLUDED 1

#include "y/chemical/species.hpp"
#include "y/associative/be-key.hpp"
#include "y/associative/suffix/map.hpp"

namespace upsylon
{
    namespace Chemical
    {
        
        //______________________________________________________________________
        //
        //
        //! information about an actor
        //
        //______________________________________________________________________
        class Actor
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef suffix_map<be_key,const Actor> Map;   //!< database
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            Actor(const size_t, const Species &) throw(); //!< setup
            ~Actor() throw();                              //!< cleanup
            Actor(const Actor &) throw();                          //!< copy
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            //! display
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Actor &a)
            {
                if(a.nu>1)
                {
                    os << a.nuString() << ' ';
                }
                return os << a.sp;
            }

            //! nu or empty
            string nuString() const;


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Species &sp;   //!< species
            const size_t   nu;   //!< coefficient > 0
            const size_t   nu1;  //!< nu-1
            const unit_t   snu;  //!< signed nu >0
            
            
            
        private:
            Y_DISABLE_ASSIGN(Actor);
        };
        
        //______________________________________________________________________
        //
        //! iterator
        //______________________________________________________________________
        typedef Actor::Map::data_node ANode;

    }
    
}

#endif

