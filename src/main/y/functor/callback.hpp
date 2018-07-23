//! \file
#ifndef Y_FUNCTOR_CALLBACK_INCLUDED
#define Y_FUNCTOR_CALLBACK_INCLUDED 1

#include "y/functor/call.hpp"

namespace upsylon {

    namespace core
    {

        //! wrapper for a callable entity
        /**
         * this can be a C function or a functionoid
         *
         */
        template <typename R, typename TLIST, typename FUNCTION>
        class callback : public virtual callable<R,TLIST>
        {
        public:
            typedef callable<R,TLIST> callable_type; //!< alias

            //! constructor by copy
            explicit callback( const FUNCTION &fn ) : function_( fn ) {}

            //! desctructor
            virtual ~callback() throw()    {}
            //! cloneable interface: use copy constructor
            virtual callable<R,TLIST> *clone() const { return new callback( *this ); }

            Y_FUNCTOR_PARAMETERS(); //!< alias for parameters

            //! no argument call
            inline R operator()(void)
            {
                return function_();
            }

            //! one argument call
            inline R operator()( param1 P1 )
            {
                return function_( P1 );
            }

            //! two arguments call
            inline R operator()( param1 P1, param2 P2 )
            {
                return function_( P1, P2 );
            }

            //! three arguments call
            inline R operator()( param1 P1, param2 P2, param3 P3)
            {
                return function_( P1, P2, P3 );
            }

            //! four arguments call
            inline R operator()( param1 P1, param2 P2, param3 P3, param4 P4)
            {
                return function_( P1, P2, P3, P4);
            }


        private:
            Y_DISABLE_ASSIGN(callback);
            //! use the copy semantics of FUNCTION to clone
            explicit callback( const callback &other ) : callable_type(), function_( other.function_ ) {}
            FUNCTION function_;
        };

    }

}

#endif

