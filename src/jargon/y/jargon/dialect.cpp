
#include "y/jargon/dialect.hpp"

#include "y/jargon/lexical/plugin/end-of-line-comment.hpp"
#include "y/jargon/lexical/plugin/multi-lines-comment.hpp"
#include "y/jargon/lexical/plugin/jstring.hpp"
#include "y/jargon/lexical/plugin/rstring.hpp"
#include "y/jargon/lexical/plugin/error.hpp"

#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {

        Dialect:: ~Dialect() throw()
        {
        }
        
        Dialect:: Dialect() : Parser( "Jargon::Dialect" )
        {
            
            if(!dict.insert("NAME",RegularExpression::Identifier))
                throw exception("[%s] couldn't initialize dictionary!",**title);
            
            
            //__________________________________________________________________
            //
            // setup grammar
            //__________________________________________________________________
            Aggregate &G   = agg();
            
            
            //__________________________________________________________________
            //
            // terminals declarations
            //__________________________________________________________________
            Axiom     &END   = mark(';');
            Axiom     &COLON = mark(':');
            Axiom     &ID    = term("ID","{NAME}");
            Axiom     &RX    = plug(Lexical::jString::Type,"RX");
            Axiom     &RS    = plug(Lexical::rString::Type,"RS");
            Axiom     &STR   = choice(RX,RS);
            
            //__________________________________________________________________
            //
            // top level: declare module name
            //__________________________________________________________________
            {
                G << cat(term("module", "[.]{NAME}"),END);
            }
            
            //__________________________________________________________________
            //
            // aliases
            //__________________________________________________________________
            
            Axiom & ALIAS = ( agg("alias") << ID << COLON << STR << END);
            
            G << zom(  ( alt() << ALIAS ) );
            
            //__________________________________________________________________
            //
            // lexical only
            //__________________________________________________________________
            grab(Lexical::EndOfLineComment::Type,  "EOLComment","//");
            grab(Lexical::MultiLinesComment::Type, "MLComments","/\\*","\\*/");
            drop("blank", "[:blank:]");
            endl("endl",  "[:endl:]");
            grab(Lexical::Error::Type,"Dialect Syntax Error");
            
            dict.release_all();
            compile();
        }
        

    }
    
}

