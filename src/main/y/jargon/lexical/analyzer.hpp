
#ifndef Y_JARGON_LEXICAL_ANALYZER_INCLUDED
#define Y_JARGON_LEXICAL_ANALYZER_INCLUDED 1

#include "y/jargon/lexical/scanner.hpp"
#include "y/associative/set.hpp"
#include "y/jargon/pattern/dictionary.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            //------------------------------------------------------------------
            //
            //! analyzer, with different sub-scanners
            /**
             transform a source of Chars in a source of Units
             */
            //------------------------------------------------------------------
            class Analyzer : public Scanner
            {
            public:
                typedef lstack<Scanner *>           Calls;    //!< alias to store calls
                typedef set<string,Scanner::Handle> Scanners; //!< database of scanners
                
                explicit Analyzer(const string &id); //!< start
                virtual ~Analyzer() throw();         //!< cleanup
                
                //! scanner declaration
                template <typename ID> inline
                Scanner & decl( const ID &id )
                {
                    const string _(id);
                    return declare(id);
                }
                
                //! get next unit, processing all events
                Unit *get(Source &source);
                
                //! restart state
                void restart() throw();
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Analyzer);
                Scanner   &declare(const string &id);
                void       leap( const string &id, const char *when );
                
                Scanner   *current;
                Unit::List units;
                Calls      calls;
                Scanners   scanners;
                
            public:
                Dictionary dict;
                Tags       tags;
               
            };
            
        }
        
    }
    
}

#endif

