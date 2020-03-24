
#include "y/jargon/lexical/analyzer.hpp"
#include "y/exception.hpp"
#include "y/associative/suffix-tree.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {

            typedef  suffix_tree<Scanner *> sDict;
            
            Analyzer:: ~Analyzer() throw()
            {
                if(sdict)
                {
                    delete static_cast<sDict *>(sdict);
                }
            }
            
            
            void Analyzer:: compile()
            {
                if(!sdict)
                {
                    throw exception("[[%s]] is already compiled",**label);
                }
                
                compileRulesWith(*this);
                sDict *dict = static_cast<sDict *>(sdict);
                finish();
                size_t          n = dict->entries();
                sDict::iterator i = dict->begin();
                while(n>0)
                {
                    Scanner &sub = **i;
                    sub.finish();
                    sub.compileRulesWith(*this);
                    ++i;
                    --n;
                }
                
                delete dict;
                sdict=0;
                
            }

  
            
            Analyzer:: Analyzer(const string &id) :
            Scanner( new string(id) ),
            scanners(),
            units(),
            sdict( new sDict() )
            {
                
            }
            

            
        }
        
    }
    
}


