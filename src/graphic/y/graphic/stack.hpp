
//! \file

#ifndef Y_GRAPHIC_STACK_INCLUDED
#define Y_GRAPHIC_STACK_INCLUDED 1

#include "y/graphic/pixmap.hpp"
#include "y/sequence/slots.hpp"

namespace upsylon {

    namespace Graphic {

        //! a stack of pixmaps
        template <typename T>
        class Stack : public slots< Pixmap<T> >
        {
        public:

            typedef slots< Pixmap<T> > Slots; //!< alias

            //! setup
            inline explicit Stack(const size_t n) : Slots(n)
            {
            }

            //! cleanup
            inline virtual ~Stack() throw()
            {
            }
            




        private:
            Y_DISABLE_COPY_AND_ASSIGN(Stack);
        };

    }

}

#endif

