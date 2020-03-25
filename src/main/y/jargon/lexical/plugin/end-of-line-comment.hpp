//! \file

#ifndef Y_JARGON_LEXICAL_PLUGIN_END_OF_LINE_COMMENT
#define Y_JARGON_LEXICAL_PLUGIN_END_OF_LINE_COMMENT 1


#include "y/jargon/lexical/plugin.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {
         
            class EndOfLineComment : public Plugin
            {
            public:
                virtual ~EndOfLineComment() throw();
                template <typename ID,typename EXPR>
                explicit EndOfLineComment(Analyzer       &lexer,
                                          const ID       &id,
                                          const EXPR     &regexp) :
                Plugin(lexer,id,regexp)
                {
                    setup();
                }
                
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(EndOfLineComment);
                void setup();
            };
            
#define Y_JARGON_END_OF_LINE_COMMENT(CLASS,TRIGGER) \
class CLASS : public EndOfLineComment {\
private: Y_DISABLE_COPY_AND_ASSIGN(CLASS);\
public:                 inline virtual ~CLASS() throw() {}\
template <typename ID>  inline explicit CLASS(Analyzer &lexer, const ID &id) :\
EndOfLineComment(lexer,id,TRIGGER){}\
}
            Y_JARGON_END_OF_LINE_COMMENT(CppComment,"//");
            Y_JARGON_END_OF_LINE_COMMENT(ShellComment,"#");
            Y_JARGON_END_OF_LINE_COMMENT(LuaComment,"--");
            Y_JARGON_END_OF_LINE_COMMENT(TexComment,"%");

        }
    }
}

#endif

