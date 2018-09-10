
#include "y/lang/pattern/compiler.hpp"
#include "y/lang/pattern/logic.hpp"
#include "y/lang/pattern/basic.hpp"
#include "y/lang/pattern/joker.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {


        class Compiler
        {
        public:
            const char       *curr;
            const char       *last;
            const Dictionary *dict;
            inline ~Compiler() throw() {}

            //__________________________________________________________________
            //
            // initialize the compiler
            //__________________________________________________________________
            inline  Compiler(const string &rx, const Dictionary *user_dict) :
            curr( *rx ),
            last( curr+rx.size() ),
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
                            
                        default:
                            p->add( new Single(C) );
                            ++curr;
                    }
                }

                //______________________________________________________________
                //
                // sanity check
                //______________________________________________________________
                if(p->operands.size<=0)
                {
                    throw exception("Lang::RegEx(empty sub-expression)");
                }

                return p.yield();
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
        };

        Pattern * Compile::RegEx( const string &rx, const Dictionary *dict )
        {

            Compiler cmp(rx,dict);
            auto_ptr<Pattern> p = cmp.sub();
            return p.yield();
        }

    }
}
