
#include "y/jargon/lexical/plugin/strings.hpp"
#include "y/jargon/lexical/analyzer.hpp"

#include "y/exception.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {
            
            
            Strings:: ~Strings() throw()
            {}
            
            void Strings:: setup()
            {
                discard("core", "[:core:]", this, &Strings::onCore);
                back(*trigger,this, & Strings::onEmit );
                discard("esc0", "\\\\[nrtfvb]",   this, &Strings::onEsc0);
                discard("esc1", "\\\\[\"'\\\\/]", this, &Strings::onEsc1);
                discard("hexa", "\\\\x[:xdigit:][:xdigit:]", this, &Strings::onHexa);
                discard("any","[\\x00-\\xff]",this,&Strings::failed);
            }
            
            
            void Strings:: checkSymbol(const Token &sep, const char *fn) const
            {
                if(sep.size!=1)
                    throw exception("%sinvalid token.size=%u", fn,unsigned(sep.size) );
                
                if(char(sep.head->code) != symbol)
                    throw exception("%sexpecting '%c' instead of '%s'", fn,symbol,cchars::encoded[sep.head->code] );
                
            }

            
            void Strings:: onInit(const Token &sep)
            {
                static const char fn[] = "Jargon::Strings::onInit: ";
                content = 0;
                context = 0;
                checkSymbol(sep,fn);
                context = new Context( *(sep.head) );
                content = new string();
            }
            
            void Strings:: onCore(const Token &t)
            {
                assert( content.is_valid() );
                assert( context.is_valid() );
                for(const Char *ch=t.head;ch;ch=ch->next)
                {
                    *content << char(ch->code);
                }
            }
            
            void Strings:: onEsc0(const Token &esc)
            {
                assert(content.is_valid());
                assert(2==esc.size);
                assert('\\'==esc.head->code);
                const char C = esc.tail->code;
                switch(C)
                {
                    case 'n': *content << '\n'; break;
                    case 'r': *content << '\r'; break;
                    case 't': *content << '\t'; break;
                    case 'v': *content << '\v'; break;
                    case 'f': *content << '\f'; break;
                    case 'b': *content << '\b'; break;
                    default : throw exception("%s: corrupted escape sequence code!",**label);
                }
            }
            
            void Strings:: onEsc1(const Token &esc)
            {
                assert(content.is_valid());
                assert(2==esc.size);
                assert('\\'==esc.head->code);
                const char C = esc.tail->code;
                switch(C)
                {
                    case '"':
                    case '\'':
                    case '\\':
                    case '/':
                        *content << C;
                        break;
                    default : throw exception("%s: corrupted escaped character!", **label);
                }
            }
            
            void Strings:: onHexa(const Token &hexa)
            {
                assert(4==hexa.size);
                assert(content.is_valid());
                
                const Char *ch = hexa.head; assert('\\'==ch->code);
                ch=ch->next;                assert('x' ==ch->code);
                ch=ch->next; const int hi = hexadecimal::to_decimal(ch->code); assert(hi>=0);
                ch=ch->next; const int lo = hexadecimal::to_decimal(ch->code); assert(lo>=0);
                const uint8_t code = hi*16+lo;
                *content << char(code);
            }
            
            void Strings:: onEmit(const Token &sep)
            {
                
                static const char fn[] = "Jargon::Strings::onEmit: ";
                assert( content.is_valid() );
                assert( context.is_valid() );
                checkSymbol(sep,fn);
                
                auto_ptr<Unit> unit = new Unit(sep.cache,*context,label);
                unit->append(*content);
                
                content = 0;
                context = 0;
                lexer.unget( unit.yield() );
            }
            
            void Strings:: failed(const Token &t)
            {
                assert(t.size>=1);
                const char    bad = t.head->code;
                const Context &ctx = *(t.head);
                
                if('\\'==bad)
                {
                    throw exception("%s:%d:%d: %s unfinished sequence after '%s\\'",
                                    **ctx.tag, ctx.line, ctx.column,
                                    **label, **content);
                }
                else
                {
                    throw exception("%s:%d:%d: %s invalid char '%s' after '%s'",
                                    **ctx.tag, ctx.line, ctx.column,
                                    **label, cchars::encoded[uint8_t(bad)], **content);
                }
            }
        }
        
    }
}


