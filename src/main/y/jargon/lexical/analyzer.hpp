
#ifndef Y_JARGON_LEXICAL_ANALYZER_INCLUDED
#define Y_JARGON_LEXICAL_ANALYZER_INCLUDED 1

#include "y/jargon/lexical/scanner.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            
            class Analyzer : public Scanner
            {
            public:
                typedef core::list_of_cpp<Scanner> Scanners;
                explicit Analyzer(const string &id);
                virtual ~Analyzer() throw();
                
                template <typename ID> inline
                Scanner & decl( const ID &id )
                {
                    const string _(id);
                    return declare(id);
                }
                
                void compile();
                
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Analyzer);
                Scanners   scanners;
                Unit::List units;
                void      *sdict;
                Scanner   &declare(const string &id);
            };
            
        }
        
    }
    
}

#endif

