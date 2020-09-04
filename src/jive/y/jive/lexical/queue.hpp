//! \file


#ifndef Y_JIVE_LEXICAL_Q_INCLUDED
#define Y_JIVE_LEXICAL_Q_INCLUDED 1

#include "y/jive/lexical/unit.hpp"

namespace upsylon {


    namespace Jive {

        namespace Lexical
        {
            class Queue
            {
            public:
                virtual ~Queue() throw();

                virtual void push(Unit *) throw() = 0;

            protected:
                explicit Queue() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Queue);
            };
        }

    }

}

#endif
