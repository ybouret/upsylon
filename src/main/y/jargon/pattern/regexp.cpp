#include "y/jargon/pattern/regexp.hpp"
#include "y/jargon/pattern/dictionary.hpp"
#include "y/jargon/pattern/all.hpp"
#include "y/jargon/pattern/posix.hpp"
#include "y/ptr/auto.hpp"
#include "y/exception.hpp"
//#include "y/string/tokenizer.hpp"
#include "y/string/convert.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        bool RegularExpression::Verbose = false;
        static const char fn[] = "RegularExpression: ";
        
#define LPAREN     '('
#define RPAREN     ')'
#define ALT        '|'
#define BACKSLASH  '\\'
#define LBRACK     '['
#define RBRACK     ']'
#define DASH       '-'
#define LBRACE     '{'
#define RBRACE     '}'
#define CARET      '^'
#define TILDE      '~'
#define DOT        '.'
#define COLON      ':'
        
#define Y_RX_VERBOSE(code) if (Verbose) do { code; } while(false)
        
        
        class RegularExpression::Engine
        {
        public:
            //------------------------------------------------------------------
            //
            // data to process
            //
            //------------------------------------------------------------------
            const char       *curr;
            const char       *last;
            long              depth;
            const Dictionary *dict;
            const char       *text;
            
            //------------------------------------------------------------------
            //
            // cleanup
            //
            //------------------------------------------------------------------
            inline ~Engine() throw() {}
            
            //------------------------------------------------------------------
            //
            // startup
            //
            //------------------------------------------------------------------
            inline Engine(const char *rx, const size_t nc, const Dictionary *d) :
            curr( rx ),
            last( rx + nc ),
            depth(0),
            dict( d ),
            text( curr )
            {
                assert( !(NULL==rx&&nc>0) );
                Y_RX_VERBOSE(std::cerr << fn << "initialized" << std::endl);
            }
            
            
            //------------------------------------------------------------------
            //
            //
            // compile expression starting from curr
            //
            //
            //------------------------------------------------------------------
            inline Pattern *compileExpression()
            {
                Y_RX_VERBOSE(std::cerr << fn << "[+] expression@" << depth << std::endl);
                auto_ptr<Logical> expr = AND::Create();
                
                while(curr<last)
                {
                    const char C = *curr;
                    
                    switch(C)
                    {
                            //--------------------------------------------------
                            //
                            // grouping
                            //
                            //--------------------------------------------------
                        case LPAREN:
                            ++depth; // increase depth
                            ++curr;  // skip LPAREN
                            expr->push_back( compileExpression() );
                            break;
                            
                            
                        case RPAREN:
                            if(depth<=0) throw exception("%sextraneous '%c'",fn,RPAREN);
                            --depth; // come back
                            ++curr;  // skip RPAREN
                            goto RETURN;
                            
                            //--------------------------------------------------
                            //
                            // alternating
                            //
                            //--------------------------------------------------
                        case ALT: {
                            Y_RX_VERBOSE(std::cerr << fn << "[+] alt@" << depth << std::endl);
                            auto_ptr<Logical> alt = OR::Create();
                            ++curr; // skip ALT
                            alt->push_back( compileExpression() );
                            alt->push_front( expr.yield() );
                            Y_RX_VERBOSE(std::cerr << fn << "[-] alt@" << depth+1 << std::endl);
                            return alt.yield();
                        };
                            
                            //--------------------------------------------------
                            //
                            // DOT
                            //
                            //--------------------------------------------------
                        case DOT:
                            ++curr; // skip DOT;
                            expr->push_back( posix::dot() );
                            break;
                            
                            //--------------------------------------------------
                            //
                            // simple Joker
                            //
                            //--------------------------------------------------
                        case '+':
                            ++curr; //! skip '+'
                            expr->push_back( Repeating::OneOrMore( extractJoker(*expr,C) ) );
                            break;
                            
                        case '*':
                            ++curr; //! skip '*'
                            expr->push_back( Repeating::ZeroOrMore( extractJoker(*expr,C) ) );
                            break;
                            
                        case '?':
                            ++curr; //! skip '?'
                            expr->push_back( Optional::Create( extractJoker(*expr,C) ) );
                            break;
                            
                        case '~': {
                            ++curr; //! skip '~'
                            auto_ptr<Pattern> J = extractJoker(*expr,C);
                            auto_ptr<Logical> P = NONE::Create();
                            P->push_back( J.yield() );
                            expr->push_back( P.yield() );
                        } break;
                            
                            //--------------------------------------------------
                            //
                            // counting Joker
                            //
                            //--------------------------------------------------
                        case LBRACE:
                            compileCounting(*expr);
                            break;
                            
                            //--------------------------------------------------
                            //
                            // Escape Sequence
                            //
                            //--------------------------------------------------
                        case BACKSLASH:
                            ++curr; // skip BACKSLASH
                            expr->push_back( expressionESC() );
                            break;
                            
                            //--------------------------------------------------
                            //
                            // blocks
                            //
                            //--------------------------------------------------
                        case LBRACK:
                            ++depth;
                            expr->push_back( compileBlock() );
                            break;
                            
                            //--------------------------------------------------
                            //
                            // add single byte matching
                            //
                            //--------------------------------------------------
                        default:
                            Y_RX_VERBOSE(std::cerr << fn << "[+] '" << C << "'" << std::endl);
                            expr->add(C);
                            ++curr;
                            break;
                    }
                    
                }
                
            RETURN:
                Y_RX_VERBOSE(std::cerr << fn << "[-] expression@" << depth << std::endl);
                if(expr->size<=0) throw exception("%sempty expression in '%s'",fn,text);
                return Pattern::Optimize(expr.yield());
            }
            
            //------------------------------------------------------------------
            //
            // extract Joker
            //
            //------------------------------------------------------------------
            inline Pattern  *extractJoker( Logical &expr, const char jk )
            {
                Y_RX_VERBOSE(std::cerr << fn << "[+] joker <" << jk << ">@" << depth << std::endl);
                if(expr.size<=0) throw exception("%sno expression before '%c' in '%s'",fn,jk,text);
                return expr.pop_back();
            }
            
            //------------------------------------------------------------------
            //
            // create a Counting
            //
            //------------------------------------------------------------------
            inline void checkBraces(const Logical &expr, const string &data)
            {
                if(expr.size<=0) throw exception("%sno expression before {%s} in '%s'",fn,*data,text);
            }
            
            inline void compileCounting( Logical &expr )
            {
                static const char maxcount[] = "maximalCount";
                static const char mincount[] = "minimalCount";
                
                assert( LBRACE == curr[0] );
                //______________________________________________________________
                //
                // extracting brace content
                //______________________________________________________________
                const char *org = ++curr;
                {
                    while(curr<last&&RBRACE!=*curr)
                        ++curr;
                    if(curr>=last) throw exception("%sunfinished brace in '%s'",fn,text);
                }
                const string data(org,curr - org);
                Y_RX_VERBOSE(std::cerr << fn << "[+] counting <" << data << ">@" << depth << std::endl);
                
                
                
                //______________________________________________________________
                //
                // parsing joker
                //______________________________________________________________
                const char *ini = *data;
                const char *end = ini + data.size();
                const char *sep = strchr(ini, ',');
                if(sep)
                {
                    //__________________________________________________________
                    //
                    // with a separator
                    //__________________________________________________________
                    const string smin(ini,sep++-ini);if(smin.size()<=0) throw exception("%sinvalid {%s} in '%s'",fn,ini,text);
                    const size_t nmin = to_size(smin,mincount);
                    const string smax(sep,end-sep);
                    if(smax.size()>0)
                    {
                        // normal counting
                        const size_t nmax = to_size(smax,maxcount);
                        checkBraces(expr,data);
                        expr.push_back( Counting::Create(expr.pop_back(),nmin,nmax) );
                    }
                    else
                    {
                        // repeating
                        checkBraces(expr,data);
                        expr.push_back( Repeating::Create(expr.pop_back(),nmin) );
                    }
                    
                }
                else
                {
                    //__________________________________________________________
                    //
                    // without a separator
                    //__________________________________________________________
                    if(data.size()<=0) throw exception("%sinvalid empty braces in '%s'",fn,text);
                    const char   c = data[0];
                    if(c>='0'&&c<='9')
                    {
                        // new counting
                        const size_t n = to_size(data,"single [min|max]imalCount");
                        checkBraces(expr,data);
                        expr.push_back( Counting::Create(expr.pop_back(),n) );
                    }
                    else
                    {
                        // use dictionary
                        if(!dict) throw exception("%sno dictionary to find [%s] in '%s'",fn,*data,text);
                        const Motif *m = dict->search(data);
                        if(!m)    throw exception("%sno dictionary[%s] for '%s'",fn,*data,text);
                        expr.push_back( (**m).clone() );
                    }
                }
                ++curr; // skip RBRACE
            }
            
            static inline size_t to_size( const string &s, const char *when)
            {
                assert(when);
                size_t ans = 0;
                for(size_t i=0;i<s.size();++i)
                {
                    const char C = s[i];
                    if(C<'0'||C>'9') throw exception("%s invalid %s value '%s' ",fn,when,*s);
                    ans *= 10;
                    ans += C-'0';
                }
                return ans;
            }
            
            //------------------------------------------------------------------
            //
            // escape sequence for expression/block
            //
            //------------------------------------------------------------------
            static inline
            Pattern *TryControlESC(const char C)
            {
                switch(C)
                {
                    case 'n': return Single::Create('\n');
                    case 'r': return Single::Create('\r');
                    case 't': return Single::Create('\t');
                    case 'v': return Single::Create('\v');
                    case 'f': return Single::Create('\f');
                    default:
                        break;
                }
                return NULL;
            }
            
            inline
            Pattern *hexaESC()
            {
                assert('x'==curr[-1]);
                if(curr>=last)   throw exception("%smissing xdigits in '%s'",fn,text);
                const int hi = hexadecimal::to_decimal(*curr);
                if(hi<0)         throw exception("%sinvalid first xdigit '%c' in '%s'",fn,*curr,text);
                if(++curr>=last) throw exception("%sunfinished xdigits '%s'",fn,text);
                const int lo = hexadecimal::to_decimal(*curr);
                if(lo<0)         throw exception("%sinvalid second xdigit '%c' in '%s'",fn,*curr,text);
                const int code = hi*16+lo;
                ++curr;
                return Single::Create( uint8_t(code) );
            }
            
            inline
            Pattern *expressionESC()
            {
                const char C = *(curr++);
                
                {
                    Pattern   *ctrl = TryControlESC(C);
                    if(ctrl) return ctrl;
                }
                
                switch(C)
                {
                        // hexa:
                    case 'x':
                        return hexaESC();
                        
                        // direct
                    case BACKSLASH:
                    case LBRACE:
                    case RBRACE:
                    case LBRACK:
                    case RBRACK:
                    case ALT:
                    case DOT:
                    case TILDE:
                    case '+':
                    case '?':
                    case '*':
                        return Single::Create(C);
                        
                    default:
                        throw exception("%sunknown expression escape sequence '\\%c'",fn,C);
                }
            }
            
            
            //------------------------------------------------------------------
            //
            // Block parsing
            //
            //------------------------------------------------------------------
            inline
            Pattern *compileBlock()
            {
                Y_RX_VERBOSE(std::cerr << fn << "[+] block@" << depth << std::endl);
                assert( LBRACK == *curr );
                if(++curr>=last) throw exception("%searly unfinished block in '%s'",fn,text);
                auto_ptr<Logical> q = 0;
                
                //______________________________________________________________
                //
                // first char after LBRACK
                //______________________________________________________________
                switch(*curr)
                {
                    case CARET: q = NONE::Create(); ++curr; break;
                    case DASH:  q = OR::Create(); q->add(DASH); ++curr; break;
                    case COLON: --depth; return compilePOSIX();
                    default:    q = OR::Create();
                }
                
                //______________________________________________________________
                //
                // content
                //______________________________________________________________
                while(curr<last)
                {
                    const char C = *curr;
                    switch(C)
                    {
                            
                        case LBRACK:
                            ++depth;
                            q->push_back( compileBlock() );
                            break;
                            
                        case RBRACK:
                            --depth;
                            ++curr;   // skip RBRACK
                            if(q->size<=0) throw exception("%sempty block in '%s'",fn,text);
                        {
                            // first pass optimization
                            auto_ptr<Pattern> p = Pattern::Optimize(q.yield());
                            
                            // second pass optimization
                            switch(p->uuid)
                            {
                                case OR::  UUID: Pattern::PairwiseMerge( * p->as<OR>()   ); break;
                                case NONE::UUID: Pattern::PairwiseMerge( * p->as<NONE>() ); break;
                                default:
                                    break;
                            }
                            return Pattern::Optimize(p.yield());
                        }
                            
                        case BACKSLASH:
                            ++curr; // skip BACKSLASH
                            q->push_back( blockESC() );
                            break;
                            
                        case DASH:
                            ++curr; // skip DASH
                            compileRange(*q);
                            break;
                            
                        default:
                            q->add(C);
                            ++curr;
                    }
                }
                throw exception("%sunfinished block in '%s'",fn,text);
            }
            
            //------------------------------------------------------------------
            //
            // range parsing
            //
            //------------------------------------------------------------------
            inline
            void compileRange( Logical &q )
            {
                static const char sub[] = " Range: ";
                assert(DASH==curr[-1]);
                // check if enough chars
                if(curr>=last)
                    throw exception("%s%sunfinished block range in '%s'",fn,sub,text);
                
                // check LHS
                if( (q.size<=0) || q.tail->uuid != Single::UUID )
                    throw exception("%s%smissing single char before '-' in '%s'",fn,sub,text);
                const auto_ptr<Pattern> lhs = q.pop_back();
               
                // check RHS
                const char        C   = *(curr++);
                auto_ptr<Pattern> rhs = 0;
                switch(C)
                {
                    case LBRACK:
                    case RBRACK:
                        throw exception("%s%sinvalid '%c' after '-' in '%s'",fn,sub,C,text);
                        
                    case BACKSLASH:
                        rhs = blockESC();
                        break;
                        
                    default:
                        rhs = Single::Create(C);
                }
                assert(rhs.is_valid());
                assert(Single::UUID==rhs->uuid);
                q.add( lhs->as<Single>()->code, rhs->as<Single>()->code );
            }
            
            //------------------------------------------------------------------
            //
            // block escape sequence
            //
            //------------------------------------------------------------------
            inline
            Pattern *blockESC()
            {
                const char C = *(curr++);
                
                {
                    Pattern   *ctrl = TryControlESC(C);
                    if(ctrl) return ctrl;
                }
                
                switch(C)
                {
                        // hexa:
                    case 'x':
                        return hexaESC();
                        
                        // direct
                    case BACKSLASH:
                    case LBRACK:
                    case RBRACK:
                    case DASH:
                    case COLON:
                        return Single::Create(C);
                        
                    default:
                        throw exception("%sunknown block escape sequence '\\%c'",fn,C);
                }
            }
            
            //------------------------------------------------------------------
            //
            // POSIX parsing
            //
            //------------------------------------------------------------------
            inline
            Pattern *compilePOSIX()
            {
                //______________________________________________________________
                //
                // setup name origin
                //______________________________________________________________
                assert(COLON==*curr);
                const char *org = ++curr;
                if(curr>=last) throw exception("%sunfinished POSIX in '%s'",fn,text);
               
                //______________________________________________________________
                //
                // look up for end
                //______________________________________________________________
                while(curr+1<last)
                {
                    if(COLON==curr[0]&&RBRACK==curr[1])
                    {
                        const string id(org,curr-org);
                        Y_RX_VERBOSE(std::cerr << fn << "[+] POSIX :" << id << ":" << std::endl);
                        Pattern *p = posix::query(id);
                        if(!p) throw exception("%sunknown posix [:%s:]",fn,*id);
                        curr += 2;
                        return p;
                    }
                    ++curr;
                }
                throw exception("%sunfinished POSIX name in '%s'",fn,text);
            }
            
            //------------------------------------------------------------------
            //
            // Entry point
            //
            //------------------------------------------------------------------
            static inline
            Pattern *Compile(const string &rx, const Dictionary *dict)
            {
                Engine            engine(*rx,rx.size(),dict);
                auto_ptr<Pattern> result =  engine.compileExpression();
                if( engine.depth != 0) throw exception("%sunfinished '%s'",fn,*rx);
                return result.yield();
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Engine);
        };
        
        
        
    }
    
}

namespace upsylon {
    
    namespace Jargon {
        
       

        Pattern * RegularExpression::Compile(const string      &rx,
                                             const Dictionary *dict)
        {
            return  RegularExpression::Engine::Compile(rx,dict);
        }
        
        
        
        Pattern * RegularExpression::Compile(const char       *rx,
                                             const Dictionary *dict)
        {
            const  string _(rx);
            return Compile(_,dict);
        }
        
        Pattern * RegularExpression::Compile(const char       C,
                                             const Dictionary *)
        {
            return Single::Create(C);
        }
        
    }
    
}
