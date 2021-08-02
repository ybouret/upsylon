//! \file

#ifndef Y_ALCHEMY_LIBRARY_INCLUDED
#define Y_ALCHEMY_LIBRARY_INCLUDED 1

#include "y/alchemy/species.hpp"
#include "y/ios/scribe.hpp"
#include "y/sequence/accessible.hpp"
#include "y/type/gateway.hpp"
#include <iomanip>

namespace upsylon
{
    namespace Alchemy
    {

        class Library : public gateway<const Species::Set>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef typename Species::Set::const_iterator const_iterator;
            static const char CLID[];
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Library();
            virtual ~Library() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! register a new species
            template <typename ID>
            const Species & operator()(const ID &name, const long z)
            {
                return use( new Species(name,z,sdb.size()+1) );
            }

            //! check ownership
            bool owns(const Species &) const throw();

            //! prefix aligned name
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM       &os,
                                        const Library &lib)
            {
                os << '{' << '\n';
                for(const_iterator it=lib.sdb.begin();it!=lib.sdb.end();++it)
                {
                    const Species &sp = **it;
                    os << ' ' << sp;
                    for(size_t i=sp.name.size();i<lib.snw;++i) os << ' ';
                    os << vformat(" : %3ld @%u\n", sp.z, unsigned(sp.indx));
                }
                os << '}';
                return os;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Library);
            virtual const_type &bulk() const throw(); //!< sdb
            const Species &     use(Species *);       //!< insert new species
            Species::Set        sdb;                  //!< species database
            const size_t        snw;                  //!< species name width
        };

#if 0
        //______________________________________________________________________
        //
        //
        //! library of species
        //
        //______________________________________________________________________
        class library : public compilable, public gateway<const species::db>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef hash_set<string,species::pointer> db_type;        //!< alias
            typedef typename db_type::const_iterator  const_iterator; //!< alias
            static const char                         clid[];         //!< "alchemy library"
                                                                      
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit library();
            virtual ~library() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            //! register a new species
            template <typename ID>
            const species & operator()(const ID &name, const int z)
            {
                return use( new species(name,z) );
            }




            //! display associated array
            template <typename OSTREAM,typename T> inline
            OSTREAM & display(OSTREAM &os, const accessible<T> &arr) const
            {
                assert(compiled);
                const ios::scribe &_ = ios::scribe::query<T>();
                assert(arr.size()>=db.size());
                os << '{' << '\n';
                for(const_iterator it=db.begin();it!=db.end();++it)
                {
                    const species &sp = **it;
                    os << ' ' << sp << " = ";
                    _.print(os,arr[sp.indx]) << '\n';
                }
                os << '}';
                return os;
            }


            //! check that species is owned
            bool owns(const species &) const throw();
            
            
           
        private:
            Y_DISABLE_COPY_AND_ASSIGN(library);
            db_type      db;

            const species &use(species *);
            virtual void   on_compile();
            virtual const species::db & bulk() const throw();
        };
#endif

    }

}



#endif
