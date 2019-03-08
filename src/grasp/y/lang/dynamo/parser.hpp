//! \file
#ifndef Y_LANG_DYNAMO_PARSER_INCLUDED
#define Y_LANG_DYNAMO_PARSER_INCLUDED 1

#include "y/lang/syntax/parser.hpp"

namespace upsylon
{
    namespace Lang
    {
        //! Parser for the Dynamo Grammar
        /**
         - dynamo : module item*;
         - module : "[.]{ID}" ';';
         - item   : rule | plg | lxr | cmd;
         - rid    : "{ID}";
         - lid    : "@{ID}";
         - plg    : lid ':' rid ';' ;
         - rx     : jstring;
         - rs     : rstring;
         - lxr    : lid ':' (rs|rs)* ';' ;
         - cid    : "%{ID}";
         - cmd    : cid ':' rs* ';' ;
         */
        class DynamoParser : public Syntax::Parser
        {
        public:
            explicit DynamoParser();          //!< setup the grammar
            virtual ~DynamoParser() throw();  //!< destructor

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoParser);
        };

      

    }
}

#endif

