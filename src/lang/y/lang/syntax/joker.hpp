//! \file
#ifndef Y_LANG_SYNTAX_JOKER_INCLUDED
#define Y_LANG_SYNTAX_JOKER_INCLUDED 1

#include "y/lang/syntax/rule.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            class Joker : public Rule
            {
            public:
                virtual ~Joker() throw();


            protected:
                explicit Joker(const uint32_t t,
                               const string  &id,
                               const Rule    &jk);

                const Rule &rule;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Joker);
            };


            class Optional : public Joker
            {
            public:
                static const uint32_t UUID = Y_FOURCC('O','P','T',' ');

                virtual ~Optional() throw();
                explicit Optional(const string &id, const Rule &jk);

                virtual bool accept( Y_LANG_SYNTAX_RULE_ARGS ) const;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Optional);
            };

            class Repeating : public Joker
            {
            public:
                static const uint32_t UUID = Y_FOURCC('R','E','P',' ');

                const size_t nmin;
                virtual ~Repeating() throw();
                explicit Repeating(const string &id, const Rule &jk);
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Repeating);
            };

        }

    }

}


#endif

