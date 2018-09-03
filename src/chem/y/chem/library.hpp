//! \file
#ifndef Y_CHEM_LIBRARY_INCLUDED
#define Y_CHEM_LIBRARY_INCLUDED 1

#include "y/chem/species.hpp"
#include "y/associative/set.hpp"
#include "y/sequence/array.hpp"

namespace upsylon
{
    namespace Chemical
    {
        //! base type for Library
        typedef set<string,Species::Pointer> LibraryType;


        //! Database of species
        class Library : public Object, public LibraryType
        {
        public:
            explicit Library(size_t n=0);  //!< initialize
            virtual ~Library() throw();    //!< destructor
            Library(const Library &other); //!< copy

            //! register a species
            Species        & operator()(const string &name, const int z);

            //! register a speceis
            inline Species & operator()(const char   *name, const int z)
            {
                const string _ = name;
                return (*this)(_,z);
            }

            //! get max name length of registered speceis
            size_t max_name_length() const throw();

            //! output an array mapped on library
            template <typename T>
            inline void display(std::ostream &os, const array<T> &a,const char *pfx=0) const
            {
                assert(a.size()>=size());
                const size_t sz = max_name_length();
                size_t       j  = 1;
                for(const_iterator i=begin();i!=end();++i,++j)
                {
                    if(pfx) os << pfx;
                    const string &name = (**i).name;
                    os << '[' <<  name << ']'; for(size_t k=name.size();k<=sz;++k) os << ' ';
                    os << ':' << ' ' << a[j] << std::endl;
                }
            }


        private:
            Y_DISABLE_ASSIGN(Library);
        };

    }
}

#endif

