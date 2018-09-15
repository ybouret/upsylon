
#include "y/lang/lexical/plugin/strings.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {

            __String:: ~ __String() throw()
            {
            }


            __String:: __String(Translator &tr, const string &id, const char *ch) : Plugin(tr,id,ch), content()
            {

                discard("char", "[:word:]", this, & __String::Collect );

                back(trigger,this,& __String::Forward);
            }

            void __String:: Init( const Token &t )
            {
                std::cerr << "enter " << label << std::endl;
                content.clear();
                Collect(t);
            }

            void __String:: Collect(const Token &t)
            {
                content.merge_back_copy(t);
            }


        }
    }
}


namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {

            jString:: ~jString() throw()
            {
            }


            jString:: jString(Translator &t, const string &id) : __String(t, id,"\\x22")
            {

            }


        }
    }
}

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {

            rString:: ~rString() throw()
            {
            }


            rString:: rString(Translator &t, const string &id) : __String(t, id,"\\x27")
            {

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
            void __String:: Forward(const Token &t)
            {
                Collect(t);
                std::cerr << "leave " << label << " with '" << content << "'" << std::endl;
                Lexeme *lx = new Lexeme(label);
                lx->swap_with(content);
                lexer.unget(lx);
            }
        }
    }
}


