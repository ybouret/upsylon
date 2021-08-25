//! \file

#ifndef Y_ALCHEMY_EQUILIBRIA_DB_INCLUDED
#define Y_ALCHEMY_EQUILIBRIA_DB_INCLUDED 1

#include "y/alchemy/equilibria.hpp"
#include "y/associative/hash/map.hpp"

namespace upsylon
{

    namespace Alchemy
    {
        //______________________________________________________________________
        //
        //
        //! database of name+encoded description
        //
        //______________________________________________________________________
        typedef hash_map<string,const string> EqInfo;

        //______________________________________________________________________
        //
        //
        //! database of encoded equilibria
        //
        //______________________________________________________________________
        class EqDB : public singleton<EqDB>, public gateway<const EqInfo>
        {
        public:

            void operator()(const string &info);                 //!< register
            const string & operator[](const string &name) const; //!< get
            const string & operator[](const char   *name) const; //!< get, wrapper

            //! find all equilibria matching regular expression
            size_t find(sequence<string> &which,
                        const string     &rx) const;


        private:
            explicit EqDB();
            virtual ~EqDB() throw();
            Y_SINGLETON_DECL_WITH(0,EqDB);
            virtual const_type & bulk() const throw();
            EqInfo db;
        };

    }

}

#endif
