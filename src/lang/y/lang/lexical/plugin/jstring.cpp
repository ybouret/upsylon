
#include "y/lang/lexical/plugin/jstring.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {

            jString:: ~jString() throw()
            {
            }

            static const char __quote[] = "\\x22";

            jString:: jString(Translator &t, const string &id) : Plugin(t, id,__quote), content()
            {

                discard("char", "[:word:]", this, & jString::Collect );

                back(__quote,this,&jString::Finish);
            }

            void jString:: Init( const Token &t )
            {
                std::cerr << "enter jString" << std::endl;
                content.clear();
                Collect(t);
            }

            void jString:: Collect(const Token &t)
            {
                content.merge_back_copy(t);
            }


            

        }
    }
}

#include "y/lang/lexical/translator.hpp"
namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
            void jString:: Finish(const Token &t)
            {
                Collect(t);
                std::cerr << "leave jString with '" << content << "'" << std::endl;
                Lexeme *lx = new Lexeme(label);
                lx->swap_with(content);
                lexer.unget(lx);
            }
        }
    }
}


