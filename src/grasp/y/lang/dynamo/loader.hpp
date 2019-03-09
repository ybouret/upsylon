
//! \file
#ifndef Y_LANG_DYNAMO_LOADER_INCLUDED
#define Y_LANG_DYNAMO_LOADER_INCLUDED 1

#include "y/lang/dynamo/parser.hpp"

namespace upsylon
{
    namespace Lang
    {

        class DynamoLoader : public DynamoParser
        {
        public:
            explicit DynamoLoader();
            virtual ~DynamoLoader() throw();

            //! load grammar node and perform includes
            XNode * load( Module *m );


            Syntax::Node * getCmdArgs( XNode &cmd, string &cmdName ) const;

            string         getString(const XNode &node,
                                     const char  *description,
                                     bool       (*matching)(const string &identifier)) const;


            string         getRS(  const XNode &node ) const;
            string         getRX(  const XNode &node ) const;
            string         getSTR( const XNode &node ) const;

        private:
            Y_DISABLE_COPY(DynamoLoader);
            void checkIncludes(Syntax::Node &node, const Module &currentModule);
        };
        
    }

}
#endif
