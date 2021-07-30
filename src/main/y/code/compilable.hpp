//! \file

#ifndef Y_CODE_COMPILABLE_INCLUDED
#define Y_CODE_COMPILABLE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    
    //__________________________________________________________________________
    //
    //
    //! interface for compilable objects
    //
    //__________________________________________________________________________
    class compilable
    {
    public:
        virtual   ~compilable() throw(); //!< cleanup
        void       compile();            //!< call on_compile()
        const bool compiled;             //!< flag

    protected:
        explicit   compilable() throw();   //!< setup
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(compilable);
        virtual void on_compile() = 0;
    };
}

#endif

