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

        class Object : public counted_object
        {
        public:
            explicit Object() throw();
            virtual ~Object() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Object);
        };

        typedef memory::cslot<memory::global> LocalMemory;

        class Species : public Object
        {
        public:
            typedef intr_ptr<string,Species> Pointer;
            
            const string name;
            const int    z;
            LocalMemory  cache;

            inline Species( const string &identifier, const int charge ) :
            Object(), name(identifier), z(charge), cache() {}

            inline Species( const char *identifier, const int charge ) :
            Object(), name(identifier), z(charge), cache() {}

            inline virtual ~Species() throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
        };
        

    }
}

#endif

