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

                const Rule &jk;

            protected:
                explicit Joker(const uint32_t i, const string &n, const Rule &r);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Joker);
            };

            class Optional : public Joker
            {
            public:
                static const uint32_t UUID = Y_FOURCC('?',0,0,0);

                explicit Optional(const string &n, const Rule &r);
                virtual ~Optional() throw();

                Y_LANG_SYNTAX_ACCEPT_PROTO();
                virtual const char *typeName() const throw(); //!< "Optional"
                virtual bool        isHollow() const throw(); //!< true

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Optional);
            };

            class ZeroOrMore : public Joker
            {
            public:
                static const uint32_t UUID = Y_FOURCC('>', '=', '0', 0);

                explicit ZeroOrMore(const string &n, const Rule &r);
                virtual ~ZeroOrMore() throw();
                
                Y_LANG_SYNTAX_ACCEPT_PROTO();
                virtual const char *typeName() const throw(); //!< "ZeroOrMore"
                virtual bool        isHollow() const throw(); //!< true

            private:
                Y_DISABLE_COPY_AND_ASSIGN(ZeroOrMore);
            };

        }
    }
}

#endif

