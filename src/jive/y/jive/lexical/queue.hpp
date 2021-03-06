//! \file


#ifndef Y_JIVE_LEXICAL_Q_INCLUDED
#define Y_JIVE_LEXICAL_Q_INCLUDED 1

#include "y/jive/lexical/unit.hpp"

namespace upsylon {


    namespace Jive {

        namespace Lexical
        {
            class Plugin; //!< forward declaration

            //__________________________________________________________________
            //
            //
            //! interface for plugins to return compiled units
            //
            //__________________________________________________________________
            class Queue
            {
            public:
                virtual ~Queue() throw();              //!< cleanup
                virtual void push(Unit *) throw() = 0; //!< store result

            protected:
                explicit Queue() throw();              //!< setup
                size_t   counter;                      //!< shared counted to stamp units

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Queue);
                friend class Plugin;
            };
        }

    }

}

#endif
