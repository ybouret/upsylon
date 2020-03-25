
#include "y/jargon/lexical/plugin/strings.hpp"
#include "y/jargon/lexical/analyzer.hpp"

#include "y/exception.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {
        
            const char *Strings:: RegExp( const Separator symbol ) throw()
            {
                switch(symbol)
                {
                    case DoubleQuote: return "\"";
                    case SimpleQuote: return "\'";
                }
            }
            
            Strings:: ~Strings() throw()
            {}
            
            void Strings:: setup()
            {
                discard("core", "[:core:]", this, &Strings::onCore);
                back( RegExp(symbol),this, & Strings::onEmit );
            }
            
            
            void Strings:: checkSymbol(const Token &sep, const char *fn) const
            {
                if(sep.size!=1) throw exception("%sinvalid token.size=%u", fn,unsigned(sep.size) );
                switch(symbol)
                {
                    case DoubleQuote:
                        if( sep.head->code != '"' )
                            throw exception("%sexpecting \" instead of '%s'", fn,printable_char[sep.head->code] );
                        break;
                        
                    case SimpleQuote:
                        if( sep.head->code != '\'')
                            throw exception("%sexpecting ' instead of '%s'", fn,printable_char[sep.head->code] );
                        break;
                }
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
            
            void Strings:: onEmit(const Token &sep)
            {
                
                static const char fn[] = "Jargon::Strings::onEmit: ";
                assert( content.is_valid() );
                assert( context.is_valid() );
                checkSymbol(sep,fn);

                const Context &ctx   = *context;
                const string  &str   = *content;
                Cache          cache = sep.cache;
                const size_t   n     = str.size();
                
                auto_ptr<Unit> unit = new Unit(cache,ctx,label);
                for(size_t i=0;i<n;++i)
                {
                    unit->push_back( Char::Make(cache,ctx,str[i] ) );
                }
                
                content = 0;
                context = 0;
                lexer.unget( unit.yield() );
            }
        }
        
    }
}


