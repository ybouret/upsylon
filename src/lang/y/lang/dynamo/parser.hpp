
//! \file
#ifndef Y_LANG_DYNAMO_PARSER_INCLUDED
#define Y_LANG_DYNAMO_PARSER_INCLUDED 1

#include "y/lang/dynamo/core/parser.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! full parsing of dynamo trees
        /**
         run the parser and process the '%include' commands
         */
        class DynamoParser : public DynamoCoreParser
        {
        public:
            explicit DynamoParser();         //!< initialize
            virtual ~DynamoParser() throw(); //!< desctructor

            XNode * load( Module *m ); //!< load grammar node and perform includes

            XNode * getCmdArgs( XNode &cmd, string &cmdName ) const; //!< get first argument and the '%cmdName' from the node

            string   getRS(  const XNode &node ) const; //!< extract rs, without simple quote
            string   getRX(  const XNode &node ) const; //!< extract rx, without double quotes
            string   getSTR( const XNode &node ) const; //!< extract RS or RX, remove quote(s)
            string   getRID( const XNode &node ) const; //!< extract rid

        private:
            Y_DISABLE_COPY(DynamoParser);
            void checkIncludes(Syntax::Node &node, const Module &currentModule);
            
            string         getString(const XNode &node,
                                     const char  *description,
                                     Matching    &matching) const;
            mutable Matching isRS;
            mutable Matching isRX;
            mutable Matching isSTR;
            
            string getContent(const XNode &node,
                              const char  *description,
                              const char  *id) const;
            
        };
        
    }

}
#endif
