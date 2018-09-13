//!\file
#ifndef Y_LANG_SYNTAX_RULE_INCLUDED
#define Y_LANG_SYNTAX_RULE_INCLUDED

#include "y/lang/syntax/node.hpp"
#include "y/lang/lexical/translator.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            //! arguments for accept proto
#define Y_LANG_SYNTAX_RULE_ARGS Node * &tree, Lexical::Translator &lexer, Source &source

            //! rule to accept lexeme(s)
            class Rule : public Object
            {
            public:
                typedef core::list_of_cpp<Rule> List; //!< alias

                const uint32_t uuid; //!< class identifier
                const string   name; //!< user's identifier
                Rule          *next; //!< for list
                Rule          *prev; //!< for list

                virtual ~Rule()  throw(); //!< destructor

                //! virtual accept method
                virtual bool accept( Y_LANG_SYNTAX_RULE_ARGS ) const = 0;

                //! handling of new nodes
                static inline void Grow( Node * &tree, Node *child ) throw()
                {
                    if(tree)
                    {
                        assert(tree->internal);
                        tree->add(child);
                    }
                    else
                    {
                        tree = child;
                    }
                }

            protected:
                //! initialize
                explicit Rule(const uint32_t t,const string &id);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rule);
            };

        }

    }

}

#endif

