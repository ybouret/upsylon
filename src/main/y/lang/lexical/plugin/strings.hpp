//! \file
#ifndef Y_LANG_LEXICAL_STRINGS_INCLUDED
#define Y_LANG_LEXICAL_STRINGS_INCLUDED 1

#include "y/lang/lexical/plugin.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {

            //! base class for a String like plugin
            class __String : public Plugin
            {
            public:
                virtual ~ __String() throw();     //!< destructor
                virtual void Init(const Token &); //!< clear content

                Token content; //!< collected
                
            protected:
                //! initialize with return type=id, and delimiter=ch
                explicit __String(Translator &tr,const string &id, const char *ch);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(__String);
                void Collect(const Token &);
                void Forward(const Token &);
                void EscCopy(const Token &); //!< quote(s)
                void EscCode(const Token &); //!< \n, \r, \t,...
                void EscHexa(const Token &); //!< \xhh
            };

            //! almost json string, between double quotes
            class jString : public __String
            {
            public:
                explicit jString(Translator &,const string &id); //!< initialize
                virtual ~jString() throw();                      //!< destructor

            private:
                Y_DISABLE_COPY_AND_ASSIGN(jString);

            };

            //! raw string, between apostrophes
            class rString : public __String
            {
            public:
                explicit rString(Translator &,const string &id); //!< initialize
                virtual ~rString() throw();                      //!< destructor

            private:
                Y_DISABLE_COPY_AND_ASSIGN(rString);
            };

        }

    }

}

#endif

