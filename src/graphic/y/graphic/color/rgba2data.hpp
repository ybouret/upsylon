
#ifndef Y_GRAPHIC_COLOR_RGBA2DATA_INCLUDED
#define Y_GRAPHIC_COLOR_RGBA2DATA_INCLUDED 1

#include "y/graphic/convert.hpp"

namespace upsylon {

    namespace Graphic {

        //! base class for RGBA to something conversion
        class RGBA2Data
        {
        public:
            virtual ~RGBA2Data() throw(); //!< cleanup

            //! write data at a valid memory
            virtual void operator()( void *data, const RGBA &C ) throw() = 0;

        protected:
            explicit RGBA2Data() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(RGBA2Data);
        };

        //! default wrapper
        template <typename T>
        class RGBAto : public RGBA2Data
        {
        public:
            inline virtual ~RGBAto() throw() {}                //!< cleanup
            inline explicit RGBAto() throw() : RGBA2Data() {}  //!< setup

            //! use default encoded conversion
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

