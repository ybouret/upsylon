//! \file

#ifndef Y_CHEM_SPECIES_INCLUDED
#define Y_CHEM_SPECIES_INCLUDED 1

#include "y/string.hpp"
#include "y/ptr/intr.hpp"
#include "y/ptr/counted.hpp"
#include "y/memory/cslot.hpp"

namespace upsylon
{
    namespace Chemical
    {

        //! object base class
        class Object : public counted_object
        {
        public:
            explicit Object() throw(); //!< constructor
            virtual ~Object() throw(); //!< destructor

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Object);
        };

        typedef memory::cslot<memory::global> LocalMemory; //!< local memory for species

        //! species info
        class Species : public Object
        {
        public:
            typedef intr_ptr<string,Species> Pointer; //!< for database
            
            const string name;  //!< unique name
            const int    z;     //!< algebraic charge
            const size_t indx;  //!< index in library
            LocalMemory  cache; //!< extra data

            //! constructor
            inline Species( const string &identifier, const int charge, const size_t ii ) :
            Object(), name(identifier), z(charge), indx(ii), cache() {  }

            //! constructor
            inline Species( const char *identifier, const int charge, const size_t ii) :
            Object(), name(identifier), z(charge), indx(ii), cache() {}

            //! destructor
            inline virtual ~Species() throw() {}

            //! key for Library
            inline const string & key() const throw() { return name; }

            friend std::ostream & operator<<(std::ostream &, const Species &);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
        };
        

    }
}

#endif

