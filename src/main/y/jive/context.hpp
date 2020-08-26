//! \file

#ifndef Y_JIVE_CONTEXT_INCLUDED
#define Y_JIVE_CONTEXT_INCLUDED 1

#include "y/jive/tags.hpp"

namespace upsylon {

    namespace Jive
    {

        class Context : public Tag
        {
        public:
            static const int init_line   = 1;
            static const int init_column = 1;
            const int line;
            const int column;

            template <typename T>
            inline explicit Context(const T &args) :
            Tag(Tags::Make(args)),
            line(init_line),
            column(init_column)
            {
            }


            explicit Context(const Context &) throw();
            virtual ~Context() throw();
        private:
            Y_DISABLE_ASSIGN(Context);
        };

    }

}

#endif
