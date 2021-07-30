
//! \file

#ifndef Y_ALCHEMY_SPECIES_INCLUDED
#define Y_ALCHEMY_SPECIES_INCLUDED 1

#include "y/string.hpp"
#include "y/ptr/intr.hpp"
#include "y/associative/hash/set.hpp"

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
            typedef intr_ptr<string,species>          pointer; //!< alias
            typedef hash_set<string,species::pointer> db;      //!< alias
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! flexible constructor
            template <typename ID> inline
            explicit species(const ID &the_name, const int the_z) :
            name(the_name), z(the_z), indx(0), width(0) {}
            virtual ~species() throw(); //!< cleanup

            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const string & key() const throw(); //!< for library

            
            //! display as species with width for name
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const species &sp)
            {
                os << '[' << sp.name << ']';
                for(size_t i=sp.name.size();i<sp.width;++i) os << ' ';
                return os;
            }
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name;  //!< identifier
            const long   z;     //!< algebraic charge
            const size_t indx;  //!< index in library
            const size_t width; //!< output width, set during library compile

        private:
            Y_DISABLE_COPY_AND_ASSIGN(species);
        };
        
    }
    
}

#endif

