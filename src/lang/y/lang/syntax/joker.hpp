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

            //! Joker for Rule
            class Joker : public Rule
            {
            public:
                virtual ~Joker() throw(); //!< destructor


            protected:
                //! initialize
                explicit Joker(const uint32_t t,
                               const string  &id,
                               const Rule    &jk);

                const Rule &rule; //!< the target rule

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Joker);
            };

            //! Optional Rule
            class Optional : public Joker
            {
            public:
                //! class identifier
                static const uint32_t UUID = Y_FOURCC('O','P','T',' ');

                virtual ~Optional() throw();                          //!< destructor
                explicit Optional(const string &id, const Rule &jk);  //!< initialize
                virtual bool accept( Y_LANG_SYNTAX_RULE_ARGS ) const; //!< always true

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Optional);
            };


            //! Repeating a rule
            class Repeating : public Joker
            {
            public:
                //! class identifier
                static const uint32_t UUID = Y_FOURCC('R','E','P',' ');

                const size_t nmin; //!< minimal count

                //! desctructor
                virtual ~Repeating() throw();

                //! initialize
                explicit Repeating(const string &id, const Rule &jk,const size_t n);
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Repeating);
            };

        }

    }

}


#endif

