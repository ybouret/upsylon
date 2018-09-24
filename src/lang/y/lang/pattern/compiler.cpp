
#include "y/lang/pattern/compiler.hpp"
#include "y/lang/pattern/logic.hpp"
#include "y/lang/pattern/basic.hpp"
#include "y/lang/pattern/joker.hpp"
#include "y/lang/pattern/posix.hpp"

#include "y/exception.hpp"
#include "y/code/utils.hpp"
#include "y/string/convert.hpp"

namespace upsylon
{
    namespace Lang
    {

        static const char fn[] = "Lang::RegEx: ";

#define LPAREN     '('
#define RPAREN     ')'
#define ALT        '|'
#define BACKSLASH  '\\'
#define LBRACK     '['
#define RBRACK     ']'
#define DASH       '-'
#define LBRACE     '{'
#define RBRACE     '}'

        class RXCompiler
        {
        public:
            const char       *curr;
            const char       *last;
            int               depth;
            const Dictionary *dict;

            inline ~RXCompiler() throw() {}

            //__________________________________________________________________
            //
            // initialize the compiler
            //__________________________________________________________________
            inline  RXCompiler(const string &rx, const Dictionary *user_dict) :
            curr( *rx ),
            last( curr+rx.size() ),
            depth(0),
            dict( user_dict      )
            {
            }

            //__________________________________________________________________
            //
            //
            //! compile sub expression from curr
            //
            //__________________________________________________________________
            inline Pattern *sub()
            {
                //______________________________________________________________
                //
                // initialize Logical AND
                //______________________________________________________________
                auto_ptr<Logical> p = new AND();

                //______________________________________________________________
                //
                // loop over curr
                //______________________________________________________________
                while( curr != last )
                {
                    const char C = *curr;

                    switch(C)
                    {
                            //--------------------------------------------------
                            // grouping
                            //--------------------------------------------------
                        case LPAREN:
                            ++depth;
                            ++curr; // skip LPAREN
                            p->add( sub() );
                            break;

                        case RPAREN:
                            if(depth<=0) throw exception("%stoo many '%c'",fn,RPAREN);
                            --depth; // keep track
                            ++curr;  // skip RPAREN
                            goto END_OF_SUB;

                            //--------------------------------------------------
                            // alternation
                            //--------------------------------------------------
                        case ALT: {
                            if(p->operands.size<=0) throw exception("%sempty sub-expression before '%c'",fn,ALT);
                            auto_ptr<Logical> alt = new OR();
                            alt->add( p.yield() );
                            ++curr; // skip ALT
                            alt->add( sub() );
                            return Pattern::Optimize(alt.yield());
                        }

                            //--------------------------------------------------
                            // simple joker
                            //--------------------------------------------------
                        case '+':
                        case '*':
                        case '?':
                            if(p->size()<=0) throw exception("%sno sub-expression before '%c'",fn,C);
                            p->add( simpleJoker(C,p->remove() ) );
                            ++curr; // skip joker sign
                            break;

                            //--------------------------------------------------
                            // braced expression
                            //--------------------------------------------------
                        case LBRACE:
                            //if(p->operands.size<=0) throw exception("%sno sub-expression before '%c'",fn,C);
                            buildBraced(p);
                            break;

                            //--------------------------------------------------
                            // special characters
                            //--------------------------------------------------
                        case '.':
                            p->add( posix::dot() );
                            ++curr;
                            break;

                            //--------------------------------------------------
                            // escape sequence
                            //--------------------------------------------------
                        case BACKSLASH:
                            ++curr; // skip BACKSLASH
                            p->add( subEscape() );
                            break;

                            //--------------------------------------------------
                            // a new group!
                            //--------------------------------------------------
                        case LBRACK:
                            ++curr; // skip LBRACK
                            p->add( grp() );
                            break;

                        default:
                            p->add( new Single(C) );
                            ++curr;
                    }
                }

                //______________________________________________________________
                //
                // sanity check
                //______________________________________________________________
            END_OF_SUB:
                if(p->operands.size<=0) throw exception("%sempty sub-expression",fn);
                return Pattern::Optimize(p.yield());
            }

