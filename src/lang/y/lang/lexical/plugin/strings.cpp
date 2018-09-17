
#include "y/lang/lexical/plugin/strings.hpp"
#include "y/lang/lexical/translator.hpp"
#include "y/exception.hpp"
#include "y/code/utils.hpp"

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
                assert(*label==id);
                const string esc_id = *label + ".escape";
                const string esc_rx = "\\x5C";
                Scanner     &esc = tr.decl(esc_id);

                const string hex_id = *label + ".hexadecimal";
                const string hex_rx = "\\x5Cx";
                Scanner     &hex = tr.decl(hex_id);

                // core collect
                discard("core", "[:core:]", this, & __String::Collect );
                call(hex_id,hex_rx);
                call(esc_id,esc_rx);
                back(trigger,this,& __String::Forward);

                // esc collect
                esc.back("[\\x22\\x27\\x5C/]", this, & __String::EscCopy);
                esc.back("[nrtbf]", this, & __String::EscCode);

                // hex collect
                hex.back("[:xdigit:][:xdigit:]", this, & __String::EscHexa);

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

            void __String:: Forward(const Token &t)
            {
                Collect(t);
                //std::cerr << "leave " << label << " with '" << content << "'" << std::endl;
                Lexeme *lx = new Lexeme(label);
                lx->swap_with(content);
                lexer.unget(lx);
            }

            void __String:: EscCopy(const Token &t)
            {
                Collect(t);
            }


            void __String:: EscHexa(const Token &t)
            {
                assert(2==t.size);
                const Char &upCH = *(t.head);
                const Char &loCH = *(t.tail);
                const uint8_t code = (uint8_t(hexadecimal::to_decimal(upCH.code)) << 4) | uint8_t(hexadecimal::to_decimal(loCH.code));
                content.push_back( new Char(code,upCH) );
            }

#define Y_STRCODE(LABEL,CHAR) case LABEL: content.push_back( new Char(CHAR,ch) ); break

            void __String:: EscCode(const Token &t)
            {
                assert(1==t.size);
                const Char &ch = *(t.head);
                switch(ch.code)
                {
                        Y_STRCODE('n','\n');
                        Y_STRCODE('r','\r');
                        Y_STRCODE('t','\t');
                        Y_STRCODE('b','\b');
                        Y_STRCODE('f','\f');
                    default:
                        throw exception("__String::EscCode(corrupted)");
                }
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


