//! \file

#ifndef Y_ALCHEMY_LIBRARY_INCLUDED
#define Y_ALCHEMY_LIBRARY_INCLUDED 1

#include "y/alchemy/species.hpp"
#include "y/associative/hash/set.hpp"
#include <iomanip>

namespace upsylon
{
    namespace alchemy
    {

        //______________________________________________________________________
        //
        //
        //! library of species
        //
        //______________________________________________________________________
        class library
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef hash_set<string,species::pointer> db_type;        //!< alias
            typedef typename db_type::const_iterator  const_iterator; //!< alias
            static const char                         clid[];         //!< class id

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

            const db_type & operator*() const throw(); //!< access

            //! register a new species
            template <typename ID>
            const species & operator()(const ID &name, const int z)
            {
                return use( new species(name,z) );
            }

            //! prefix aligned name
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM       &os,
                                        const library &lib)
            {
                os << '{' << std::endl;
                for(const_iterator it=lib.db.begin();it!=lib.db.end();++it)
                {
                    const species &sp = **it;
                    sp.display(os << ' ',lib.max_name) << " : " << std::setw(3) << sp.z << " @" << sp.indx << std::endl;
                }
                os << '}';
                return os;
            }

            

            //! check that species is owned
            bool owns(const species &) const throw();

            //! compile
            size_t compile();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(library);
            db_type      db;

            const species &use(species *);

        public:
            const bool   compiled; //!< status flag
            const size_t max_name; //!< max name length
        };


    }

}



#endif
