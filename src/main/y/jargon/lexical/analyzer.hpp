
#ifndef Y_JARGON_LEXICAL_ANALYZER_INCLUDED
#define Y_JARGON_LEXICAL_ANALYZER_INCLUDED 1

#include "y/jargon/lexical/scanner.hpp"
#include "y/associative/set.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            
            class Analyzer : public Scanner
            {
            public:
                typedef lstack<Scanner *>           Calls;
                typedef set<string,Scanner::Handle> Scanners;
                explicit Analyzer(const string &id);
                virtual ~Analyzer() throw();
                
                template <typename ID> inline
                Scanner & decl( const ID &id )
                {
                    const string _(id);
                    return declare(id);
                }
                
                Unit *get(Source &source);
                
                void restart() throw();
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Analyzer);
                Scanner   *current;
                Unit::List units;
                Calls      calls;
                Scanners   scanners;
                Scanner   &declare(const string &id);
                void       leap( const string &id, const char *when );
            };
            
        }
        
    }
    
}

#endif

