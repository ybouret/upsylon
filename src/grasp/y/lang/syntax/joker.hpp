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
            //! a Joker holds a reference to another Rule
            class Joker : public Rule
            {
            public:
                virtual ~Joker() throw(); //!< destructor
                const Rule &jk;           //!< the joker rule

            protected:
                //! construct and reject a hollow rule!
                explicit Joker(const uint32_t i, const string &n, const Rule &r);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Joker);
            };

            //! optional joker, always accepted
            class Optional : public Joker
            {
            public:
                static const uint32_t UUID = Y_FOURCC('?',0,0,0); //!< UUID

                explicit Optional(const string &n, const Rule &r); //!< setup
                virtual ~Optional() throw();                       //!< destructor

                Y_LANG_SYNTAX_ACCEPT_PROTO();                 //!< true, may contain data
                virtual const char *typeName() const throw(); //!< "Optional"
                virtual bool        isHollow() const throw(); //!< true

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Optional);
            };

            //! accept zero or more times the same rule
            class ZeroOrMore : public Joker
            {
            public:
                static const uint32_t UUID = Y_FOURCC('>', '=', '0', 0); //!< UUID

                explicit ZeroOrMore(const string &n, const Rule &r); //!< setup
                virtual ~ZeroOrMore() throw();                       //!< desctructor
                
                Y_LANG_SYNTAX_ACCEPT_PROTO();                 //!< count, and always accept
                virtual const char *typeName() const throw(); //!< "ZeroOrMore"
                virtual bool        isHollow() const throw(); //!< true

            private:
                Y_DISABLE_COPY_AND_ASSIGN(ZeroOrMore);
            };

            //! accept one or more times the same rule
            class OneOrMore : public Joker
            {
            public:
                static const uint32_t UUID = Y_FOURCC('>', '=', '1', 0); //!< UUID

                explicit OneOrMore(const string &n, const Rule &r); //!< setup
                virtual ~OneOrMore() throw();                       //!< desctructor

                Y_LANG_SYNTAX_ACCEPT_PROTO();                 //!< count and accept if >= 1
                virtual const char *typeName() const throw(); //!< "OneOrMore"
                virtual bool        isHollow() const throw(); //!< false

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OneOrMore);
            };

        }
    }
}

#endif

