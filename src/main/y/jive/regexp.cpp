#include "y/jive/pattern/all.hpp"
#include "y/jive/pattern/dictionary.hpp"
#include "y/jive/pattern/posix.hpp"

#include "y/ptr/auto.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    
    namespace Jive
    {
        
        namespace
        {
            
#define LPAREN '('
#define RPAREN ')'
#define ALT    '|'
#define LBRACE '{'
#define RBRACE '}'
            
#define Y_RX_PRINTLN(OUTPUT) \
do { if(RegExpCompiler::Verbose) { indent(std::cerr << "|_") << OUTPUT << std::endl; } } while(false)
            
            static const char fn[] = "Jive::RegExp: ";
            
            //==================================================================
            //
            // Regular Expression Compiler
            //
            //==================================================================
            class RegExpCompiler
            {
            public:
                
                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                static bool        Verbose;
                const char        *curr; //!< current position
                const char        *last; //!< invalid position
                unsigned           rank; //!< current recursivity level
                const Dictionary  *dict; //!< optional dictionary
                const char * const expr; //!< initial expression
                
                inline std::ostream & indent(std::ostream &os) const {
                    unsigned r = rank; while(r-- >0) std::cerr << "__";
                    return os;
                }
                
                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                
                //! cleanup
                inline ~RegExpCompiler() throw()
                {
                    curr = 0;
                    last = 0;
                    rank = 0;
                    dict = 0;
                }
                
                explicit RegExpCompiler(const char       *ini,
                                        const char       *end,
                                        const Dictionary *usr) :
                curr(ini),
                last(end),
                rank(0),
                dict(usr),
                expr(ini)
                {
                    
                }
                
                //--------------------------------------------------------------
                //
                //
                // main entry point: gathering patterns
                //
                //
                //--------------------------------------------------------------
                Logical * compile()
                {
                    //----------------------------------------------------------
                    //
                    // create a new pattern
                    //
                    //----------------------------------------------------------
                    Y_RX_PRINTLN("<compile@" << rank <<">");
                    auto_ptr<Logical> p = And::Create();
                    
                    //----------------------------------------------------------
                    //
                    // main RX loop
                    //
                    //----------------------------------------------------------
                    while(curr<last)
                    {
                        
                        const char C = *curr;
                        switch(C)
                        {
                                
                                //----------------------------------------------
                                //
                                // sub-expressions
                                //
                                //----------------------------------------------
                            case LPAREN:
                                //----------------------------------------------
                                Y_RX_PRINTLN("<sub-compile>");
                                //----------------------------------------------
                                ++curr; // skip LPAREN
                                ++rank; // increase rank
                                p->push_back( compile() );
                                break;
                                
                            case RPAREN:
                                //----------------------------------------------
                                Y_RX_PRINTLN("<sub-compile/>");
                                //----------------------------------------------
                                --rank; // decrease rank
                                ++curr; // skip RPAREN
                                goto COMPILED;
                                
                                //----------------------------------------------
                                //
                                // alternation
                                //
                                //----------------------------------------------
                            case ALT: {
                                //----------------------------------------------
                                Y_RX_PRINTLN("<alternation>");
                                //----------------------------------------------
                                ++curr; // skip ALT
                                auto_ptr<Logical> alt = Or::Create();
                                alt->push_front(p.yield());
                                alt->push_back(compile());
                                return alt.yield(); }
                                
                                //----------------------------------------------
                                //
                                // jokers
                                //
                                //----------------------------------------------
                            case '*':
                                p->push_back( Repeating::Create( get_previous(*p), 0) );
                                break;
                                
                            case '+':
                                p->push_back( Repeating::Create( get_previous(*p), 1) );
                                break;
                                
                            case '?':
                                p->push_back( Optional::Create( get_previous(*p) ) );
                                break;
                                
                            case '~': {
                                auto_ptr<Logical> q = None::Create();
                                q->push_back( get_previous(*p) );
                                p->push_back( q.yield() );
                            } break;
                                
                            case '.':
                                ++curr;
                                p->push_front( posix::dot() );
                                break;
                                
                                
                            case  LBRACE:
                                braces(*p);
                                break;
                                
                                //----------------------------------------------
                                //
                                // escape sequences
                                //
                                //----------------------------------------------
                            case '\\':
                                p->push_back( compileEscape() );
                                break;
                            
                                
                                //----------------------------------------------
                                //
                                // default
                                //
                                //----------------------------------------------
                            default:
                                //----------------------------------------------
                                Y_RX_PRINTLN("<single '" << cchars::visible[uint8_t(C)] << "'>");
                                //----------------------------------------------
                                p->add(*(curr++));
                                break;
                        }
                        
                    }
                    
                COMPILED:
                    //----------------------------------------------------------
                    //
                    // end of current expression
                    //
                    //----------------------------------------------------------
                    Y_RX_PRINTLN("<compile@" << rank <<"/>");
                    if(p->size<=0) throw exception("%sempty expression in '%s'",fn,expr);
                    return p.yield();
                }
                
                //--------------------------------------------------------------
                //
                //
                // get the previous pattern
                //
                //
                //--------------------------------------------------------------
                Pattern * get_previous(Logical &ops)
                {
                    if(ops.size<=0) throw exception("%sno previous pattern before '%c' in '%s'",fn,*curr,expr);
                    ++curr;
                    return ops.pop_back();
                }
                
                //--------------------------------------------------------------
                //
                //
                // braces joker
                //
                //
                //--------------------------------------------------------------
                void braces(Logical &p)
                {
                    assert(*curr==LBRACE);
                    //----------------------------------------------------------
                    //
                    // look for closing right brace
                    //
                    //----------------------------------------------------------
                    const char *org = ++curr;
                    while(curr<last)
                    {
                        if(RBRACE== *curr)
                        {
                            //--------------------------------------------------
                            //
                            // <process>
                            //
                            //--------------------------------------------------
                            const char  *rbrace = curr++;     // skip rbrace
                            const string ctx(org,rbrace-org);

                            Y_RX_PRINTLN("<braces: {" << ctx <<"}>");

                            if(ctx.size()<=0) throw exception("%sempty braces in '%s'",fn,expr);

                            if(isDigit(ctx[0]))
                            {
                                //----------------------------------------------
                                //
                                // counting/repeating
                                //
                                //----------------------------------------------
                                if(p.size<=0) throw exception("%sno pattern before braces in '%s'",fn,expr);
                                const char *coma = strchr(org, ',');
                                if(coma)
                                {
                                    //------------------------------------------
                                    //  counting
                                    //------------------------------------------
                                    const string minStr(org,coma-org); ++coma;
                                    const string maxStr(coma,rbrace-coma);
                                    Y_RX_PRINTLN("--> min=<" << minStr << ">(#" << minStr.size() <<") | max=<" << maxStr << ">(#" << maxStr.size() << ")");
                                    const size_t minCount = string2count(minStr,"minCount");
                                    const size_t maxCount = string2count(maxStr,"maxCount");
                                    p.push_back( Counting::Create( p.pop_back(), minCount, maxCount));
                                }
                                else
                                {
                                    //------------------------------------------
                                    //  repeating
                                    //------------------------------------------
                                    const string & minStr = ctx;
                                    Y_RX_PRINTLN("--> min=<" << minStr << ">(#" << minStr.size() <<")");
                                    const size_t minCount = string2count(minStr,"minCount");
                                    p.push_back( Repeating::Create( p.pop_back(), minCount) );
                                }
                            }
                            else
                            {
                                //----------------------------------------------
                                //
                                // alias
                                //
                                //----------------------------------------------
                                if(!dict) throw exception("%sno dictionary for alias '%s' in '%s'",fn,*ctx,expr);
                                const Motif *ppp = dict->search_by(ctx);
                                if(!ppp)  throw exception("%sno entry '%s' in dictionary for '%s'",fn,*ctx,expr);
                                p.push_back( (**ppp).clone() );
                            }
                            //--------------------------------------------------
                            //
                            // <process/>
                            //
                            //--------------------------------------------------
                            return;
                        }
                        ++curr;
                    }
                    throw exception("%sunfinished braces in '%s'",fn,expr);
                }
                
                static inline bool isDigit(const char C) throw()
                {
                    return (C>='0'&&C<='9');
                }

                inline size_t string2count(const string &s, const char *id)
                {
                    assert(id);
                    size_t ans = 0;
                    const size_t n = s.size();
                    if(n<=0) throw exception("%sempty %s in '%s'",fn,id,expr);
                    for(size_t i=0;i<n;++i)
                    {
                        const char c = s[i];
                        if(!isDigit(c)) throw exception("%sinvalid digit '%s' in <%s> from '%s'",fn, cchars::visible[ uint8_t(c) ], *s, expr);
                        ans *= 10;
                        ans += c-'0';
                    }
                    return ans;
                }
                
                
                
                
                //--------------------------------------------------------------
                //
                //
                // escape sequences
                //
                //
                //--------------------------------------------------------------
                
                //--------------------------------------------------------------
                //
                // hexadecimal
                //
                //--------------------------------------------------------------
                
                Pattern *hexadecimalEscape()
                {
                    assert('x' ==curr[-1]);
                    assert('\\'==curr[-2]);
                    //----------------------------------------------------------
                    // get hi
                    //----------------------------------------------------------
                    if(curr>=last) throw exception("%smissing first escaped hexa in '%s'",fn,expr);
                    const int hi = hexadecimal::to_decimal(*curr);
                    if(hi<0) throw exception("%sinvalid first escaped hexa '%s' in '%s'",fn, cchars::visible[uint8_t(*curr)],expr);

                    //----------------------------------------------------------
                    // get lo
                    //----------------------------------------------------------
                    if(++curr>=last) throw exception("%smissing second escaped hexa in '%s'",fn,expr);
                    const int lo = hexadecimal::to_decimal(*curr);
                    if(lo<0) throw exception("%sinvalid second escaped hexa '%s' in '%s'",fn, cchars::visible[uint8_t(*curr)],expr);

                    //----------------------------------------------------------
                    // skip lo char
                    //----------------------------------------------------------
                    ++curr;

                    //----------------------------------------------------------
                    // build code
                    //----------------------------------------------------------
                    return Single::Create( (hi<<4) | lo );
                }
                
                //--------------------------------------------------------------
                //
                // macro for indirect escape
                //
                //--------------------------------------------------------------
                
#define TRY_ESCAPE()                   \
case 'n': return Single::Create('\n'); \
case 'r': return Single::Create('\r'); \
case 't': return Single::Create('\t')
                
                Pattern *compileEscape()
                {
                    assert(*curr=='\\');
                    if(++curr>=last) throw exception("%sunfinished escaped sequence in '%s'",fn,expr);
                    const char C = *(curr++);
                    switch(C)
                    {
                        case  'x': return hexadecimalEscape();
                        case '\\':
                        case  '+':
                        case  '*':
                        case  '?':
                        case  '~':
                        case  '.':
                        case  LPAREN:
                        case  RPAREN:
                        case  LBRACE:
                        case  RBRACE:
                            return Single::Create(C);
                            
                            TRY_ESCAPE();
                            
                        default: break;
                    }
                    
                    throw exception("%sunknown escape sequence \\x%s... in '%s'",fn,cchars::visible[ uint8_t(C) ],expr);
                }
                
                
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(RegExpCompiler);
            };
            
            //==================================================================
            
            bool RegExpCompiler::Verbose = false;
            
        }

        bool & RegExpVerbose() throw()
        {
            return RegExpCompiler::Verbose;
        }
        
        Pattern * RegExp(const string &rx, const Dictionary *dict)
        {
            const char       *ini = *rx;
            RegExpCompiler    rxc(ini,ini+rx.size(),dict);
            auto_ptr<Pattern> ptr = rxc.compile();
            if(rxc.rank!=0)
            {
                throw exception("%sunfinished expression '%s'",fn,ini);
            }
            ptr->graphViz("regexp.dot");
            return Pattern::Optimize(ptr.yield());
        }
        
        Pattern * RegExp(const char *text, const Dictionary *dict)
        {
            const string _(text);
            return RegExp(_,dict);
        }
        
        
    }
    
}

