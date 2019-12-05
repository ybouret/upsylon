
//! \file

#ifndef Y_GRAPHIC_COLOR_DATA2RGBA_INCLUDED
#define Y_GRAPHIC_COLOR_DATA2RGBA_INCLUDED 1

#include "y/graphic/convert.hpp"

namespace upsylon {

    namespace Graphic {

        class Data2RGBA : public Object
        {
        public:
            virtual ~Data2RGBA() throw();

            RGBA operator()( const void *addr ) throw();

        protected:
            explicit Data2RGBA() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Data2RGBA);
            virtual RGBA get(const void *addr) throw() = 0;
        };


        template <typename T>
        class GetRGBA : public Data2RGBA
        {
        public:
            inline explicit GetRGBA() throw() {}
            inline virtual ~GetRGBA() throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(GetRGBA);

            virtual RGBA get(const void *addr) throw()
            {
                return Convert::Get<RGBA,T>( *static_cast<const T*>(addr) );
            }
        };

        template <typename T,const size_t CHANNEL>
        class GetChannel : public Data2RGBA
        {
        public:
            inline explicit GetChannel() throw() {}
            inline virtual ~GetChannel() throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(GetChannel);

            virtual RGBA get(const void *addr) throw()
            {
                static const RGBA mask( (0==CHANNEL) ? 0xff : 0 );
                RGBA C = Convert::Get<RGBA,T>( *static_cast<const T*>(addr) );
                C.r &= mask.r;
                C.g &= mask.g;
                C.b &= mask.b;
                return C;
            }
        };

    }

}

#endif

