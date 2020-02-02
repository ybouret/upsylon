//! \file
#ifndef Y_DYNAMO_NODE_INCLUDED
#define Y_DYNAMO_NODE_INCLUDED 1

#include "y/lang/lexical/lexeme.hpp"
#include "y/lang/dynamo/types.hpp"
#include "y/lang/source.hpp"
#include "y/ios/serializable.hpp"
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
        public ios::serializable
        {
        public:
            const DynamoType  type;   //!< node type
            const string      name;   //!< from the id=rule.name
            
            
            //! create an internal node with compiled lexeme
            explicit DynamoNode(const string     &id,
                                const Lexeme     &lx,
                                const size_t      nskip,
                                const size_t      ntrim);
            
            //! create an internal node
            explicit DynamoNode(const string &id);
            
            //! destructor
            virtual ~DynamoNode() throw();
            
            const string       & content()  const throw(); //!< Terminal: compiled content, empty=>univocal
            DynamoList         & children() throw();       //!< Internal: children
            const DynamoList   & children() const throw(); //!< Internal: children
            
            //! display the node with indentation level
            std::ostream & display( std::ostream &os, int level ) const;
            
            //! indentation helper
            static        std::ostream & Indent(std::ostream &, int level);
            
            //! display helper
            friend inline std::ostream & operator<<( std::ostream &os, const DynamoNode &node )
            {
                return node.display(os,0);
            }
            
            //! write this node as GraphViz code
            void viz( ios::ostream &fp ) const;
            
            //! create a directed graph and try to render
            void graphViz( const string &filename ) const;
            
            void save( ios::ostream &fp, size_t *bytes=0) const;      //!< save to bin stream
            void save( const string &binfile, size_t *bytes=0) const; //!< save to file
            void save( const char   *binfile, size_t *bytes=0) const; //!< save to file
            
            //! get bytes to store binary
            size_t outputBytes() const;
            
            //! get a binary representation
            string toBinary() const;
            
            //! load from binary stream, recursive
            static DynamoNode *Load_( Source &source );

            //! load from any module
            static DynamoNode *Load( Module *m );
            
            //! recursive run of a hashing function
            void run( hashing::function &H ) const throw();
            
            

            virtual const char *className() const throw();
            virtual size_t      serialize( ios::ostream &fp) const;

        private:
            void            *impl;
            Y_DISABLE_COPY_AND_ASSIGN(DynamoNode);
        };
        
        
    }
    
}

#endif
