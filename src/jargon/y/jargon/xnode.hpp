
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
        class                            Terminal; //!< forward Terminal declaration
        class                            Internal; //!< forward Internal declaration
        class                            Axiom;    //!< forward
        typedef arc_ptr<Axiom>           Dogma;    //!< forward shared Axiom
        
        class                            XNode; //!< Syntax Node
        typedef core::list_of_cpp<XNode> XList; //!< Syntax Node List
        
        //----------------------------------------------------------------------
        //
        //! versatile agnostic syntax node
        //
        //----------------------------------------------------------------------
        class XNode :
        public Object,
        public inode<XNode>,
        public Vizible,
        public Serializable
        {
        public:
            static const char LexemeMark = '@'; //!< a lexeme exists
            static const char BranchMark = '>'; //!< no lexeme
            
            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            virtual      ~XNode() throw();                     //!< cleanup
            static XNode *Create( const Axiom &, Lexeme * );   //!< create a generic XNode, lexeme is handled
            
            
            static void Restore(XNode *, Lexer &)           throw(); //!< return content to lexer and push back into list
            static void Advance(XNode * &tree, XNode *node) throw(); //!< handle node to advance/setup tree
            
            
            virtual size_t      serialize(ios::ostream &) const;  //!< serializable interface
            virtual const char *className()        const throw(); //!< serializable interface
            
            const Dogma      dogma;     //!< who created this node
            auto_ptr<Lexeme> lexeme;    //!< with or without lexeme
            XList            children;  //!< children
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(XNode);
            explicit XNode(const Axiom &, Lexeme *) throw();
            virtual  void vizCore(ios::ostream   &) const;
            
        };
        
        
    }
    
}

#endif

