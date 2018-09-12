
#include "y/lang/lexical/translator.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {

            Translator:: Translator(const string &id) :
            name( new string(id) ),
            curr(0),
            base(0),
            scanners(4,as_capacity)
            {
                setup();
            }

            Translator:: ~Translator() throw()
            {
            }

            void Translator::setup()
            {
                base = new Scanner(name);
                const Scanner::Pointer p(base);
                if(!scanners.insert(p))
                {
                    throw exception("unable to insert base scanner [%s]", **(p->label));
                }
            }

            Scanner & Translator:: decl(const string &id)
            {
                assert(base);
                Scanner::Pointer p = new Scanner(id);
                if(!scanners.insert(p))
                {
                    throw exception("[%s] multiple scanner [%s]", **name, *id);
                }
                return *p;
            }


        }
    }
}

