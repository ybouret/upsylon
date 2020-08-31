//! \file

#ifndef Y_JIVE_PATTERN_MATCHING_INCLUDED
#define Y_JIVE_PATTERN_MATCHING_INCLUDED 1

#include "y/jive/regexp.hpp"

namespace upsylon
{
    namespace Jive
    {

        class Matching : public Motif
        {
        public:
            virtual ~Matching() throw();
            template <typename REGEXP> inline
            Matching(const REGEXP &rx, const Dictionary *dict =0 ) :
            Motif( RegExp(rx,dict) )
            {
            }

            Matching(const Matching &other) throw();

            bool exactly_(Module *) const;
            bool somehow_(Token &, Module *) const;

            template <typename DATANAME, typename DATATYPE> inline
            bool exactly( const DATANAME &dataName, const DATATYPE &data ) const
            {
                return exactly_( Module::OpenData(dataName,data) );
            }

            template <typename DATANAME,typename DATATYPE> inline
            bool somehow(Token &token, const DATANAME &dataName, const DATATYPE &data ) const
            {
                 return somehow_(token,Module::OpenData(dataName,data));
            }


        private:
            Y_DISABLE_ASSIGN(Matching);
        };

    }
}

#endif
