//! \file
#ifndef Y_STATIC_CHECK_INCLUDED
#define Y_STATIC_CHECK_INCLUDED  1

namespace upsylon
{
    //! compile_time_check for true values
    template <bool> struct compile_time_check {
        compile_time_check(...); //!< accept any argument
    };

    //! will failed since constructor is not defined
    template <> struct compile_time_check<false> {};

    //! implement a local static check
#define Y_STATIC_CHECK(expr,msg)                 \
/*    */    {                                    \
/*    */        class ERROR_##msg {};            \
/*    */        (void)sizeof( (upsylon::compile_time_check<(expr)==true>(ERROR_##msg())) );\
/*    */    }

}

#endif
