//! \file
#ifndef Y_FUNCTOR_CALL_INCLUDED
#define Y_FUNCTOR_CALL_INCLUDED 1


#include "y/ptr/counted.hpp"
#include "y/type/traits.hpp"

namespace upsylon
{

    namespace core
    {

        //==============================================================
        //
        // callable interface
        //
        //==============================================================

        //! callable skeleton
#define Y_CALLABLE_PROTOTYPE()            \
private:                                  \
Y_DISABLE_COPY_AND_ASSIGN(callable);      \
public:                                   \
explicit callable() throw() : object(), counted() {} \
virtual ~callable() throw() {}            \
virtual callable *clone() const = 0


        //! memory management
#define Y_CALLABLE_OBJECT() : public virtual object, public counted
        //! defines the behavior of a generic callable object
        /**
         the parameters are send thru a typelist
         */
        template <typename R,class TLIST>
        class callable Y_CALLABLE_OBJECT()
        {
            Y_CALLABLE_PROTOTYPE();
        };

        //! no argument callable type
        template <typename R>
        class callable<R,null_type> Y_CALLABLE_OBJECT()
        {
            Y_CALLABLE_PROTOTYPE();
            virtual R operator() (void) = 0; //!< interface for no argument call
        };

        //! one argument callable type
        template <typename R, typename P1>
        class callable<R,TL1(P1)> Y_CALLABLE_OBJECT()
        {
            Y_CALLABLE_PROTOTYPE();
            virtual R operator()( typename type_traits<P1>::parameter_type ) = 0;
        };

        //! two arguments callable type
        template <typename R, typename P1, typename P2>
        class callable<R,TL2(P1,P2)> Y_CALLABLE_OBJECT()
        {
            Y_CALLABLE_PROTOTYPE();
            virtual R operator()(typename type_traits<P1>::parameter_type,
                                 typename type_traits<P2>::parameter_type)= 0;
        };

        //! three arguments callable type
        template <typename R, typename P1, typename P2, typename P3>
        class callable<R,TL3(P1,P2,P3)> Y_CALLABLE_OBJECT()
        {
            Y_CALLABLE_PROTOTYPE();
            virtual R operator()(typename type_traits<P1>::parameter_type,
                                 typename type_traits<P2>::parameter_type,
                                 typename type_traits<P3>::parameter_type )= 0;
        };

        //! four arguments callable type
        template <typename R, typename P1, typename P2, typename P3, typename P4>
        class callable<R,TL4(P1,P2,P3,P4)> Y_CALLABLE_OBJECT()
        {
            Y_CALLABLE_PROTOTYPE();
            virtual R operator()(typename type_traits<P1>::parameter_type,
                                 typename type_traits<P2>::parameter_type,
                                 typename type_traits<P3>::parameter_type,
                                 typename type_traits<P4>::parameter_type
                                 ) = 0;
        };

        //! compute parameters aliases
#define Y_FUNCTOR_PARAMETERS() \
typedef typename type_traits< typename tl::safe_type_at<TLIST,0,empty_type>::result>::parameter_type param1;\
typedef typename type_traits< typename tl::safe_type_at<TLIST,1,empty_type>::result>::parameter_type param2;\
typedef typename type_traits< typename tl::safe_type_at<TLIST,2,empty_type>::result>::parameter_type param3;\
typedef typename type_traits< typename tl::safe_type_at<TLIST,3,empty_type>::result>::parameter_type param4


    } // core

} // yocto

#endif
