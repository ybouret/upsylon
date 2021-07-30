
//! \file

#ifndef Y_ALCHEMY_SPECIES_INCLUDED
#define Y_ALCHEMY_SPECIES_INCLUDED 1

#include "y/string.hpp"
#include "y/ptr/intr.hpp"
#include "y/ios/align.hpp"

namespace upsylon
{
    namespace alchemy
    {
        //______________________________________________________________________
        //
        //
        //! one species with minimal info
        //
        //______________________________________________________________________
        class species :  public object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef intr_ptr<string,species> pointer; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! flexible constructor
            template <typename ID> inline
            explicit species(const ID &the_name, const int the_z) :
            name(the_name), z(the_z), indx(0) {}

            species(const species &);   //!< copy
            virtual ~species() throw(); //!< cleanup

            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const string & key() const throw(); //!< for library

            //! display as species with width for name
            template <typename OSTREAM> inline
            OSTREAM & display(OSTREAM &os, const size_t w=0) const
            {
                return os << ios::align(name,ios::align::left,w);
            }

            //! display as concentration
            template <typename OSTREAM> inline
            OSTREAM & display_concentration(OSTREAM &os, const size_t w=0) const
            {
                os << '[' << name << ']';
                for(size_t i=name.size();i<w;++i) os << ' ';
                return os;
            }
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name; //!< identifier
            const long   z;    //!< algebraic charge
            const size_t indx; //!< index in library


        private:
            Y_DISABLE_ASSIGN(species);
        };
        
    }
    
}

#endif

