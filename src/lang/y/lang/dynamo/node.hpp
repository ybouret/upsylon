//! \file
#ifndef Y_DYNAMO_NODE_INCLUDED
#define Y_DYNAMO_NODE_INCLUDED 1

#include "y/lang/lexical/lexeme.hpp"
#include "y/lang/dynamo/core/types.hpp"
#include "y/lang/source.hpp"
#include "y/lang/alias.hpp"
#include "y/hashing/digest.hpp"

namespace upsylon{

    namespace Lang {
        
        //! type of DynamoNode
        enum DynamoType
        {
            DynamoTerminal, //!< a terminal
            DynamoInternal  //!< an internal
        };
        
        class DynamoNode;                                 //!< forward declaration
        typedef core::list_of_cpp<DynamoNode> DynamoList; //!< alias
        
        //! a generic node
        class DynamoNode :
        public DynamoObject,
        public core::inode<DynamoNode>,
        public Serializable,
        public Vizible
        {
        public:
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const DynamoType  type;   //!< node type
            const string      name;   //!< from the id=rule.name

            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            //! create an internal node with compiled lexeme
            explicit DynamoNode(const string     &id,
                                const Lexeme     &lx,
                                const size_t      nskip,
                                const size_t      ntrim);
            
            //! create an internal node
            explicit DynamoNode(const string &id);
            
            //! destructor
            virtual ~DynamoNode() throw();

            //------------------------------------------------------------------
            //
            // access method
            //
            //------------------------------------------------------------------
            const string       & content()  const throw(); //!< Terminal: compiled content, empty=>univocal
            DynamoList         & children() throw();       //!< Internal: children
            const DynamoList   & children() const throw(); //!< Internal: children

            //------------------------------------------------------------------
            //
            // output
            //
            //------------------------------------------------------------------
            std::ostream        & output(std::ostream &,int) const;               //!< display the node with indentation level
            static std::ostream & Indent(std::ostream &,int);                     //!< indentation helper
            friend std::ostream & operator<<(std::ostream &, const DynamoNode &); //! display helper

            //------------------------------------------------------------------
            //
            // GraphViz interface
            //
            //------------------------------------------------------------------
            virtual void vizCore(ios::ostream &) const;  //! write node content

            //------------------------------------------------------------------
            //
            // serialize interface
            //
            //------------------------------------------------------------------
            virtual const char *className() const throw();          //!< DynamoNode
            virtual size_t      serialize(ios::ostream &fp) const;  //!< save data
            
            //------------------------------------------------------------------
            //
            // reloading
            //
            //------------------------------------------------------------------
            static DynamoNode *Load(Module *); //!< load from any module

        private:
            void            *impl;
            Y_DISABLE_COPY_AND_ASSIGN(DynamoNode);
            static DynamoNode *Load_(Source &);
        };
        
        
    }
    
}

#endif
