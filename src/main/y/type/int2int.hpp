//! file
#ifndef Y_TYPE_I2I_INCLUDED
#define Y_TYPE_I2I_INCLUDED 1

#include "y/type/ints.hpp"
#include "y/type/spec.hpp"
#include "y/os/endian.hpp"

namespace upsylon {

    namespace core {

        //! integral to integral common
        struct _i2i
        {
            //! throw on overflow
            static void overflow_exception(const type_spec &target,
                                           const type_spec &source);

            //! throw on negative to positive
            static void negative_exception(const type_spec &target,
                                           const type_spec &source);
        };

        //! converting according to integral type
        template <typename TARGET,typename SOURCE>
        struct i2i {
            static const size_t SIZEOF_TARGET = sizeof(TARGET);                //!< alias
            static const bool   SIGNED_TARGET = is_signed<TARGET>::value;      //!< alias
            static const size_t SIZEOF_SOURCE = sizeof(SOURCE);                //!< alias
            static const bool   SIGNED_SOURCE = is_signed<SOURCE>::value;      //!< alias
            static const bool   LARGER_SOURCE = SIZEOF_SOURCE > SIZEOF_TARGET; //!< alias

            //! triple dispatch function
            static inline
            TARGET _( const SOURCE &source )
            {
                return convert(source,
                               int2type<SIGNED_TARGET>(),
                               int2type<SIGNED_SOURCE>(),
                               int2type<LARGER_SOURCE>());
            }

        private:
            //------------------------------------------------------------------
            //
            //  UNSIGNED TARGET <- UNSIGNED SOURCE
            //
            //------------------------------------------------------------------

            //------------------------------------------------------------------
            // |_sizeof(SOURCE)<=sizeof(TARGET)
            //------------------------------------------------------------------
            static inline
            TARGET convert(const SOURCE &source,
                           int2type<false>,        // SIGNED_TARGET
                           int2type<false>,        // SIGNED_SOURCE
                           int2type<false>         // LARGER_SOURCE
                           ) throw()
            {
                return static_cast<TARGET>( source );
            }

            //------------------------------------------------------------------
            // |_sizeof(SOURCE)>sizeof(TARGET)
            //------------------------------------------------------------------
            static inline
            TARGET convert(SOURCE   source,
                           int2type<false>,        // SIGNED_TARGET
                           int2type<false>,        // SIGNED_SOURCE
                           int2type<true>          // LARGER_SOURCE
            )
            {
                assert(sizeof(SOURCE)>sizeof(TARGET) );
                static const SOURCE source_max = static_cast<SOURCE>( limit_of<TARGET>::maximum );
                if(source>source_max) _i2i:: overflow_exception( type_spec_of<TARGET>(), type_spec_of<SOURCE>() );
                source = swap_le(source);
                return swap_le( *(const TARGET *)&source );
            }

            //------------------------------------------------------------------
            //
            //  SIGNED TARGET <- UNSIGNED SOURCE
            //
            //------------------------------------------------------------------

            //------------------------------------------------------------------
            // |_sizeof(SOURCE)<=sizeof(TARGET)
            //------------------------------------------------------------------
            static inline
            TARGET convert(const SOURCE &source,
                           int2type<true>,         // SIGNED_TARGET
                           int2type<false>,        // SIGNED_SOURCE
                           int2type<false>         // LARGER_SOURCE
            )
            {
                if(source<0) _i2i::negative_exception( type_spec_of<TARGET>(), type_spec_of<SOURCE>() );
                return static_cast<TARGET>( source );
            }

            //------------------------------------------------------------------
            //
            //  SIGNED TARGET <- SIGNED SOURCE
            //
            //------------------------------------------------------------------


            //------------------------------------------------------------------
            // |_sizeof(SOURCE)<=sizeof(TARGET)
            //------------------------------------------------------------------
            static inline
            TARGET convert(const SOURCE &source,
                           int2type<true>,         // SIGNED_TARGET
                           int2type<true>,         // SIGNED_SOURCE
                           int2type<false>         // LARGER_SOURCE
            )
            {
                return static_cast<TARGET>( source );
            }

            //------------------------------------------------------------------
            // |_sizeof(SOURCE)>sizeof(TARGET)
            //------------------------------------------------------------------
            static inline
            TARGET convert(const SOURCE &source,
                           int2type<true>,         // SIGNED_TARGET
                           int2type<true>,         // SIGNED_SOURCE
                           int2type<true>          // LARGER_SOURCE
            )
            {
                static const SOURCE source_max = static_cast<SOURCE>( limit_of<TARGET>::maximum );
                static const SOURCE source_min = static_cast<SOURCE>( limit_of<TARGET>::minimum );
                if(source<source_min||source>source_max) _i2i:: overflow_exception( type_spec_of<TARGET>(), type_spec_of<SOURCE>() );
                return static_cast<TARGET>( source );
            }



        };

    }

    //! top level call by integral type matching
    template <typename TARGET, typename SOURCE>
    inline TARGET i2i( const SOURCE &source )
    {
        typedef typename integral_of<TARGET>::type tgt_t;
        typedef typename integral_of<SOURCE>::type src_t;
        return core::i2i<tgt_t,src_t>::_( (const src_t &)source );
    }



}

#endif

