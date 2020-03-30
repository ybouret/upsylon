
//! \file

#ifndef Y_JARGON_XNODE_INCLUDED
#define Y_JARGON_XNODE_INCLUDED 1

#include "y/jargon/lexer.hpp"

namespace upsylon {
    
    namespace Jargon {

        //----------------------------------------------------------------------
        //
        // forward declarations and aliases
        //
        //----------------------------------------------------------------------
        class                  Inactive; //!< forward
        class                  Terminal; //!< forward
        class                  Internal; //!< forward
        class                  Axiom;    //!< forward
        typedef arc_ptr<Axiom> Dogma;    //!< forward
        
        class                            XNode; //!< Syntax Node
        typedef core::list_of_cpp<XNode> XList; //!< Syntax Node List
        
        //----------------------------------------------------------------------
        //
        //! versatile, variant syntax node
        //
        //----------------------------------------------------------------------
        class XNode :
        public Object,
        public inode<XNode>,
        public Vizible,
        public Serializable
        {
        public:
            //! genre of syntax node
            enum Genre
            {
                IsTerminal, //!< a terminal => lexeme
                IsInternal, //!< an internal => children
                IsInactive  //!< prepared memory
            };
            static const char TerminalMark = '_';
            static const char InternalMark = '@';
            static const char InactiveMark = '~';
            
            static XNode *Create( const Inactive & );
            static XNode *Create( const Internal & );
            static XNode *Create( const Terminal &, Lexeme *);

            
            const Lexeme & lexeme()   const throw(); //!< lexeme for terminal
            XList        & children() throw();       //!< children for internal
            const XList  & children() const throw(); //!< children for internal

            virtual       ~XNode() throw(); //!< cleanup
            bool           isInternal() const throw();
            bool           isTerminal() const throw();
            bool           isInactive() const throw();
            
            
            static void Restore(XNode    *node, Lexer &, XList &) throw();
            static void Release(XNode *, XList &) throw();
            
            XNode      *activate(const Internal &) throw();
            XNode      *activate(const Terminal &, Lexeme *lexeme) throw();

            virtual size_t      serialize(ios::ostream &) const;
            virtual const char *className()        const throw();
            
            const Genre genre;
            const Dogma dogma;
            
           
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(XNode);
            explicit XNode(const Inactive &)          throw(); //!< create an inactive node
            explicit XNode(const Terminal &,Lexeme *) throw();
            explicit XNode(const Internal &)          throw();
            virtual void   vizCore(ios::ostream   &) const;
           
            
            union {
                Lexeme *lexeme;
                char    children[sizeof(XList)];
            } depot;
            void cleanup()  throw(); //!< clear depot
            void shutdown() throw(); //!< cleanup() and set inactive
        };
        
        
    }
    
}

#endif