            //__________________________________________________________________
            //
            // making a simple joker
            //__________________________________________________________________
            inline Pattern *simpleJoker(const int type,Pattern *jk)
            {
                switch(type)
                {
                    case '?': return Optional ::Create(jk);
                    case '+': return Repeating::OneOrMore(jk);
                    case '*': return Repeating::ZeroOrMore(jk);
                    default:
                        break;
                }
                throw exception("%sinvalid simple joker type!!!",fn);
            }

            //__________________________________________________________________
            //
            //
            // bulding from a braced expression
            //
            //__________________________________________________________________
            inline void buildBraced( auto_ptr<Logical> &p )
            {
                //______________________________________________________________
                //
                // initialized
                //______________________________________________________________
                assert(LBRACE==curr[0]);
                const char *ini = curr;

                //______________________________________________________________
                //
                // look for end
                //______________________________________________________________
                do
                {
                    if(++curr>=last) throw exception("%sunfinished braced expression",fn);
                }
                while(RBRACE != *curr);
                const char *end = curr;

                //______________________________________________________________
                //
                // extract content
                //______________________________________________________________
                assert(end>ini);
                assert(LBRACE==*ini);
                assert(RBRACE==*end);
                ++ini;
                string content(ini,end-ini);
                if(content.size()<=0) throw exception("%sempty braces",fn);

                //______________________________________________________________
                //
                // analyze
                //______________________________________________________________
                const char choice = content[0];
                if(isalpha(choice))
                {
                    insertEntry(p,content);
                }
                else if( isdigit(choice) || ',' == choice )
                {
                    insertJoker(p,content);
                }
                else
                {
                    throw exception("%sinvalid content char '%s'",fn,visible_char[ uint8_t(content[0]) ]);
                }
                ++curr;
            }

            //__________________________________________________________________
            //
            //
            // insert a directory entry
            //
            //__________________________________________________________________
            inline void insertEntry( auto_ptr<Logical> &p, const string &content )
            {
                if(!dict) throw exception("%smissing dictionary for '%s'",fn,*content);
                p->add( dict->create(content) );
            }

            //__________________________________________________________________
            //
            //
            // insert a counting joker
            //
            //__________________________________________________________________

            inline void insertJoker( auto_ptr<Logical> &p,  string &content )
            {
                //______________________________________________________________
                //
                // check status
                //______________________________________________________________
                if(p->size()<=0) throw exception("%sno sub-expression before {%s}",fn,*content);

                //______________________________________________________________
                //
                // analyze
                //______________________________________________________________
                char  *smin = *content;
                char  *smax = (char *)strchr(smin,',');
                size_t lmax = 0;
                size_t nmax = 0;

                if( smax )
                {
                    *(smax++)=0;
                    lmax = length_of(smax);
                    std::cerr << "smax=" << smax << "/#" << lmax << std::endl;
                    if(lmax>0)
                    {
                        nmax = string_convert::to<size_t>(smax,"Lang::RegEx::Range.nmax");
                        std::cerr << "nmax=" << nmax << std::endl;
                    }
                }

                const size_t lmin = length_of(smin);
                std::cerr << "smin=" << smin << "/#" << lmin << std::endl;

                //______________________________________________________________
                //
                // process
                //______________________________________________________________
                if(lmin>0)
                {
                    // smin has a value
                    const size_t nmin = string_convert::to<size_t>(smin,"Lang::RegEx::Range.nmin");
                    std::cerr << "nmin=" << nmin << std::endl;
                    if(lmax>0)
                    {
                        p->add( Counting::Create(p->remove(),nmin,nmax) );
                    }
                    else
                    {
                        p->add( Repeating::Create(p->remove(),nmin) );
                    }
                }
                else
                {
                    // empty smin -> nmin = 0
                    assert(lmin<=0);
                    if(lmax<=0)
                    {
                        auto_ptr<Logical> q = new NONE();
                        q->add( p->remove() );
                        p->add( q.yield() );
                    }
                    else
                    {
                        p->add( Counting::Create(p->remove(),0,nmax) );
                    }
                }
            }

