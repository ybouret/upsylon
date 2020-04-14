
//! \file
#ifndef Y_JSON_COMPILER_INCLUDED
#define Y_JSON_COMPILER_INCLUDED 1

#include "y/json/value.hpp"
#include "y/jargon/parser.hpp"
#include "y/jargon/evaluator.hpp"
#include "y/hashing/mph.hpp"

namespace upsylon {
 
    namespace JSON {
      
        //! compiling JSON files
        class Compiler : public Jargon::Parser, public Jargon::Evaluator
        {
        public:
            explicit Compiler();          //!< setup
            virtual ~Compiler() throw();  //!< cleanup
            
            //! all in one function
            template <typename FILENAME>
            const Value & operator()( const FILENAME &fileName )
            {
                auto_ptr<Jargon::XNode> tree = parseFile(fileName);
                browse(*tree);
                return value;
            }
           
            Value                   value;   //!< final value
            Array                   vArray;  //!< stack of values
            vector<Pair>            vObject; //!< stack of pairs
            
            
        private:
            const hashing::mperf  terminalHash;
            const hashing::mperf  internalHash;
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
            virtual void onTerminal(const Jargon::Tag &, const Jargon::Token &);
            virtual void onInternal(const Jargon::Tag &, const size_t );
            virtual void onOperator(const Jargon::Tag &,const Jargon::Token &,const size_t);

            virtual void onInit();
            virtual void onQuit();
        };
        
    }
    
}

#endif

