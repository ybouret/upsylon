//! \file
#ifndef Y_DYNAMO_COMPILER_INCLUDED
#define Y_DYNAMO_COMPILER_INCLUDED 1

#include "y/lang/dynamo/node.hpp"
#include "y/lang/dynamo/core-parser.hpp"
#include "y/lang/syntax/analyzer.hpp"
#include "y/hashing/mph.hpp"

namespace upsylon
{
    namespace Lang
    {

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
            const hashing::mperf lxh;   //!< "module", "cid", "lid", "rs", "rx", "eid"

            //! called when a terminal is met
            virtual void onTerminal( const string &id, const Lexeme &lx );

            //! called when an internal is met
            virtual void onInternal( const string &id, const size_t  sz, const string *data);
        };

    }
}

#endif

