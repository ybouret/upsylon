
//! \file

#ifndef Y_JARGON_LEXICAL_PLUGIN_MULTI_LINES_COMMENT
#define Y_JARGON_LEXICAL_PLUGIN_MULTI_LINES_COMMENT 1


#include "y/jargon/lexical/plugin.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {
            
            class MultiLinesComment : public Plugin
            {
            public:
                virtual ~MultiLinesComment() throw();
                template <typename ID,typename ENTER,typename LEAVE>
                explicit MultiLinesComment(Analyzer       &lexer,
                                           const ID       &id,
                                           const ENTER    &enter,
                                           const LEAVE    &leave ) :
                Plugin(lexer,id,enter)
                {
                    Y_JSCANNER(std::cerr << "|" << id << "| @'" << leave << "'" << std::endl;);
                    back(leave,this,&Scanner::nothing);
                    setup();
                }
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(MultiLinesComment);
                void setup();
                virtual void onInit(const Token &) throw();
            };

#define Y_JARGON_MULTI_LINES_COMMENT(CLASS,ENTER,LEAVE) \
class CLASS : public MultiLinesComment {\
private: Y_DISABLE_COPY_AND_ASSIGN(CLASS);\
public:                 inline virtual ~CLASS() throw() {}\
template <typename ID>  inline explicit CLASS(Analyzer &lexer, const ID &id) :\
MultiLinesComment(lexer,id,ENTER,LEAVE){}\
}
            Y_JARGON_MULTI_LINES_COMMENT(C_Comments,"/\\*","\\*/");
            Y_JARGON_MULTI_LINES_COMMENT(XML_Comments,"<!--","-->");

            
        }
    }
}


#endif