            //__________________________________________________________________
            //
            //
            // escape sequence in sub-expression
            //
            //__________________________________________________________________
#define Y_RX_ESC_CONTROL() \
case 'n': return new Single('\n');\
case 'r': return new Single('\r');\
case 't': return new Single('\t');\
case 'v': return new Single('\v');\
case 'f': return new Single('\f')

            inline Pattern * subEscape()
            {
                assert(BACKSLASH==curr[-1]);
                if(curr>=last) throw exception("%smissing sub-expression escape sequence",fn);
                const char C = *(curr++);
                switch(C)
                {
                        // direct sequence
                    case '\\':
                    case '.':
                    case '(':
                    case ')':
                    case '[':
                    case ']':
                    case '{':
                    case '}':
                    case '+':
                    case '*':
                    case '?':
                    case '"':
                    case '\'':
                        return new Single(C);

                        // interpreted
                        Y_RX_ESC_CONTROL();

                    case 'x':
                        return hexEscape();

                    default:
                        break;

                }
                throw exception("%sunknown sub-expression escape sequence '%s'",fn,visible_char[uint8_t(C)]);
            }

            //__________________________________________________________________
            //
            //
            // parse hexadecimal
            //
            //__________________________________________________________________
            inline Pattern *hexEscape()
            {
                assert('x'==curr[-1]);
                // read 2 chars
                if(curr>=last) throw exception("%smissing first hexadecimal char",fn);
                const char hiCH = *(curr++);
                if(curr>=last) throw exception("%smissing second hexadecimal char",fn);
                const char loCH = *(curr++);

                const int hi = hexadecimal::to_decimal(hiCH); if(hi<0) throw exception("%sinvalid first hexadecimal char '%s'",fn,visible_char[uint8_t(hiCH)]);
                const int lo = hexadecimal::to_decimal(loCH); if(lo<0) throw exception("%sinvalid second hexadecimal char '%s'",fn,visible_char[uint8_t(loCH)]);

                const uint8_t B = uint8_t(hi) << 4 | uint8_t(lo);
                return new Single(B);
            }

            //__________________________________________________________________
            //
            //
            // parse group
            //
            //__________________________________________________________________
            inline Pattern *grp()
            {
                assert(curr[-1]==LBRACK);
                if(curr>=last)
                {
                    throw exception("%smissing group",fn);
                }
                //______________________________________________________________
                //
                // analyze the first char
                //______________________________________________________________
                auto_ptr<Logical> g = NULL;
                {
                    const char C = *curr;
                    switch(C)
                    {
                        case ':':
                            return posix();

                        case '^':
                            ++curr; // skip carret
                            g = new NONE();
                            break;

                        case DASH:
                            ++curr;
                            g = new OR();
                            g->add( new Single(DASH) );
                            break;

                        default:
                            g = new OR();
                            break;
                    }
                }
                assert( g.is_valid() );

                //______________________________________________________________
                //
                // run
                //______________________________________________________________
                while(curr<last)
                {
                    const char C = *curr;
                    switch(C)
                    {
                            //--------------------------------------------------
                            // End Of Group
                            //--------------------------------------------------
                        case RBRACK:
                            ++curr;
                            if(g->size()<=0) throw exception("%sempty group",fn);
                            return Pattern::Optimize( g.yield() );

                            //--------------------------------------------------
                            // recursive call
                            //--------------------------------------------------
                        case LBRACK:
                            ++curr;
                            g->add( grp() );
                            break;

                            //--------------------------------------------------
                            // escape sequence
                            //--------------------------------------------------
                        case BACKSLASH:
                            ++curr; // skip BACKSLASH
                            g->add( grpEscape() );
                            break;

                            //--------------------------------------------------
                            // making a range
                            //--------------------------------------------------
                        case DASH: makeRange( g ); break;

                        default:
                            ++curr;
                            g->add( new Single(C) );
                    }

                }
                throw exception("%sunfinished group",fn);
            }

