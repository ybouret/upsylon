
//! \file

#ifndef Y_JARGON_XNODE_INCLUDED
#define Y_JARGON_XNODE_INCLUDED 1

#include "y/jargon/lexeme.hpp"

namespace upsylon {
    
    namespace Jargon {

        class XNode;
        typedef core::list_of_cpp<XNode> XList;

        class XNode : public Object, public inode<XNode>
        {
        public:
            enum Kind
            {
                IsTerminal,
                IsInternal,
                IsInactive
            };
            
            const Kind  kind;
            
            static XNode * Create(); //!< create an Inactive XNode
            
            virtual ~XNode() throw(); //!< cleanup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(XNode);
            explicit XNode() throw(); //!< create an inactive node
            //union { Lexeme *lexeme; XList  *children; };
            
        };

    }
    
}

#endif

