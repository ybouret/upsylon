
#include "y/jive/lexical/plugin/strings.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {

            String_:: ~String_() throw()
            {
                aliasing::_(delimiter)=0;
            }

            
            void String_:: setup()
            {
                back(delimiter,this,&String_::OnQuit);
                discard("char", "[:core:]", this, &String_::OnCore);
                {
                    const char rx[4] = { '\\', '\\', delimiter,0};
                    discard("dlm",rx,this, &String_::OnDelim);
                }
                discard("hexa", "\\\\x[:xdigit:][:xdigit:]",this,&String_::OnHexa);
                discard("herr1", "\\\\x[^[:xdigit:]][^]?",this,&String_::OnHerr);
                discard("herr2", "\\\\x[:xdigit:][^[:xdigit:]]",this,&String_::OnHerr);
                discard("esc",  "\\\\[^]",this,&String_::OnEsc);
                discard("error","[^]",this,&String_::OnError);
                
            }

            void String_:: OnInit(const Token &)
            {
                s.release();
            };

            void String_:: OnQuit(const Token &)
            {
                Unit *unit = new Unit(*(s.head),label);
                unit->swap_with(s);
                Q.push(unit);
            }

            void String_:: OnCore(const Token &t)
            {
                s << t;
            }

            void String_:: OnDelim(const Token &t)
            {
                assert(t.size==2);
                Char *ch = Char::Copycat(*t.tail);
                s << ch;
            }


            void String_:: OnError(const Token &t)
            {
                assert(t.size>0);
                exception   excp;
                t.head->cat(excp);                  // context
                excp.cat("[%s] invalid ",**label);  // message
                t.cat(excp);                        // token
                throw excp;
            }
            
            void String_:: OnHerr(const Token &t)
            {
                assert(t.size>0);
                exception   excp;
                t.head->cat(excp);                  // context
                excp.cat("[%s] invalid hexadecimal sequence ",**label);  // message
                t.cat(excp);                        // token
                throw excp;
            }
            
            void String_:: OnHexa(const Token &t)
            {
                assert(4==t.size);
                const int  lo = hexadecimal::to_decimal(t.tail->code);
                const int  hi = hexadecimal::to_decimal(t.tail->prev->code);
                const int  ch = (hi<<4) | lo;
                s << Char::Copyset(*t.head, ch);
            }

#define YLP_ESC(A,B) case A : s << Char::Copyset(*t.head,B); break

            void String_:: OnEsc(const Token &t)
            {
                assert(2==t.size);
                const uint8_t C = t.tail->code;
                switch(C)
                {
                    case '\\':
                    case '"':
                    case '\'':
                        s << Char::Copyset(*t.head,C);
                        break;

                        YLP_ESC('n','\n');
                        YLP_ESC('r','\r');
                        YLP_ESC('t','\t');


                    default:
                        OnError(t);
                }
            }

            jString:: ~jString() throw()
            {
            }

            rString:: ~rString() throw()
            {
            }

        }

    }
}

