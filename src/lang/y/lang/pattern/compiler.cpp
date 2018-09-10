
#include "y/lang/pattern/compiler.hpp"
#include "y/lang/pattern/logic.hpp"
#include "y/lang/pattern/basic.hpp"
#include "y/lang/pattern/joker.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {

        static const char fn[] = "Lang::RegEx: ";

#define LPAREN '('
#define RPAREN ')'
#define ALT    '|'

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
            Pattern *sub()
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
                            return Pattern::Optimize(alt.yield()); // both sides are optimized
                        }

                            //--------------------------------------------------
                            // joker
                            //--------------------------------------------------
                        case '+':
                        case '*':
                        case '?':
                            if(p->operands.size<=0) throw exception("%sno sub-expression before '%c'",fn,C);
                            p->add( simpleJoker(C,p->operands.pop_back() ) );
                            ++curr; // skip joker sign
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

            Pattern *simpleJoker(const int type,Pattern *jk)
            {
                switch(type)
                {
                    case '?': return Optional::Create(jk);
                    case '+': return Repeating::OneOrMore(jk);
                    case '*': return Repeating::ZeroOrMore(jk);

                    default:
                        break;
                }
                throw exception("%sinvalid simple joker type!!!",fn);
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
