//! \file
#ifndef Y_JSON_COMPILER_INCLUDED
#define Y_JSON_COMPILER_INCLUDED 1

#include "y/json/value.hpp"
#include "y/lang/dynamox.hpp"

namespace upsylon
{
    namespace JSON
    {
        class Compiler : public Lang::DynamoX
        {
        public:
            explicit Compiler();
            virtual ~Compiler() throw();
            
            void  load( Value &v, Lang::Module *module, const bool verbose_flag=false);

            virtual void onTerminal(const Lang::Lexeme &data );
            virtual void onInternal(const string &name, const size_t size);


        private:
            Array        vstack;
            vector<Pair> pstack;
            bool         verbose;
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
        };

        
    }
}

#endif

