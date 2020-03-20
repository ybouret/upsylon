#include "y/jargon/pattern/regexp.hpp"
#include "y/jargon/pattern/dictionary.hpp"
#include "y/jargon/pattern/all.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        bool RegularExpression::Verbose = false;
        
#define LPAREN     '('
#define RPAREN     ')'
#define ALT        '|'
#define BACKSLASH  '\\'
#define LBRACK     '['
#define RBRACK     ']'
#define DASH       '-'
#define LBRACE     '{'
#define RBRACE     '}'
#define CARRET     '^'

#define Y_RX_VERBOSE(code) if (Verbose) do { code; } while(false)
        
        class RegularExpression::Engine
        {
        public:
            //------------------------------------------------------------------
            //
            // data to process
            //
            //------------------------------------------------------------------
            const char       *curr;
            const char       *last;
            const Dictionary *dict;
            
            //------------------------------------------------------------------
            //
            // cleanup
            //
            //------------------------------------------------------------------
            inline ~Engine() throw() {}
             
            //------------------------------------------------------------------
            //
            // startup
            //
            //------------------------------------------------------------------
            inline Engine(const string &rx, const Dictionary *d) :
            curr( *rx ),
            last( curr + rx.size() ),
            dict( d )
            {
                Y_RX_VERBOSE(std::cerr << "compiling" << std::endl);
            }
            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Engine);
        };
        
        Pattern * RegularExpression::Compile(const string &rx, const Dictionary *dict)
        {
            Engine engine(rx,dict);
            return 0;
        }
    }
    
}
