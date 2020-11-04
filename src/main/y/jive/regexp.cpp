#include "y/jive/pattern/all.hpp"
#include "y/jive/pattern/basic/rework.hpp"
#include "y/jive/pattern/dictionary.hpp"
#include "y/jive/pattern/posix.hpp"

#include "y/ptr/auto.hpp"
#include "y/exception.hpp"
#include "y/code/utils.hpp"

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
#define LBRACK '['
#define RBRACK ']'
#define IGN    '&'

#define CARET  '^'
#define DASH   '-'
#define COLON  ':'

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
                
                inline  RegExpCompiler(const char       *ini,
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
                inline Logical * compile()
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
                                ++curr; //! skip dot
                                p->push_front( posix::dot() );
                                break;

                            case IGN: {
                                auto_ptr<Pattern> arg = get_previous(*p);
                                p->push_back( Pattern::IgnoreCase( & *arg ) );
                            } break;
                                
                                
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
                                // clusters
                                //
                                //----------------------------------------------
                            case LBRACK:
                                p->push_back( cluster() );
                                assert(curr[-1]==RBRACK);
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
                inline Pattern * get_previous(Logical &ops)
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
                inline void braces(Logical &p)
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
                                const Pattern *alias = dict->query(ctx);
                                if(!alias)  throw exception("%sno entry '%s' in dictionary for '%s'",fn,*ctx,expr);
                                p.push_back( alias->clone() );
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


                //--------------------------------------------------------------
                //
                // helper
                //
                //--------------------------------------------------------------
                static inline bool isDigit(const char C) throw()
                {
                    return (C>='0'&&C<='9');
                }


                //--------------------------------------------------------------
                //
                // helper
                //
                //--------------------------------------------------------------
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
                
                inline Single *hexadecimalEscape()
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
                
                inline Single *compileEscape()
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
                        case  IGN:
                        case  LPAREN:
                        case  RPAREN:
                        case  LBRACE:
                        case  RBRACE:
                        case  LBRACK:
                        case  RBRACK:
                            return Single::Create(C);
                            
                            TRY_ESCAPE();
                            
                        default: break;
                    }
                    
                    throw exception("%sunknown escape sequence \\x%s... in '%s'",fn,cchars::visible[ uint8_t(C) ],expr);
                }

                //--------------------------------------------------------------
                //
                //
                // building clusters
                //
                //
                //--------------------------------------------------------------
                inline Pattern *cluster()
                {
                    auto_ptr<Logical> p = 0;
                    assert(*curr==LBRACK);
                    //----------------------------------------------------------
                    //
                    // get first char
                    //
                    //----------------------------------------------------------
                    {
                        if(++curr>=last) throw exception("%sunfinished cluster in '%s'",fn,expr);
                        const char C = *curr;
                        switch(C)
                        {
                            case COLON: return Posix();
                            case CARET: p = None::Create(); ++curr; break;
                            case DASH:  p = Or::Create(); p->add(DASH); ++curr; break;
                            default:    p = Or::Create(); break;
                        }
                    }
                    assert(p.is_valid());
                    Y_RX_PRINTLN("<cluster>");
                    
                    //----------------------------------------------------------
                    //
                    // loop
                    //
                    //----------------------------------------------------------
                    while(curr<last)
                    {
                        const char C = *curr;
                        switch(C)
                        {
                                //----------------------------------------------
                                // end of cluster
                                //----------------------------------------------
                            case RBRACK:
                                ++curr;
                                goto END_OF_CLUSTER;

                                //----------------------------------------------
                                // recursive nested cluster
                                //----------------------------------------------
                            case LBRACK:
                                p->push_back( cluster() );
                                break;

                                //----------------------------------------------
                                // range
                                //----------------------------------------------
                            case DASH: {
                                if( p->size<=0)        throw exception("%s no previous pattern for range in '%s'",fn,expr);
                                if( !p->tail->is<Single>() ) throw exception("%s left side of range is not a character in '%s'",fn,expr);
                                uint8_t lower = p->tail->as<Single>()->code;
                                delete p->pop_back();
                                auto_ptr<Single> rhs = clusterRangeRHS();
                                uint8_t upper = rhs->code;
                                if(lower>upper) cswap(lower,upper);
                                p->push_back(  Rework::Single2Range( rhs.yield(), lower, upper) );
                            } break;


                                //----------------------------------------------
                                // escape sequence
                                //----------------------------------------------
                            case '\\':
                                p->push_back(clusterEscape());
                                break;

                            default:
                                Y_RX_PRINTLN("add '" << cchars::to_visible(C)<< "'");
                                p->add( C );
                                ++curr;
                                break;
                        }
                    }
                    throw exception("%sunfinished cluster in '%s'",fn,expr);

                END_OF_CLUSTER:
                    Y_RX_PRINTLN("<cluster/>");
                    if(p->size<=0)
                    {
                        switch(p->uuid)
                        {
                            case None::UUID: return Any::Create();
                            default: break;
                        }
                        throw exception("%sillegal empty cluster in '%s'",fn,expr);
                    }
                    return p.yield();
                }

                //--------------------------------------------------------------
                //
                //
                // cluster escape
                //
                //
                //--------------------------------------------------------------
                inline Single *clusterEscape()
                {
                    assert(*curr=='\\');
                    if(++curr>=last) throw exception("%sunfinished escaped sequence in '%s'",fn,expr);
                    const char C = *(curr++);
                    switch(C)
                    {
                        case  'x'   : return hexadecimalEscape();
                        case '\\'   :
                        case  CARET :
                        case  DASH  :
                        case  COLON :
                        case  LBRACK:
                        case  RBRACK:
                            return Single::Create(C);

                            TRY_ESCAPE();

                        default: break;
                    }

                    throw exception("%sunknown escape sequence \\x%s... in '%s'",fn,cchars::to_visible(C),expr);
                }

                //--------------------------------------------------------------
                //
                //
                // cluster escape
                //
                //
                //--------------------------------------------------------------
                inline Single *clusterRangeRHS()
                {

                    assert(DASH==*curr);
                    if(++curr>=last) throw exception("%sunfinished range in '%s'",fn,expr);
                    const char C = *curr;
                    switch(C)
                    {
                        case '\\':  return clusterEscape();

                        case DASH:
                        case LBRACK:
                        case RBRACK:
                        case CARET:
                            throw exception("%sinvalid right hand side '%s' for range in '%s'",fn,cchars::to_visible(C),expr);

                        default: break;
                    }
                    ++curr;
                    return Single::Create(C);

                }

                //--------------------------------------------------------------
                //
                //
                // get a posix patter
                //
                //
                //--------------------------------------------------------------
                inline Pattern *Posix()
                {
                    assert(curr[-1]==LBRACK);
                    assert(curr[ 0]==COLON);

                    //----------------------------------------------------------
                    //
                    // loop to end of id
                    //
                    //----------------------------------------------------------
                    const char *org  = ++curr;
                    const char *next = curr+1;
                    while(next<last)
                    {
                        if( (COLON==*curr) && (RBRACK==*next) )
                        {
                            //--------------------------------------------------
                            // found
                            //--------------------------------------------------
                            const string id(org,curr-org);
                            Y_RX_PRINTLN("<posix ID=:" << id << ":>");
                            Pattern *p = posix::query(id);
                            if(!p) throw exception("%sno posix [:%s:] in '%s'",fn,*id,expr);
                            curr += 2;
                            return p;
                        }
                        ++curr;
                        ++next;
                    }
                    throw exception("%sunfinished posix ID in '%s",fn,expr);
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
            //ptr->graphViz("regexp.dot");
            return Pattern::Optimize(ptr.yield());
        }
        
        Pattern * RegExp(const char *text, const Dictionary *dict)
        {
            const string _(text);
            return RegExp(_,dict);
        }
        
        Pattern * RegExp(const char    C, const Dictionary *)
        {
            return Single::Create(C);
        }

    }
    
}

