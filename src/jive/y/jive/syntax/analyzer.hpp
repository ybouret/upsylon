//! \file
#ifndef Y_JIVE_SYNTAX_ANALYZER_INCLUDED
#define Y_JIVE_SYNTAX_ANALYZER_INCLUDED 1

#include "y/jive/syntax/node.hpp"


namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {

            class Analyzer
            {
            public:
                const Tag name;
                template <typename ID> inline
                explicit Analyzer(const ID &id) : name( Tags::Make(id) ),
                depth(0)
                {
                }
                
                virtual ~Analyzer() throw();

                virtual int onTerminal(const string &id, const Lexeme &data);
                virtual int onInternal(const string &id, const size_t  size);

                int walk(const Node *);

                const size_t depth;

                template <typename OSTREAM>
                OSTREAM & indent( OSTREAM &os ) const
                {
                    for(size_t i=depth;i>0;--i) os << '.';
                    os << "|_";
                    return os;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Analyzer);
                int walkDown(const Node *node);
                int walkDownInternal(const Node *node);
            };

        }

    }

}


#endif

