
//! \file

#ifndef Y_JSON_COMPILER_INCLUDED
#define Y_JSON_COMPILER_INCLUDED 1

#include "y/jive/syntax/analyzer.hpp"
#include "y/json/value.hpp"
#include "y/hashing/mph.hpp"

namespace upsylon
{

    namespace JSON
    {

        class Analyzer : public Jive::Syntax::Analyzer
        {
        public:
            explicit Analyzer();
            virtual ~Analyzer() throw();


            vector<Value>                  vstack;
            vector<Pair>                   pstack;
            const hashing::minimal_perfect vhash;
            const hashing::minimal_perfect ihash;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Analyzer);
            virtual int onTerminal(const string &id, const Jive::Lexeme &data);
            virtual int onInternal(const string &id, const size_t  size);



        };

    }

}

#endif
