
//! \file

#ifndef Y_ALCHEMY_SPECIES_INCLUDED
#define Y_ALCHEMY_SPECIES_INCLUDED 1

#include "y/string.hpp"
#include "y/ptr/intr.hpp"
#include "y/associative/hash/set.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        //______________________________________________________________________
        //
        //
        //! one species with minimal info
        //
        //______________________________________________________________________
        class Species :  public object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef intr_ptr<string,Species>          Pointer; //!< alias
            typedef hash_set<string,Species::Pointer> Set;     //!< alias
            typedef Set::node_type                    Node;    //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! constructor
            template <typename ID> inline
            explicit Species(const ID    &_name,
                             const long   _z,
                             const size_t _indx) :
            name(_name),
            z(_z),
            indx(_indx),
            active(false)
            {
                checkIndex();
            }

            //! cleanup
            virtual ~Species() throw();

            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const string & key() const throw(); //!< for library

            
            //! display as concentration
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Species &sp)
            {
                return (os << '[' << sp.name << ']');
            }
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name;   //!< identifier
            const long   z;      //!< algebraic charge
            const size_t indx;   //!< index in library
            const bool   active; //!< modified upon equilibrium

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
            void checkIndex() const;
        };
        
    }
    
}

#endif

