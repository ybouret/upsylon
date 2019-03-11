//! \file
#ifndef Y_DYNAMO_COMPILER_INCLUDED
#define Y_DYNAMO_COMPILER_INCLUDED 1

#include "y/lang/dynamo/loader.hpp"
#include "y/lang/syntax/analyzer.hpp"

namespace upsylon
{
    namespace Lang
    {
        class DynamoNode; //!< foward declaration
        
        
        //! a list of DynamoNodes
        class DynamoList : public core::list_of_cpp<DynamoNode>
        {
        public:
            explicit DynamoList(const string *) throw(); //!< setup
            virtual ~DynamoList() throw();               //!< destruct
            DynamoList( const DynamoList &other);        //!< hard copy
            const string *data;
            
        private:
            Y_DISABLE_ASSIGN(DynamoList);
        };
        
        enum DynamoType
        {
            DynamoTerminal,
            DynamoInternal
        };
        
        //! used to build in intermediate tree
        class DynamoNode : public core::inode<DynamoNode>
        {
        public:
            explicit DynamoNode(const string &id,
                                const Lexeme &lx,
                                const size_t nskip=0,
                                const size_t ntrim=0);
            
            explicit DynamoNode(const string &id, const string *s);
            
            virtual ~DynamoNode() throw();
            DynamoNode(const DynamoNode &other);
            
            const DynamoType type;
            const string     name;
            
            DynamoList       & children() throw();
            const DynamoList & children() const throw();
            
        private:
            void *impl;
            Y_DISABLE_ASSIGN(DynamoNode);
        };
    
        
        //! build an intermediate tree
        class DynamoCompiler : public DynamoLoader, public Syntax::Analyzer
        {
        public:
            explicit DynamoCompiler();
            virtual ~DynamoCompiler() throw();
            
            DynamoNode *IL( const XNode &top );
            
            size_t created;
            
        private:
            //! called when a terminal is met
            virtual void onTerminal( const string &id, const Lexeme &lx );
            
            //! called when an internal is met
            virtual void onInternal( const string &id, const size_t  sz, const string *data);
            
            DynamoList items;
            Y_DISABLE_COPY_AND_ASSIGN(DynamoCompiler);
            
        };
        
    }
}

#endif

