//! \file
#ifndef Y_JIVE_SYNTAX_XNODE_INCLUDED
#define Y_JIVE_SYNTAX_XNODE_INCLUDED 1

#include "y/jive/lexemes.hpp"

namespace upsylon
{
    namespace Jive
    {
        
        namespace Syntax
        {
            
            
            class XNode : public inode<XNode>
            {
            public:
                enum Kind
                {
                    IsTerminal,
                    IsInternal
                };
                
                
                
            private:
                
            public:
                //__________________________________________________________________
                //
                // types for XNode::Supply
                //__________________________________________________________________
                typedef memory::tight::xcache           XCache_; //!< base class for XCache
                typedef memory::tight::xcache_of<XNode> XCache;  //!< memory cache of Char
                typedef core::pool_of<XNode>            ZCache;  //!< local cache type
                
                class Supply : public singleton<Supply>
                {
                public:
                    
                private:
                    
                };
            };
            
        }
        
    }
}
    

#endif
