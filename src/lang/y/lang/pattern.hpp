//! \file
#ifndef Y_LANG_PATTERN_INCLUDED
#define Y_LANG_PATTERN_INCLUDED 1

#include "y/lang/token.hpp"
#include "y/ios/ocstream.hpp"
#include "y/lang/source.hpp"
#include "y/type/fourcc.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! a pattern to accept from a source
        class Pattern : public Object
        {
        public:
            const uint32_t uuid; //!< unique ID for the class
            Pattern       *next; //!< for List
            Pattern       *prev; //!< for List

            virtual ~Pattern() throw(); //!< destructor

            //! emit its address for GraphViz
            ios::ostream & emitAddress( ios::ostream &os ) const;

            virtual Pattern *clone() const = 0;
            virtual bool     match( Token &tkn, Source &src) const = 0;

            typedef core::list_of_cloneable<Pattern> List;
            
        protected:
            //! build pattern from ID
            explicit Pattern(const uint32_t id)  throw();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pattern);
        };

        
    }
}

#endif

