//! \file
#ifndef Y_CHEM_LIBRARY_INCLUDED
#define Y_CHEM_LIBRARY_INCLUDED 1

#include "y/chem/species.hpp"
#include "y/associative/set.hpp"
#include "y/sequence/array.hpp"
#include "y/ios/ostream.hpp"

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

            //! register a species, wrapper
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

            //! check indices consistency
            void update() throw();

            //! access by name
            Species        & operator[](const string &id ) const;

            //! access by name
            Species & operator[](const char   *id ) const;

            //! write species
            ios::ostream & header( ios::ostream &fp ) const;

            //! write concentrations only
            ios::ostream & xprint( ios::ostream &fp, const array<double> &a ) const;


            //! return -log10([H+]) or -1 if does not exists
            double pH( const array<double> &a ) const;

        private:
            const string protonID; //!< "H+"
            Y_DISABLE_ASSIGN(Library);
        };

    }
}

#endif

