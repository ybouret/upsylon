//! \file

#ifndef Y_TENSOR_LOOPS_INCLUDED
#define Y_TENSOR_LOOPS_INCLUDED 1

#include "y/counting/mloop.hpp"
#include "y/tensor/tensor.hpp"


namespace upsylon {

    //! base type for loops on tensors
    typedef  core::mloop<size_t> tensor_loop_type;

    //! setup loop from tensor dimensions
    class tensor_loop : public tensor_loop_type
    {
    public:
        explicit tensor_loop( const core::tensor &t ); //!< setup(lower,upper)
        virtual ~tensor_loop() throw();                //!< cleanup
        tensor_loop(const tensor_loop &);              //!< copy

    private:
        Y_DISABLE_ASSIGN(tensor_loop);
    };


}

#endif

