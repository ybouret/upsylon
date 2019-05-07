//! \file
#ifndef Y_FUNCTOR_COMMAND_INCLUDED
#define Y_FUNCTOR_COMMAND_INCLUDED 1

#include "y/functor/call.hpp"

namespace upsylon {

    namespace core {

        //! wrapper for host + method
        template <
        typename R,
        typename TLIST,
        typename OBJECT_POINTER,
        typename METHOD_POINTER >
        class command : public virtual callable<R,TLIST>
        {
        public:
            typedef callable<R,TLIST> callable_type; //!< alias

            //! construct from host+method
            explicit command( const OBJECT_POINTER o, const METHOD_POINTER m ) :
            object(),
            callable_type(),
            object_( o ),
            method_( m )
            {
                assert( NULL != object_ );
                assert( NULL != method_ );
            }
            //! destructor
            virtual ~command() throw() {}

            //! cloneable interface
            virtual  callable<R,TLIST> *clone() const { return new command( *this ); }

            //! parameters aliases
            Y_FUNCTOR_PARAMETERS();

            //! no argument call
            inline R operator()(void)
            {
                assert( NULL != object_ );
                assert( NULL != method_ );
                return ( (*object_).*method_ )();
            }

            //! one argument call
            inline R operator()( param1 P1 )
            {
                assert( NULL != object_ );
                assert( NULL != method_ );
                return ( (*object_).*method_ )( P1 );
            }

            //! two arguments call
            inline R operator()( param1 P1, param2 P2 )
            {
                assert( NULL != object_ );
                assert( NULL != method_ );
                return ( (*object_).*method_ )( P1, P2 );
            }

            //! three arguments call
            inline R operator()( param1 P1, param2 P2, param3 P3)
            {
                assert( NULL != object_ );
                assert( NULL != method_ );
                return ( (*object_).*method_ )( P1, P2, P3 );
            }

            //! four arguments call
            inline R operator()( param1 P1, param2 P2, param3 P3, param4 P4)
            {
                assert( NULL != object_ );
                assert( NULL != method_ );
                return ( (*object_).*method_ )( P1, P2, P3, P4);
            }


        private:
            Y_DISABLE_ASSIGN(command);
            OBJECT_POINTER object_;
            METHOD_POINTER method_;

            //! copy for the clone function
            explicit command( const command &other ) throw():
            object(),
            callable_type(),
            object_( other.object_ ),
            method_( other.method_ )
            {

            }
        };


    }

}

#endif

