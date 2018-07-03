//! \file
#ifndef Y_STATIC_CHECK_INCLUDED
#define Y_STATIC_CHECK_INCLUDED  1

namespace upsylon
{
    template <bool> struct compile_time_check {
        compile_time_check(...);
    };

    template <> struct compile_time_check<false> {};

#define Y_STATIC_CHECK(expr,msg)         \
/*    */    {                                    \
/*    */        class ERROR_##msg {};            \
/*    */        (void)sizeof( (upsylon::compile_time_check<(expr)==true>(ERROR_##msg())) );\
/*    */    }

}

#endif