            //__________________________________________________________________
            //
            //
            // identify a posix alias
            //
            //__________________________________________________________________
#define Y_RX_POSIX(LABEL) if(label==#LABEL) return posix:: LABEL()

            inline Pattern *posix()
            {
                assert(':'==*curr);
                const char *ini = curr;
                do
                {
                    ++curr;
                    if(curr>=last) throw exception("%sunfinished posix label",fn);
                }
                while( ':' != *curr );
                const char *end = curr;
                if(++curr>=last)  throw exception("%send of expression after ':'",fn);
                if(RBRACK!=*curr) throw exception("%sinvalid '%s' after ':'", fn, visible_char[ uint8_t(*curr) ]);
                ++curr; // skip RBRACK
                assert(end>ini);
                assert(*ini==':');
                assert(*end==':');
                ++ini;
                const string label(ini,end-ini);
                
                Y_RX_POSIX(lower);
                Y_RX_POSIX(upper);
                Y_RX_POSIX(alpha);
                Y_RX_POSIX(digit);
                Y_RX_POSIX(alnum);
                Y_RX_POSIX(xdigit);
                Y_RX_POSIX(blank);
                Y_RX_POSIX(space);
                Y_RX_POSIX(punct);
                Y_RX_POSIX(word);
                Y_RX_POSIX(endl);
                Y_RX_POSIX(dot);
                Y_RX_POSIX(core);


                throw exception("%sunknown posix label '%s'", fn, *label );

            }


            //__________________________________________________________________
            //
            //
            // escape sequence in sub-expression
            //
            //__________________________________________________________________
            inline Pattern * grpEscape()
            {
                assert(BACKSLASH==curr[-1]);
                if(curr>=last) throw exception("%smissing group escape sequence",fn);
                const char C = *(curr++);
                switch(C)
                {
                        // direct sequence
                    case '\\':
                    case '[':
                    case ']':
                    case '-':
                    case '^':
                    case '"':
                    case '\'':
                        return new Single(C);

                        // interpreted
                        Y_RX_ESC_CONTROL();

                    case 'x':
                        return hexEscape();

                    default:
                        break;

                }
                throw exception("%sunknown group escape sequence '%s'",fn,visible_char[uint8_t(C)]);
            }

            //__________________________________________________________________
            //
            //
            // make a range
            //
            //__________________________________________________________________
            inline void makeRange( auto_ptr<Logical> &g )
            {
                assert(DASH==*curr);
                //______________________________________________________________
                //
                // check lhs
                //______________________________________________________________
                if(g->operands.size<=0) throw exception("%sno left argument for range",fn);
                auto_ptr<Pattern> lhs = g->remove();
                if( Single::UUID != lhs->uuid) throw exception("%sno single char before dash",fn);
                const uint8_t lo = static_cast<Single *>(lhs->priv)->code;

                //______________________________________________________________
                //
                // find rhs
                //______________________________________________________________
                if(++curr>=last)
                {
                    throw exception("%sunfinished range",fn);
                }
                const char C  = *(curr++); // skip current char
                uint8_t    up = uint8_t(C);
                switch(C)
                {
                    case RBRACK:
                    case LBRACK:
                        throw exception("%sinvalid upper value for range='%s'",fn,visible_char[ up ]);

                    case BACKSLASH: {
                        auto_ptr<Pattern> rhs = grpEscape(); assert( Single::UUID==rhs->uuid);
                        up = static_cast<Single *>(rhs->priv)->code;
                    } break;

                    default:
                        break;
                }

                //______________________________________________________________
                //
                // append range
                //______________________________________________________________
                g->add(new Range(lo,up));
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(RXCompiler);
        };

        Pattern * RegExp( const string &rx, const Dictionary *dict )
        {
            RXCompiler        cmp(rx,dict);
            auto_ptr<Pattern> p = cmp.sub();
            if(cmp.depth>0)
            {
                throw exception("%sunfinished sub-expression",fn);
            }
            return p.yield();
        }

    }
}
