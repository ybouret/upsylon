
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
            Grammar(label),
            root( **static_cast<Lexer *>(this) ),
            raw(0)
            {
            }

            const string & Parser:: key() const throw()
            {
                return *label;
            }


            Node * Parser:: parse( Source &source, bool keepRaw)
            {
                raw = 0;
                raw = run(*this,source); assert(raw.is_valid());
                if(keepRaw)
                {
                    return Node::AST( new Node( *raw ) );
                }
                else
                {
                    return Node::AST( raw.yield()      );
                }
            }



            Terminal & Parser:: __term(const string &id,
                                       const string &rx,
                                       const Terminal::Attribute attr)
            {
                static const char fn[] = "Parser:terminal: ";
                Grammar &G = *this;
                Scanner &S = root;

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

                    assert(rule->data);
                    const Terminal *t = static_cast<const Terminal *>(rule->data);
                    return *(Terminal *)t;
                    
                }
                else
                {
                    S.emit(id,rx);               // register in Lexer's root
                    return G.terminal(id,attr);  // register in Grammar
                }

            }


            const Rule & Parser:: term(const string &id)
            {
                Grammar     &G      = *this;
                const string toCall = Scanner::CallLabel(id);
                
                if(root.getRuleByLabel(toCall)==NULL)
                {
                    throw exception("{%s} no lexical rule '%s'", **(G.name), *toCall);
                }

                const Rule *r = G.getRuleByName(id);
                if(r)
                {
                    return *r;
                }
                else
                {
                    return terminal(id);
                }
            }


        }
    }
}



