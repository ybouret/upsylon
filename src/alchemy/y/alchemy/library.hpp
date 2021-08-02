//! \file

#ifndef Y_ALCHEMY_LIBRARY_INCLUDED
#define Y_ALCHEMY_LIBRARY_INCLUDED 1

#include "y/alchemy/species.hpp"
#include "y/ios/scribe.hpp"
#include "y/sequence/accessible.hpp"
#include "y/type/gateway.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        //______________________________________________________________________
        //
        //
        //! library of species
        //
        //______________________________________________________________________
        class Library : public gateway<const Species::Set>
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
            // methods
            //__________________________________________________________________
            //! register a new species
            template <typename ID>
            const Species & operator()(const ID &name, const long z)
            {
                return use( new Species(name,z,sdb.size()+1) );
            }


            bool  owns(const Species &) const throw();        //!< check ownership
            const Species & operator[](const string &) const; //!< look up by name
            const Species & operator[](const char *  ) const; //!< look up by name



            //! prefix aligned name
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM       &os,
                                        const Library &lib)
            {
                os << '{' << '\n';
                for(const_iterator it=lib.sdb.begin();it!=lib.sdb.end();++it)
                {
                    const Species &sp = **it;
                    lib.pad(os << ' ',sp);
                    os << vformat(" : %3ld @%u\n", sp.z, unsigned(sp.indx));
                }
                os << '}';
                return os;
            }


            //! display associated array
            template <typename OSTREAM,typename T> inline
            OSTREAM & display(OSTREAM &os, const accessible<T> &arr) const
            {
                const ios::scribe &_ = ios::scribe::query<T>();
                assert(arr.size()>=sdb.size());
                os << '{' << '\n';
                for(const_iterator it=sdb.begin();it!=sdb.end();++it)
                {
                    const Species &sp = **it;
                    pad(os << ' ',sp) << " = ";
                    _.print(os,arr[sp.indx]) << '\n';
                }
                os << '}';
                return os;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Library);
            template <typename OSTREAM> inline
            OSTREAM & pad(OSTREAM &os, const Species &sp) const
            {
                os << sp; for(size_t i=sp.name.size();i<snw;++i) os << ' ';
                return os;
            }
            virtual const_type &bulk() const throw(); //!< sdb
            const Species &     use(Species *);       //!< insert new species
            Species::Set        sdb;                  //!< species database
            const size_t        snw;                  //!< species name width
        };


    }

}



#endif
