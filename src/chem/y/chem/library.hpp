//! \file
#ifndef Y_CHEM_LIBRARY_INCLUDED
#define Y_CHEM_LIBRARY_INCLUDED 1

#include "y/chem/species.hpp"
#include "y/associative/set.hpp"

namespace upsylon
{
    namespace Chemical
    {
        typedef set<string,Species::Pointer> LibraryType;


        class Library : public Object, public LibraryType
        {
        public:
            explicit Library(size_t n=0);
            virtual ~Library() throw();
            Library(const Library &other);

            Species        & operator()(const string &name, const int z);
            inline Species & operator()(const char   *name, const int z)
            {
                const string _ = name;
                return (*this)(_,z);
            }

            size_t max_name_length() const throw();

        private:
            Y_DISABLE_ASSIGN(Library);
        };

    }
}

#endif

