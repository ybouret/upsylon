//! \file

#ifndef Y_ALCHEMY_LIBRARY_INCLUDED
#define Y_ALCHEMY_LIBRARY_INCLUDED 1

#include "y/alchemy/species.hpp"
#include "y/alchemy/freezable.hpp"
#include "y/type/gateway.hpp"
#include "y/ios/scribe.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        typedef memory::dyadic                 Allocator;    //!< alias
        typedef vector<string,Allocator>       Strings;      //!< alias

        //______________________________________________________________________
        //
        //
        //! library of species
        //
        //______________________________________________________________________
        class Library : public gateway<const Species::Set>, public Freezable
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef Species::Set::const_iterator const_iterator; //!< alias
            static const char                    CLID[];         //!< "Library"
            

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Library();
            virtual ~Library() throw();

            //__________________________________________________________________
            //
            // methods for species
            //__________________________________________________________________

            //! register a new species
            template <typename ID>
            const Species & operator()(const ID &name, const long z)
            {
                return use( new Species(name,z,sdb.size()+1) );
            }

            bool has(const string &) const throw(); //!< check species
            bool has(const char   *) const;         //!< check species

            bool            owns(const Species &) const throw(); //!< check ownership
            const Species & operator[](const string &)    const; //!< look up by name
            const Species & operator[](const char *  )    const; //!< look up by name

            const Species & operator()(const size_t indx) const; //!< slow access by index

            //! access array content by name
            template <typename ARR, typename ID> inline
            typename ARR::type &operator()(ARR &arr, const ID &id) const
            {
                const Library &self = *this;
                return arr[ self(id) ];
            }

            //! access array content by name, const
            template <typename ARR, typename ID> inline
            typename ARR::const_type operator()(const ARR &arr, const ID &id) const
            {
                const Library &self = *this;
                return arr[ self(id) ];
            }

            //! draw a random positive concentration
            void draw(randomized::bits &, Addressable &C) const throw();

            //__________________________________________________________________
            //
            // display methods
            //__________________________________________________________________

            //! display left aligned species
            template <typename OSTREAM> inline
            OSTREAM & print(OSTREAM &os, const Species &sp) const
            {
                os << sp; for(size_t i=sp.name.size();i<snw;++i) os << ' ';
                return os;
            }

            //! output info
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM       &os,
                                        const Library &lib)
            {
                os << '{' << '\n';
                for(const_iterator it=lib.sdb.begin();it!=lib.sdb.end();++it)
                {
                    const Species &sp = **it;
                    lib.print(os << ' ',sp);
                    os << vformat(" : %3ld @%u\n", sp.z, unsigned(sp.indx));
                }
                os << '}';
                return os;
            }

            //! format strings in order of species
            template <typename T> inline
            size_t format(Strings &str, const accessible<T> &arr) const
            {
                const ios::scribe &_   = ios::scribe::query<T>();
                size_t             ans = 0;

                str.free();
                for(const_iterator it=sdb.begin();it!=sdb.end();++it)
                {
                    const Species &sp  = **it;
                    const string   s   = _.write( &arr[sp.indx] );
                    const size_t   l   = s.size();
                    if(l>ans)      ans = l;
                    str.push_back(s);
                }
                return ans;
            }

            //! display associated array
            template <typename OSTREAM,typename T> inline
            OSTREAM & display(OSTREAM &os, const accessible<T> &arr) const
            {
                assert(arr.size()>=sdb.size());
                Strings      str(sdb.size(),as_capacity);
                const size_t smx = format(str,arr);
                os << '{' << '\n';
                size_t             j=1;
                for(const_iterator it=sdb.begin();it!=sdb.end();++it,++j)
                {
                    const Species &sp = **it;
                    const string  &sv = str[j];
                    print(os << ' ',sp) << " = ";
                    for(size_t i=sv.size();i<smx;++i) os << ' ';
                    os << sv;
                    os << '\n';
                }
                os << '}';
                return os;
            }

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            const Species &get(const string &); //!< simple parsing
            const Species &get(const char   *); //!< simple parsing wrapper

            //! simple parsing helper
            template <typename ID> inline
            Library & operator<<(const ID &id)
            {
                (void)get(id);
                return *this;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Library);
            virtual const_type &bulk() const throw(); //!< sdb
            const Species &     use(Species *);       //!< insert new species
            Species::Set        sdb;                  //!< species database
        public:
            const size_t        snw;                  //!< species name width
        };


    }

}



#endif
