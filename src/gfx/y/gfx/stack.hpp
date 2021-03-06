
//! \file

#ifndef Y_GFX_STACK_INCLUDED
#define Y_GFX_STACK_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/sequence/slots.hpp"

namespace upsylon
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! stack of pixmaps of same sizes
        //
        //______________________________________________________________________
        template <typename T>
        class stack : public slots< pixmap<T> >
        {
        public:
            typedef slots< pixmap<T> > slots_type;
            
            inline explicit stack(const size_t n, const unit_t W, const unit_t H) :
            slots_type(n)
            {
                for(size_t i=0;i<n;++i)
                {
                    this->template build<size_t,size_t>(W,H);
                }
            }
            
            inline virtual ~stack() throw()
            {
            }            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(stack);
        };
    }
    
}

#endif
