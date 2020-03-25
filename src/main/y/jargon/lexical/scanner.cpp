#include "y/jargon/lexical/scanner.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
#if 0
            const char Scanner:: callPrefix[] = "->";
            const char Scanner:: jumpPrefix[] = "=>";
            const char Scanner:: backPrefix[] = "<-";
#endif
            

            
            Scanner:: ~Scanner() throw()
            {
            }
            
#define Y_JSCANNER_CTOR(TAG) \
CountedObject(),             \
inode<Scanner>(),            \
label(TAG),                  \
rules(),                     \
chars(NULL),                 \
dict_(NULL)        
            
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
           
            void Scanner:: add(Rule *rule)
            {
                assert(rule);
                Y_JSCANNER(std::cerr << '[' << label << ']' << "+rule <" << rule->label << ">" << std::endl);
                for(const Rule *r=rules.head;r;r=r->next)
                {
                    if( *(r->label) == *(rule->label) ) throw exception("[%s] multiple rule <%s>", **label, **(r->label));
                }
                rules.push_back(rule);
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
