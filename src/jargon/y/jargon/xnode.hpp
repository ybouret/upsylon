
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
        class                        Inactive; //!< forward
        class                        Terminal; //!< forward
        class                        Internal; //!< forward
        class                        Axiom;    //!< forward
        typedef arc_ptr<Axiom>       Dogma;    //!< forward shared Axiom
        
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
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            
            //! genre of syntax node
            enum Genre
            {
                IsTerminal, //!< a terminal => lexeme
                IsInternal, //!< an internal => children
                IsInactive  //!< prepared memory
            };
            
            static const char TerminalMark = '_'; //!< encoding a terminal
            static const char InternalMark = '@'; //!< encoding an internal
            static const char InactiveMark = '~'; //!< encoding an inactive...
            
            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            virtual      ~XNode() throw();                     //!< cleanup
            static XNode *Create( const Inactive & );          //!< create an inactive node
            static XNode *Create( const Internal & );          //!< create an internal node
            static XNode *Create( const Terminal &, Lexeme *); //!< create a terminal node

            
            const Lexeme & lexeme()   const throw(); //!< lexeme for terminal
            XList        & children() throw();       //!< children for internal
            const XList  & children() const throw(); //!< children for internal

            bool           isInternal() const throw(); //!< check if is internal
            bool           isTerminal() const throw(); //!< check if if terminal
            bool           isInactive() const throw(); //!< check if is inactive
            
            static void Restore(XNode *, Lexer &, XList &) throw();  //!< return content to lexer and puah back into list
            static void Release(XNode *, XList &) throw();           //!< recursive cleanup into list
            static void Advance(XNode * &tree, XNode *node) throw(); //!< handle node to advance/setup tree
            
            XNode      *activate(const Internal &)           throw(); //!< inactive => internal
            XNode      *activate(const Terminal &, Lexeme *) throw(); //!< inactive => terminal

            virtual size_t      serialize(ios::ostream &) const;  //!< serializable interface
            virtual const char *className()        const throw(); //!< serializable interface
            
            const Genre genre; //!< genre of this node
            const Dogma dogma; //!< who created this node
            
            //! return the last produced lexeme, may be NULL
            static const Lexeme *LastLexeme(const XNode *xnode) throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(XNode);
            explicit XNode(const Inactive &)          throw();
            explicit XNode(const Terminal &,Lexeme *) throw();
            explicit XNode(const Internal &)          throw();
            virtual void   vizCore(ios::ostream   &) const;
           
            
            union {
                Lexeme *lexeme;                  //!< lexeme   for terminal
                char    children[sizeof(XList)]; //!< children for internal
            } depot;
            void cleanup()  throw(); //!< clear depot
            void shutdown() throw(); //!< cleanup() and set inactive
        };
        
        
    }
    
}

#endif

