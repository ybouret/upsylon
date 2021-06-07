
//! \file

#ifndef Y_JSON_ANALYZER_INCLUDED
#define Y_JSON_ANALYZER_INCLUDED 1

#include "y/jive/language/analyzer.hpp"
#include "y/json/value.hpp"
#include "y/hashing/mph.hpp"

namespace upsylon
{

    namespace JSON
    {

        //______________________________________________________________________
        //
        //
        //! JSON AST analyzer
        //
        //______________________________________________________________________
        class Analyzer : public Jive::Language::Analyzer
        {
        public:
            explicit Analyzer(const Jive::Tag &);  //!< setup
            virtual ~Analyzer() throw();           //!< cleanup
            void     reset() throw();              //!< free vstack and pstack

            Array                          vstack; //!< stack of values
            list<Pair>                     pstack; //!< stack of pairs
            const hashing::minimal_perfect vhash;  //!< hash table for values
            const hashing::minimal_perfect ihash;  //!< hash table for internals

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Analyzer);
            virtual int onTerminal(const string &id, const Jive::Lexeme &data);
            virtual int onInternal(const string &id, const size_t  size);
        };

    }

}

#endif
