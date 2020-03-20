#include "y/jargon/pattern/regexp.hpp"
#include "y/jargon/pattern/dictionary.hpp"
#include "y/jargon/pattern/all.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        bool RegularExpression::Verbose = true;
        
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
            inline Engine(const char *rx, const size_t nc, const Dictionary *d) :
            curr( rx ),
            last( rx + nc ),
            dict( d )
            {
                assert( !(NULL==rx&&nc>0) );
                Y_RX_VERBOSE(std::cerr << "compiling" << std::endl);
            }
            
            
            //------------------------------------------------------------------
            //
            // compile expression starting from curr
            //
            //------------------------------------------------------------------
            Logical *compileExpression()
            {
                auto_ptr<Logical> expr = AND::Create();
            
                while(curr<last)
                {
                    const char C = *curr;
                    expr->add(C);
                    ++curr;
                }
                
                return expr.yield();
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Engine);
        };
        
        Pattern * RegularExpression::Compile(const string &rx, const Dictionary *dict)
        {
            Engine engine(*rx,rx.size(),dict);
            return engine.compileExpression();
        }
        
        Pattern * RegularExpression::Compile(const char *rx, const Dictionary *dict)
        {
            const  string _(rx);
            return Compile(_,dict);
        }
    }
    
}
