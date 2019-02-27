//! \file
#ifndef Y_LANG_SYNTAX_RULE_INCLUDED
#define Y_LANG_SYNTAX_RULE_INCLUDED 1

#include "y/lang/syntax/node.hpp"
#include "y/lang/lexical/translator.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

#define Y_LANG_SYNTAX_ACCEPT_PROTO() virtual bool         accept(Source &source, Lexer &lexer, Node * &tree ) const
#define Y_LANG_SYNTAX_ACCEPT_START(CLASS)    bool CLASS:: accept(Source &source, Lexer &lexer, Node * &tree ) const

            class Rule : public Object, public core::inode<Rule>
            {
            public:
                typedef core::list_of_cpp<Rule> List; //!< alias

                const uint32_t uuid;    //!< class identifier
                const string   name;    //!< user's identifier
                const void    *derived; //!< helper for optimizations
                bool           verbose; //!< for output

                virtual ~Rule() throw();

                Y_LANG_SYNTAX_ACCEPT_PROTO()  = 0;
                virtual const char *typeName() const throw() = 0; //!< get a textual name
                virtual bool        isHollow() const throw() = 0; //!< if accepts an empty node

            protected:
                explicit Rule(const uint32_t i, const string &n);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rule);
            };

        }
    }
}

#endif

