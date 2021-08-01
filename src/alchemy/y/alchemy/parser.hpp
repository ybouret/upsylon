//! \file

#ifndef Y_ALCHEMY_PARSER_INCLUDED
#define Y_ALCHEMY_PARSER_INCLUDED 1

#include "y/jive/language/parser.hpp"

namespace upsylon
{
    namespace alchemy
    {
        typedef Jive::Language::Parser parser_;
        
        class parser : public parser_
        {
        public:
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(parser);
        };
        
    }
}

#endif

