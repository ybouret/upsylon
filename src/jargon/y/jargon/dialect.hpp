#ifndef Y_JARGON_DIALECT_INCLUDED
#define Y_JARGON_DIALECT_INCLUDED 1

#include "y/jargon/parser.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! creating/loading a dialect, a.k.a DSL
        class Dialect : public Parser
        {
        public:
            
            explicit Dialect();          //!< setup the parser
            virtual ~Dialect() throw();  //!< cleanup
            
            XNode *checkIncludes(XNode *root, const string &fileName);
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dialect);
            string readCID(const XNode &ctl) const;
            XNode *include(const XNode &ctl, const string &fileName);
        };
        
    }
    
}


#endif
