
#ifndef Y_JIVE_LEXER_INCLUDED
#define Y_JIVE_LEXER_INCLUDED 1

#include "y/jive/lexical/scanner.hpp"
#include "y/jive/lexemes.hpp"
#include "y/sequence/list.hpp"

namespace upsylon {

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! Lexer
        /**
         transform a stream of Char(s) into a stream of Lexeme(s)
         using different lexical scanners/plugins
         */
        //______________________________________________________________________

        class Lexer : public Lexical::Scanner
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef Lexer::Scanner                Scanner;      //!< alias
            typedef suffix_tree<Scanner::Pointer> ScannerTree;  //!< alias
            typedef Scanner *HScan;
            typedef list<HScan> History;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            virtual ~Lexer() throw();

            //! setup
            template <typename ID> inline
            explicit Lexer(const ID &id) :
            Scanner(id,AcceptEOS), scan(this), hist(), io(), db()
            {
                initialize();
            }

            //! declare a new scanner
            template <typename ID> inline
            Scanner &decl(const ID &id)
            {
                const Tag l = Tags::Make(id);
                return newScanner(l);
            }

            //! get using scanner
            Lexeme *get(Source &source);



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Lexer);
            Scanner    *scan;
            History     hist;
            Lexemes     io;
            ScannerTree db;

            void      initialize();
            Scanner & newScanner(const Tag &t);
            void      jmp(const Tag &);

        };
    }

}

#endif
