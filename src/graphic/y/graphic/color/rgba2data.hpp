
#ifndef Y_GRAPHIC_COLOR_RGBA2DATA_INCLUDED
#define Y_GRAPHIC_COLOR_RGBA2DATA_INCLUDED 1

#include "y/graphic/convert.hpp"

namespace upsylon {

    namespace Graphic {
        
        class RGBA2Data
        {
        public:
            virtual ~RGBA2Data() throw();

            virtual void operator()( void *data, const RGBA &C ) throw() = 0;

        protected:
            explicit RGBA2Data() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(RGBA2Data);
        };

        template <typename T>
        class RGBAto : public RGBA2Data
        {
        public:
            inline virtual ~RGBAto() throw() {}
            inline explicit RGBAto() throw() : RGBA2Data() {}

            inline virtual void operator()( void *data, const RGBA &C ) throw()
            {
                assert(data);
                *static_cast<T*>(data) = Convert::Get<T,RGBA>(C);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(RGBAto);
        };


    }

}

#endif

