//! \file
#ifndef Y_LANG_DYNAMO_PARSER_INCLUDED
#define Y_LANG_DYNAMO_PARSER_INCLUDED 1

#include "y/lang/syntax/parser.hpp"

namespace upsylon
{
    namespace Lang
    {
        typedef Syntax::Node XNode; //!< Alias

        //! Parser for the Dynamo Grammar, produces a raw Syntax::Node tree
        /**
         - dynamo : module item*;            // top level rule
         - module : "[.]{ID}" ';';           // module name
         - item   : rule | plg | lxr | cmd;  // different kinds of items
         - rid    : "{ID}";                  // rule identifier
         - lid    : "@{ID}";                 // lexical command identifier
         - plg    : lid ':' rid ';' ;        // plugin identifier
         - rx     : jstring;                 // regular expression
         - rs     : rstring;                 // raw string
         - lxr    : lid ':' (rs|rs)* ';' ;   // lexer command
         - cid    : "%{ID}";                 // pre-processing identifier
         - cmd    : cid ':' rs* ';' ;        // pre-processing command
         - endl   : "${ID}" ':' (rs|rx);     // special End Of Line trigger
         - str    : rx | rs;                 // different kinds of string
         - op     : str '^'?                 // an operator
         - aka    : rid ':' op;              // an alias
         - rule   : rid '!'? ':' rAgg ';'    // definition of a rule, '!' for a design rule
         - rAgg   : rAlt+;                   // aggregate
         - rAlt!  : rJk ( '|' rJk)*;         // possible alternation
         - rJk!   : rAtm ('?'|'+'|'*')?;     // joker
         - rAtm   : rid | op | '(' rAgg ')'; // recursive
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

