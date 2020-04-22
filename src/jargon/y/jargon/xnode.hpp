
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
        class                            Axiom; //!< forward declaration of Axiom
        typedef arc_ptr<Axiom>           Dogma; //!< forward shared Axiom
        class                            XNode; //!< Syntax Node
        typedef core::list_of_cpp<XNode> XList; //!< Syntax Node List
        
        //----------------------------------------------------------------------
        //
        //! versatile agnostic syntax node
        //
        //----------------------------------------------------------------------
        class XNode : public Object, public inode<XNode>, public Vizible, public Serializable
        {
        public:
            //------------------------------------------------------------------
            //
            // definitions
            //
            //------------------------------------------------------------------
            static const char LexemeMark = '@'; //!< a lexeme exists
            static const char BranchMark = '>'; //!< no lexeme
            static const char CLID[];           //!< "XNode"
            
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            virtual      ~XNode() throw();                     //!< cleanup
            static XNode *Create( const Axiom &, Lexeme * );   //!< create a generic XNode, lexeme is handled
            
            
            //------------------------------------------------------------------
            //
            // helpers
            //
            //------------------------------------------------------------------
            static void Restore(XNode *, Lexer &)           throw(); //!< return content to lexer and push back into list
            static void Advance(XNode * &tree, XNode *node) throw(); //!< handle node to advance/setup tree
            bool   is(const string &) const throw();                 //!< compare to dogma->label
            bool   is(const char   *) const throw();                 //!< compare to dogma->label
            
            template <typename LABEL>
            inline friend bool operator==(const XNode &lhs, const LABEL &rhs) throw()
            {
                return lhs.is(rhs);
            }
            
            
            //------------------------------------------------------------------
            //
            // virtual methods
            //
            //------------------------------------------------------------------
            
            //! serializable interface
            /**
             Used with a grammar, this is an Intermediate Language to save AST
             - dogma->label
             - if lexeme: { LexemeMark + lexeme } else: { BranchMark }
             - #children
             - children
             */
            virtual size_t      serialize(ios::ostream &) const;
            virtual const char *className()        const throw(); //!< serializable : CLID
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const Dogma      dogma;     //!< who created this node
            auto_ptr<Lexeme> lexeme;    //!< with or without lexeme
            XList            children;  //!< children
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(XNode);
            explicit      XNode(const Axiom &, Lexeme *) throw();
            virtual  void vizCore(ios::ostream   &)      const;
            
        };
        
        
    }
    
}

#endif

