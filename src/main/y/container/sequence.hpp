//! \file
#ifndef Y_SEQUENCE_INCLUDED
#define Y_SEQUENCE_INCLUDED 1

#include "y/container/container.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    template <typename T>
    class sequence : public container
    {
    public:
        Y_DECL_ARGS(T,type);
        inline virtual ~sequence() throw() {}

        virtual void push_back( param_type ) = 0;
        virtual void push_front( param_type ) = 0;
        
    protected:
        explicit sequence() throw() : container() {}
    private:
        Y_DISABLE_COPY_AND_ASSIGN(sequence);
    };
}

#endif
