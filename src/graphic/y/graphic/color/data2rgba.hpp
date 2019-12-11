
//! \file

#ifndef Y_GRAPHIC_COLOR_DATA2RGBA_INCLUDED
#define Y_GRAPHIC_COLOR_DATA2RGBA_INCLUDED 1

#include "y/graphic/convert.hpp"

namespace upsylon {

    namespace Graphic {

        //! Data->RGBA interface
        class Data2RGBA : public Object
        {
        public:
            virtual ~Data2RGBA() throw(); //!< cleanup

            rgba operator()( const void *addr ) throw(); //!< data at address, compute rgba

        protected:
            explicit Data2RGBA() throw(); //!< setyp

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Data2RGBA);
            virtual rgba get(const void *addr) throw() = 0;
        };


        //! generic Get RHNS
        template <typename T>
        class GetRGBA : public Data2RGBA
        {
        public:
            inline explicit GetRGBA() throw() {} //!< setup
            inline virtual ~GetRGBA() throw() {} //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(GetRGBA);

            virtual rgba get(const void *addr) throw()
            {
                return Convert::Get<rgba,T>( *static_cast<const T*>(addr) );
            }
        };

        //! extract some channels
        template <typename T,const size_t CHANNEL>
        class GetChannel : public Data2RGBA
        {
        public:
            inline explicit GetChannel() throw() {} //!< setup
            inline virtual ~GetChannel() throw() {} //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(GetChannel);

            virtual rgba get(const void *addr) throw()
            {
                static const rgba mask( (0==CHANNEL) ? 0xff : 0 );
                rgba C = Convert::Get<rgba,T>( *static_cast<const T*>(addr) );
                C.r &= mask.r;
                C.g &= mask.g;
                C.b &= mask.b;
                return C;
            }
        };

    }

}

#endif

