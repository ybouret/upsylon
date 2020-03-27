
//! \file

#ifndef Y_JARGON_XNODE_INCLUDED
#define Y_JARGON_XNODE_INCLUDED 1

#include "y/jargon/lexeme.hpp"

namespace upsylon {
    
    namespace Jargon {

        class XNode;
        typedef core::list_of_cpp<XNode> XList;

        class XNode :
        public Object,
        public inode<XNode>,
        public Vizible
        {
        public:
            enum Kind
            {
                IsTerminal,
                IsInternal,
                IsInactive
            };
            
            const Kind  kind;
            
            static XNode * Create();               //!< create an Inactive XNode
            static XNode * Create(Lexeme *lexeme); //!< lexeme==NULL => internal
            
            const Lexeme & lexeme() const throw();
            XList        & children() throw();
            const XList  & children() const throw();

            virtual       ~XNode() throw(); //!< cleanup
            static void    Release( XNode *xnode, XList &xcache ) throw();
            bool           isInternal() const throw();
            bool           isTerminal() const throw();
            bool           isInactive() const throw();
            
            void           inactiveTo(Lexeme *lexeme) throw();
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(XNode);
            explicit XNode() throw(); //!< create an inactive node
            union {
                Lexeme *lexeme;
                char    children[sizeof(XList)];
            } impl;
            void clr() throw();
            virtual void vizCore( ios::ostream &fp ) const;
        };

    }
    
}

#endif

