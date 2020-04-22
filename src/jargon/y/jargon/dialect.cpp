
#include "y/jargon/dialect.hpp"

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

            drop("blank", "[:blank:]");
            endl("endl",  "[:endl:]");

            
            compile();
        }
        

    }
    
}

