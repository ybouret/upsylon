
#include "y/lang/syntax/parser.hpp"
#include "y/exception.hpp"
#include "y/code/utils.hpp"

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

            void Parser:: make_id_and_rx_from(const char C) throw()
            {
                const unsigned B = C;
                memset(__id,0,sizeof(__id));
                memset(__rx,0,sizeof(__rx));
                __id[0] = C;
                __rx[0] = '\\';
                __rx[1] = 'x';
                __rx[2] = hexadecimal::digit(B>>4);
                __rx[3] = hexadecimal::digit(B);
            }

            const string & Parser:: key() const throw()
            {
                return *label;
            }


            Node * Parser:: parse(Source &source,
                                  bool    keepRaw)
            {
                if(verbose) { std::cerr << "{" << name << "} Running..." << std::endl; }
                //______________________________________________________________
                //
                // reset and get raw tree
                //______________________________________________________________
                raw = 0;
                raw = run(*this,source); assert(raw.is_valid());

                //______________________________________________________________
                //
                // compact while detecting operators
                //______________________________________________________________
                if(verbose) { std::cerr << "{" << name << "} Compact Level-1" << std::endl; }
                bool rewrite = false;
                Node *ast = Node::Compact( keepRaw ? new Node(*raw) : raw.yield() , rewrite );
                if( rewrite )
                {
                    //__________________________________________________________
                    //
                    // rewrite the tree according to detected operators
                    //__________________________________________________________
                    if(verbose) { std::cerr << "{" << name << "} Rewriting detected..." << std::endl; }
                    ast     = Node::Rewrite(ast,*name);

                    //__________________________________________________________
                    //
                    // recompact
                    //__________________________________________________________
                    rewrite = false;
                    if(verbose) { std::cerr << "{" << name << "} Compact Level-2" << std::endl; }
                    ast     = Node::Compact(ast,rewrite);
                    if(rewrite)
                    {
                        delete ast;
                        throw exception("{%s} unexpected persistent rewrite", **name );
                    }
                }
                else
                {
                    if(verbose) { std::cerr << "{" << name << "} No Rewrite" << std::endl; }
                }

                
                return ast;
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
                    // check recorded into lexer as well
                    //----------------------------------------------------------
                    const Lexical::Rule *l = S.getRuleByLabel(id);
                    if(!l) throw exception("%s corrupted parser, no lexical rule '%s'",fn,*id);

                    //----------------------------------------------------------
                    // check same pattern
                    //----------------------------------------------------------
                    const string lhs = l->motif->to_binary();
                    const auto_ptr<const Pattern> q = RegExp(rx,S.userDict);
                    const string rhs = q->to_binary();

                    if( lhs != rhs )
                    {
                        throw exception("%s '%s' compiles to different pattern", fn, *id);
                    }

                    assert(rule->data);
                    const Terminal *t = static_cast<const Terminal *>(rule->data);

                    //check attribute
                    if(attr!=t->attr)
                    {
                        throw exception("%s '%s' gets a different attribute", fn, *id);
                    }

                    return *(Terminal *)t;
                    
                }
                else
                {
                    S.emit(id,rx);               // register in Lexer's root
                    return G.terminal(id,attr);  // register in Grammar
                }

            }


            Parser::RULE & Parser:: term(const string &id)
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



