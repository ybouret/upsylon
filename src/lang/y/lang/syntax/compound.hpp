//! \file
#ifndef Y_LANG_SYNTAX_COMPOUND_INCLUDED
#define Y_LANG_SYNTAX_COMPOUND_INCLUDED 1

#include "y/lang/syntax/rule.hpp"

namespace upsylon
{
    namespace Lang
    {

        namespace Syntax
        {
            class Compound : public Rule
            {
            public:
                virtual ~Compound() throw();

            protected:
                explicit Compound(const uint32_t t, const string &id);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Compound);
            };


            class Aggregate : public Rule
            {
            public:
                static const uint32_t UUID = Y_FOURCC('A','G','G',' ');;
                explicit Aggregate(const string &id);
                virtual ~Aggregate() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Aggregate);
            };

            class Alternate : public Rule
            {
            public:
                static const uint32_t UUID = Y_FOURCC('A','L','T',' ');;
                explicit Alternate(const string &id);
                virtual ~Alternate() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Alternate);
            };
        }

    }
}

#endif

