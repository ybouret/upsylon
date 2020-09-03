
#ifndef Y_JIVE_LEXER_INCLUDED
#define Y_JIVE_LEXER_INCLUDED 1

#include "y/jive/lexical/scanner.hpp"

namespace upsylon {

    namespace Jive
    {
        class Lexer : public Lexical::Scanner
        {
        public:
            typedef Lexer::Scanner                Scanner;
            typedef suffix_tree<Scanner::Pointer> ScannerTree;

            virtual ~Lexer() throw();

            template <typename ID> inline
            explicit Lexer(const ID &id) :
            Scanner(id,AcceptEOS),
            db()
            {
                initialize();
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Lexer);
            ScannerTree db;
            void initialize();
            
        };
    }

}

#endif
