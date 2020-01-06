
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
                assert(CHANNEL<4);
                const uint8_t byte = Convert::Get<uint8_t,T>( *static_cast<const T*>(addr) );
                rgba ans;
                ans[CHANNEL] = byte;
                return ans;
            }
        };

        template <typename T>
        class GetRed : public GetChannel<T,0>
        {
        public:
            inline explicit GetRed() throw() : GetChannel<T,0>() {}
            inline virtual ~GetRed() throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(GetRed);
        };

        template <typename T>
        class GetGreen : public GetChannel<T,1>
        {
        public:
            inline explicit GetGreen() throw() : GetChannel<T,1>() {}
            inline virtual ~GetGreen() throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(GetGreen);
        };

        template <typename T>
        class GetBlue : public GetChannel<T,2>
        {
        public:
            inline explicit GetBlue() throw() : GetChannel<T,2>() {}
            inline virtual ~GetBlue() throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(GetBlue);
        };


    }

}

#endif

