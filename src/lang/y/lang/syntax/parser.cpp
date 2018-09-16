
#include "y/lang/syntax/parser.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {

        namespace Syntax
        {
            Parser:: ~Parser() throw()
            {
            }

            Parser:: Parser(const string &id) :
            Lexical::Translator(id),
            Grammar(label)
            {
            }

            const Rule & Parser:: __term( const string &id, const string &rx, const Terminal::Attribute attr)
            {
                static const char fn[] = "Parser:terminal: ";
                Grammar &G = *this;
                Lexer   &L = *this;
                Scanner &S = *L;

                const Rule *rule = G.getRuleByName(id);
                if( rule )
                {
                    //----------------------------------------------------------
                    //
                    // previously declared:
                    //
                    //----------------------------------------------------------

                    //----------------------------------------------------------
                    // check it is another terminal
                    //----------------------------------------------------------
                    if( Terminal::UUID != rule->uuid ) throw exception("%s previous other syntax rule '%s'", fn, *id);

                    //----------------------------------------------------------
                    // todo: check attribute ?
                    //----------------------------------------------------------

                    //----------------------------------------------------------
                    // check recorded into lexer as well
                    //----------------------------------------------------------
                    const Lexical::Rule *l = S.getRuleByLabel(id);
                    if(!l) throw exception("%s corrupted parser, no lexical rule '%s'",fn,*id);

                    //----------------------------------------------------------
                    // check same pattern
                    //----------------------------------------------------------
                    const string lhs = l->motif->to_binary();
                    const auto_ptr<const Pattern> q = Compile::RegExp(rx,S.userDict);
                    const string rhs = q->to_binary();

                    if( lhs != rhs )
                    {
                        throw exception("%s '%s' compiles to different pattern", fn, *id);
                    }


                    return *rule;
                }
                else
                {
                    S.emit(id,rx);               // register in Lexer's root
                    return G.terminal(id,attr);  // register in Grammar
                }

            }


        }
    }
}

