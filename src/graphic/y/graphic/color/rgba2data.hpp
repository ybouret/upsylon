//! \file

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
            void operator()( void *data, const RGBA &C ) throw();

        protected:
            explicit RGBA2Data() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(RGBA2Data);
            virtual void put( void *data, const RGBA &C) throw() = 0;
        };

        //! default wrapper
        template <typename T>
        class PutRGBA : public RGBA2Data
        {
        public:
            inline virtual ~PutRGBA() throw() {}                //!< cleanup
            inline explicit PutRGBA() throw() : RGBA2Data() {}  //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(PutRGBA);
            //! use default encoded conversion
            inline virtual void put( void *data, const RGBA &C ) throw()
            {
                assert(data);
                *static_cast<T*>(data) = Convert::Get<T,RGBA>(C);
            }
        };


    }

}

#endif

