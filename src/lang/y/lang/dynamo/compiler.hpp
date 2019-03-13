//! \file
#ifndef Y_DYNAMO_COMPILER_INCLUDED
#define Y_DYNAMO_COMPILER_INCLUDED 1

#include "y/lang/dynamo/parser.hpp"
#include "y/hashing/mph.hpp"
#include "y/lang/syntax/analyzer.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! type of DynamoNode
        enum DynamoType
        {
            DynamoTerminal, //!< a terminal
            DynamoInternal  //!< an internal
        };

        class DynamoNode;                                 //!< forward declaration
        typedef core::list_of_cpp<DynamoNode> DynamoList; //!< alias

        //! a generic node
        class DynamoNode : public core::inode<DynamoNode>
        {
        public:
            const DynamoType type; //!< ist type
            const string     name; //!< from the id=rule.name

            //! create an internal node with compiled lexeme
            explicit DynamoNode(const string &id,
                                const Lexeme &lx,
                                const size_t  nskip,
                                const size_t  ntrim);

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
            
        private:
            void            *impl;
            Y_DISABLE_COPY_AND_ASSIGN(DynamoNode);
        };


        //! build an intermediate tree
        class DynamoCompiler : public Syntax::Analyzer
        {
        public:
            explicit DynamoCompiler();         //!< setup
            virtual ~DynamoCompiler() throw(); //!< destructor

            //! generate intermediate representation code from node
            DynamoNode *compile( const XNode &node );

            //! last number of created nodes
            const size_t created;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoCompiler);
            DynamoList           items; //!< stack
            const hashing::mperf lxh;   //!< "module", "cid", "lid", "rs", "rx"

            //! called when a terminal is met
            virtual void onTerminal( const string &id, const Lexeme &lx );

            //! called when an internal is met
            virtual void onInternal( const string &id, const size_t  sz, const string *data);
        };

    }
}

#endif

