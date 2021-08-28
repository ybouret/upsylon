

//! \file

#ifndef Y_CHEMICAL_ACTORS_INCLUDED
#define Y_CHEMICAL_ACTORS_INCLUDED 1

#include "y/chemical/actor.hpp"
#include "y/type/gateway.hpp"

namespace upsylon
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! database of unique actors
        //
        //______________________________________________________________________
        class Actors : public gateway<const Actor::Map>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Actors();          //!< setup empty
            virtual ~Actors() throw();  //!< cleanup
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! register
            void operator()(const size_t, const Species &sp);
            
            //! find
            bool owns(const Species &sp) const throw();
            
            //! display
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Actors &A)
            {
                const ANode *node = A->head();
                if(node)
                {
                    os << **node;
                    for(node=node->next;node;node=node->next)
                    {
                        os << '+' << **node;
                    }
                }
                return os;
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Actors);
            virtual const_type &bulk() const throw();
            Actor::Map adb;
        };
    }
    
}

#endif

