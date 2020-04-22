
#include "y/jargon/dialect.hpp"

#include "y/jargon/lexical/plugin/end-of-line-comment.hpp"
#include "y/jargon/lexical/plugin/multi-lines-comment.hpp"
#include "y/jargon/lexical/plugin/jstring.hpp"
#include "y/jargon/lexical/plugin/rstring.hpp"

namespace upsylon {
    
    namespace Jargon {

        Dialect:: ~Dialect() throw()
        {
        }
        
        Dialect:: Dialect() : Parser( "Jargon::Dialect" )
        {
            
            //__________________________________________________________________
            //
            // setup grammar
            //__________________________________________________________________
            Axiom &ID = term("ID",RegularExpression::Identifier);
            
            //__________________________________________________________________
            //
            // lexical only
            //__________________________________________________________________
            load(type2type<Lexical::CppComment>(),"C++Comment").hook(*this);
            load(type2type<Lexical::C_Comments>(),"C_Comments").hook(*this);

            drop("blank", "[:blank:]");
            endl("endl",  "[:endl:]");

            
            compile();
        }
        

    }
    
}

