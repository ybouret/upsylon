
#include "y/lang/pattern/compiler.hpp"
#include "y/lang/pattern/logic.hpp"
#include "y/lang/pattern/basic.hpp"
#include "y/lang/pattern/joker.hpp"
#include "y/lang/pattern/posix.hpp"

#include "y/exception.hpp"
#include "y/code/utils.hpp"

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
        
        class Compiler
        {
        public:
            const char       *curr;
            const char       *last;
            int               depth;
            const Dictionary *dict;

            inline ~Compiler() throw() {}

            //__________________________________________________________________
            //
            // initialize the compiler
            //__________________________________________________________________
            inline  Compiler(const string &rx, const Dictionary *user_dict) :
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
                            if(p->operands.size<=0) throw exception("%sno sub-expression before '%c'",fn,C);
                            p->add( simpleJoker(C,p->operands.pop_back() ) );
                            ++curr; // skip joker sign
                            break;

                            //--------------------------------------------------
                            // special characters
                            //--------------------------------------------------
                        case '.':
                            p->add( Posix::dot() );
                            ++curr;
                            break;

                            //--------------------------------------------------
                            // escape sequence
                            //--------------------------------------------------
                        case BACKSLASH:
                            ++curr; // skip backslash
                            p->add( subEscape() );
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
            // escape sequence in sub-expression
            //__________________________________________________________________
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
                        return new Single(C);

                        // interpreted
                    case 'n': return new Single('\n');
                    case 'r': return new Single('\r');
                    case 't': return new Single('\t');
                    case 'v': return new Single('\v');
                    case 'f': return new Single('\f');

                    case 'x':
                        return hexEscape();

                    default:
                        break;

                }
                throw exception("%sunknown sub-expression escape sequence '%s'",fn,visible_char[uint8_t(C)]);
            }

            //__________________________________________________________________
            //
            // parse hexadecimal
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

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
        };

        Pattern * Compile::RegEx( const string &rx, const Dictionary *dict )
        {

            Compiler cmp(rx,dict);
            auto_ptr<Pattern> p = cmp.sub();
            if(cmp.depth>0)
            {
                throw exception("%sunfinished sub-expression",fn);
            }
            return p.yield();
        }

    }
}
