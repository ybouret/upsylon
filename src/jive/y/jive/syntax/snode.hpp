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
            
            
            class SNode : public inode<SNode>
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
                typedef memory::tight::xcache_of<SNode> XCache;  //!< memory cache of Char
                typedef core::pool_of<SNode>            ZCache;  //!< local cache type
                
                class Supply : public singleton<Supply>
                {
                public:
                    
                private:
                    Y_SINGLETON_DECL(Supply);
                    Y_DISABLE_COPY_AND_ASSIGN(Supply);
                    explicit Supply();
                    virtual ~Supply() throw();
                    XCache   xchars; //!< global memory for zombi chars
                    ZCache   zchars; //!< local pool of zombi chars
                };
            };
            
        }
        
    }
}
    

#endif
