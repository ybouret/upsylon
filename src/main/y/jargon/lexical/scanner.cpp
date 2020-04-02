#include "y/jargon/lexical/scanner.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            
            
            Scanner:: ~Scanner() throw()
            {
            }
            
#define Y_JSCANNER_CTOR(TAG) \
CountedObject(),             \
inode<Scanner>(),            \
label(TAG),                  \
rules(),                     \
chars(NULL),                 \
dict_(NULL), plug_(NULL)
            
            Scanner:: Scanner( const string &id ) :
            Y_JSCANNER_CTOR( Tags::Make(id) )
            {
            }
            
            Scanner:: Scanner( const Tag &tag ) :
            Y_JSCANNER_CTOR( Tags::Make(tag) )
            {
            }
            
            bool Scanner::Verbose = false;
            
            void Scanner:: nothing(const Token &) const throw()
            {
            }
            
            const Rule & Scanner:: add(Rule *rule)
            {
                assert(rule);
                auto_ptr<Rule> guard(rule);
                
                Y_JSCANNER(std::cerr << '[' << label << ']' << "+rule <" << rule->label << ">" << std::endl);
                
                for(const Rule *r=rules.head;r;r=r->next)
                {
                    if( *(r->label) == *(rule->label) )     throw exception("[%s] multiple rule <%s>", **label, **(r->label));
                    if( r->motif->alike( & *(rule->motif))) throw exception("[%s] alike patterns <%s> and <%s>",**label,**(r->label),**(rule->label));
                }
                return *rules.push_back( guard.yield() );
            }
            
            void Scanner:: newLine(const Token &) throw()
            {
                assert(chars);
                Y_JSCANNER(std::cerr << '[' << label << ']' << "@newLine " << std::endl);
                chars->newLine();
            }
            
            const string & Scanner:: key() const throw()
            {
                return *label;
            }
            
           
            
        }
    }
}
