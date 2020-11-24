
//! \file

#ifndef Y_JSON_COMPILER_INCLUDED
#define Y_JSON_COMPILER_INCLUDED 1

#include "y/json/parser.hpp"
#include "y/json/analyzer.hpp"

namespace upsylon
{

    namespace JSON
    {
        //______________________________________________________________________
        //
        //
        //! JSON compiler
        //
        //______________________________________________________________________
        class Compiler
        {
        public:
            explicit Compiler(); //!< setup
            virtual ~Compiler() throw(); //!< cleanup

            //! get compiled value
            template <typename FILENAME>
            void loadFile(const FILENAME &fileName, Value &value)
            {
                analyzer.reset();
                parser.start();
                const Jive::XNode::Pointer tree( parser.parseFile(fileName) );
                analyzer.walk( tree.content() );
                value.swapWith(getValue());
                analyzer.reset();
            }

            Parser   parser;    //!< internal parser
            Analyzer analyzer;  //!< internal ananlyzer

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
            Value &getValue();
        };

    }

}

#endif
