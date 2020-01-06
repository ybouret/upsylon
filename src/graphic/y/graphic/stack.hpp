
//! \file

#ifndef Y_GRAPHIC_STACK_INCLUDED
#define Y_GRAPHIC_STACK_INCLUDED 1

#include "y/graphic/pixmap.hpp"
#include "y/sequence/slots.hpp"

namespace upsylon {

    namespace Graphic {

        //! a stack of pixmaps
        template <typename T>
        class Stack : public Area, public slots< Pixmap<T> >
        {
        public:
            typedef slots< Pixmap<T> > Slots; //!< alias

            //! setup
            inline explicit Stack(const size_t W,
                                  const size_t H,
                                  const size_t n) :
            Area(W,H),
            Slots(n)
            {
                for(size_t i=0;i<n;++i)
                {
                    this->template build<size_t,size_t>(w,h);
                }
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

