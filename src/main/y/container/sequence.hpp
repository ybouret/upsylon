//! \file
#ifndef Y_SEQUENCE_INCLUDED
#define Y_SEQUENCE_INCLUDED 1

#include "y/container/container.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    //! a sequence of objects
    template <typename T>
    class sequence : public container
    {
    public:
        Y_DECL_ARGS(T,type); //!< aliases
        //! dectructor
        inline virtual ~sequence() throw() {}

        //! push an object at the back of the sequence
        virtual void push_back( param_type ) = 0;
        
        //! push an object at the front of the sequence
        virtual void push_front( param_type ) = 0;
        
    protected:
        //! constructor
        explicit sequence() throw() : container() {}
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(sequence);
    };
}

#endif
