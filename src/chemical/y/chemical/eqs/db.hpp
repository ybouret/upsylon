
//! \file

#ifndef Y_CHEMICAL_EQS_DB_INCLUDED
#define Y_CHEMICAL_EQS_DB_INCLUDED 1

#include "y/chemical/equilibria.hpp"
#include "y/associative/hash/map.hpp"

namespace upsylon
{
    namespace Chemical
    {
        
        //______________________________________________________________________
        //
        //
        //! base type for equilibria database
        //
        //______________________________________________________________________
        typedef hash_map<string,const string> EqMap;
        
        //______________________________________________________________________
        //
        //
        //! database as singleton
        //
        //______________________________________________________________________
        class EqDB : public singleton<EqDB>, public EqMap
        {
        public:
            
            void   decl(const string &info); //!< declare a compact form
            
            //! collect name/info matching regular expression
            size_t find(sequence<string> &name,
                        sequence<string> &info,
                        const string     &rx) const;
            
        private:
            virtual ~EqDB() throw();
            explicit EqDB();
            
            Y_DISABLE_COPY_AND_ASSIGN(EqDB);
            Y_SINGLETON_DECL_WITH(0,EqDB);
        };
        
        
    }
}

#endif

