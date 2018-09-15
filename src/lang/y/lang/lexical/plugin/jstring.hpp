//! \file
#ifndef Y_LANG_LEXICAL_JSTRING_INCLUDED
#define Y_LANG_LEXICAL_JSTRING_INCLUDED 1

#include "y/lang/lexical/plugin.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {

            class __String : public Plugin
            {
            public:
                virtual ~ __String() throw();
                virtual void Init(const Token &);                //!< clear content
                
            protected:
                explicit __String(const string &id, const char *ch);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(__String);
            };

            //! almost json string
            class jString : public Plugin
            {
            public:

                explicit jString(Translator &,const string &id); //!< initialize
                virtual ~jString() throw();                      //!< destructor
                virtual void Init(const Token &);                //!< clear content

                Token content; //!< localy collected

            private:
                Y_DISABLE_COPY_AND_ASSIGN(jString);
                void Collect(const Token &t);
                void Finish(const Token &);
                
            };

        }

    }

}

#endif

