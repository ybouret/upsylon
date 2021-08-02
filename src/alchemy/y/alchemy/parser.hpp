//! \file

#ifndef Y_ALCHEMY_PARSER_INCLUDED
#define Y_ALCHEMY_PARSER_INCLUDED 1

#include "y/jive/language/parser.hpp"

namespace upsylon
{
    namespace alchemy
    {
        typedef Jive::Language::Parser jParser; //!< alias

        //! parser
        class Parser : public jParser
        {
        public:
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };
        
    }
}

#endif

