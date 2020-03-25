
#ifndef Y_JARGON_LEXICAL_ANALYZER_INCLUDED
#define Y_JARGON_LEXICAL_ANALYZER_INCLUDED 1

#include "y/jargon/lexical/plugin.hpp"
#include "y/associative/suffix-batch.hpp"
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
                typedef lstack<Scanner *>                    Calls;    //!< alias to store calls
                typedef suffix_batch<string,Scanner::Handle> Scanners; //!< database of scanners
                
                explicit Analyzer(const string &id); //!< start
                virtual ~Analyzer() throw();         //!< cleanup
                
                //! scanner declaration
                template <typename ID> inline
                Scanner & decl( const ID &id )
                {
                    Scanner::Handle scan = new Scanner(id);
                    return insert(scan);
                }
                
                //! get next unit, processing all events
                Unit *get(Source &source);
                
                //! restart state
                void restart() throw();
                
                //! load plugin with 1 expression
                template <typename PLUGIN, typename ID, typename EXPR>
                Plugin & load(type2type<PLUGIN>, const ID &id, const EXPR &trigger)
                {
                    return insert( static_cast<Plugin*>(new PLUGIN(*this,id,trigger)) );
                }
                
                //! load plugin with 0 expression
                template <typename PLUGIN, typename ID>
                Plugin & load(type2type<PLUGIN>, const ID &id)
                {
                    return insert( static_cast<Plugin*>(new PLUGIN(*this,id)) );
                }
                
                
                
                
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Analyzer);
                bool       store(Scanner::Handle &scan);
                Scanner   &insert(Scanner::Handle &scan); //!< insert and set dict
                Plugin    &insert( Plugin *plugin );
                void       leap( const string &id, const char *when );
                
                Scanner   *current;
                Unit::List units;
                Calls      calls;
                Scanners   scanners;
                
            public:
                Dictionary dict;   //!< local dictionary to share amongs scanners
            };
            
        }
        
    }
    
}

#endif

