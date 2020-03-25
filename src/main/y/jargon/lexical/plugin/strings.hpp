
//! \file

#ifndef Y_JARGON_LEXICAL_PLUGIN_STRINGS_COMMENT
#define Y_JARGON_LEXICAL_PLUGIN_STRINGS_COMMENT 1


#include "y/jargon/lexical/plugin.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {
            
            class Strings : public Plugin
            {
            public:
                enum Separator
                {
                    DoubleQuote,
                    SimpleQuote
                };
                static const char *RegExp( const Separator ) throw();
                
                virtual ~Strings() throw();
                
                const Separator symbol;
                
                template <typename ID>
                Strings(Analyzer   &Lx,
                        const ID   &id,
                        const Separator sym) :
                Plugin(Lx,id, RegExp(sym) ),
                symbol(sym),
                content(0),
                context(0)
                {
                    setup();
                }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Strings);
                void setup();
                virtual void onInit(const Token &);
                auto_ptr<string>  content;
                auto_ptr<Context> context;
                void onCore(const Token &);
                void onEmit(const Token &);
                
                void checkSymbol(const Token &sep, const char *fn) const;
            };
            
            class jString : public Strings
            {
            public:
                virtual ~jString() throw() {}
                template <typename ID>
                jString(Analyzer   &Lx, const ID   &id) : Strings(Lx,id,DoubleQuote) {}
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(jString);
            };
            
            class rString : public Strings
            {
            public:
                virtual ~rString() throw() {}
                template <typename ID>
                rString(Analyzer &Lx, const ID   &id) : Strings(Lx,id,SimpleQuote) {}
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(rString);
            };
            
        }
        
    }
    
}

#endif
