
//! \file
#ifndef Y_LANG_DYNAMO_LOADER_INCLUDED
#define Y_LANG_DYNAMO_LOADER_INCLUDED 1

#include "y/lang/dynamo/parser.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! full loading of dynamo trees and utility functions
        class DynamoLoader : public DynamoParser
        {
        public:
            explicit DynamoLoader();         //!< initialize
            virtual ~DynamoLoader() throw(); //!< desctructor

            XNode * load( Module *m ); //!< load grammar node and perform includes

            XNode * getCmdArgs( XNode &cmd, string &cmdName ) const; //!< get first argument and the '%cmdName' from the node

            string   getRS(  const XNode &node ) const; //!< extract rs, without simple quote
            string   getRX(  const XNode &node ) const; //!< extract rx, without double quotes
            string   getSTR( const XNode &node ) const; //!< extract RS or RX, remove quote(s)
            string   getRID( const XNode &node ) const; //!< extract rid

        private:
            Y_DISABLE_COPY(DynamoLoader);
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
