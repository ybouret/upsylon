//! \file

#ifndef Y_JARGON_LEXICAL_PLUGIN_END_OF_LINE_COMMENT
#define Y_JARGON_LEXICAL_PLUGIN_END_OF_LINE_COMMENT 1


#include "y/jargon/lexical/plugin.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {
         
            //! generic End of Line Comment
            class EndOfLineComment : public Plugin
            {
            public:
                //! cleanup
                virtual ~EndOfLineComment() throw();
                
                //! setup
                template <typename ID,typename EXPR>
                explicit EndOfLineComment(Analyzer       &Lx,
                                          const ID       &id,
                                          const EXPR     &regexp) :
                Plugin(Lx,id,regexp)
                {
                    setup();
                }
                
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(EndOfLineComment);
                void setup();
                virtual void onInit(const Token &) throw();
                
            };
            
            //! create some common EOL
#define Y_JARGON_END_OF_LINE_COMMENT(CLASS,TRIGGER) \
class CLASS : public EndOfLineComment {\
private: Y_DISABLE_COPY_AND_ASSIGN(CLASS);\
public:                 inline virtual ~CLASS() throw() {}\
template <typename ID>  inline explicit CLASS(Analyzer &Lx, const ID &id) :\
EndOfLineComment(Lx,id,TRIGGER){}\
}
            Y_JARGON_END_OF_LINE_COMMENT(CppComment,"//");  //!< C++ comment
            Y_JARGON_END_OF_LINE_COMMENT(ShellComment,"#"); //!< Shell comment
            Y_JARGON_END_OF_LINE_COMMENT(LuaComment,"--");  //!< Lua comment
            Y_JARGON_END_OF_LINE_COMMENT(TeXComment,"%");   //!< [La]TeX comment

        }
    }
}

#endif

