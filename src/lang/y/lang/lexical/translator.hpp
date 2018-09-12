//! \file
#ifndef Y_LANG_LEXICAL_TRANSLATOR_INCLUDED
#define Y_LANG_LEXICAL_TRANSLATOR_INCLUDED 1

#include "y/lang/lexical/scanner.hpp"
#include "y/associative/set.hpp"
#include "y/core/addr-list.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
            //! transform a source of Char into a source of Lexeme
            class Translator : public Object
            {
            public:
                typedef set<string,Scanner::Pointer> DataBase; //!< database of scanners
                const Origin name;                              //!< shared name
                
                //! destructor
                virtual ~Translator() throw();

                //! initialize
                explicit Translator(const string &id);

                //! return root scanner
                Scanner & operator *() throw();

                //! create, register and return a new scanner
                Scanner & decl(const string &id);

                //! create, register and return a new scanner
                inline
                Scanner & decl(const char *id) { const string _(id); return decl(_); }

                void    reset() throw();

                void    unget( Lexeme *lx ) throw();
                void    unget( Lexeme::List &lxm ) throw();

                //! get the next lexeme
                Lexeme *get( Source &source );


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Translator);
                typedef core::addr_node<Scanner> sNode;
                typedef core::addr_list<Scanner> sList;

                Scanner     *curr;
                Scanner     *base;
                Lexeme::List cache;
                sList        history;
                DataBase     scanners;

                void setup();
            };
        }

    }
}

#endif

