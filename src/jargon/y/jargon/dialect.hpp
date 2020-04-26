#ifndef Y_JARGON_DIALECT_INCLUDED
#define Y_JARGON_DIALECT_INCLUDED 1

#include "y/jargon/parser.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        //! creating/loading a dialect, a.k.a DSL
        /**
         - dialect : module item*;
         - module  : ".{ID}" ';';
         - id      : "{ID}";
         - rs      : "...";
         - rx      : '...';
         - item    : plg | ctl | lex;
         - plg     : "@{ID}" ':' id (rs|rx)*; // plugin defintion
         - ctl     : "#{ID}" (rs|rx|int|hex)*; // preprocessing controls
         - lex     : "%{ID}" (rs|rx)*;
         */
        class Dialect : public Parser
        {
        public:
            explicit Dialect();          //!< setup the parser
            virtual ~Dialect() throw();  //!< cleanup
            
            XNode *compileFile(const string &fileName); //!< parse grammar and preprocess
            XNode *compileFile(const char   *fileName); //!< parse grammar and preprocess
            XNode *compileFlat(Module *);               //!< parse preprocessed grammar: file, data...
            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dialect);
            //------------------------------------------------------------------
            // preprocessing
            //------------------------------------------------------------------
            
            string  readCtlName(const XNode &ctl) const;                //! read ctl.name from a <ctl> node
            XNode  *expressBlocks(XNode *root);                         //! check if/endif
            XNode  *checkIncludes(XNode *root, const string &fileName); //! checking include control
            XNode  *include(const XNode &ctl, const string &fileName);  //! load sub-content


        };
        
    }
    
}


#endif
