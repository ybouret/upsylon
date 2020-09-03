
#include "y/jive/lexer.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Jive
    {
        Lexer:: ~Lexer() throw()
        {
            db.release_all();
            if(refcount()>0)
            {
                (void) liberate();
            }
        }

        void Lexer:: initialize()
        {
            withhold();
            const Scanner::Pointer self = this;
            try
            {
                if( !db.insert_by( *label, self) )
                {
                    throw exception("[%s] unexpected initialize failure", **label);
                }
            }
            catch(...)
            {
                (void) liberate();
                throw;
            }
        }
    }

}
