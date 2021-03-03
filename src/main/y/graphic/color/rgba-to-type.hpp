
//! \file

#ifndef Y_COLOR_RGBA_TO_TYPE_INCLUDED
#define Y_COLOR_RGBA_TO_TYPE_INCLUDED 1

#include "y/graphic/color/convert.hpp"

namespace upsylon
{
    namespace graphic
    {
        class rgba_to_type : public  converter
        {
        public:
            virtual ~rgba_to_type() throw();

            //! convert address
            virtual void operator()(void *, const rgba) const throw() = 0;

        protected:
            explicit rgba_to_type() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(rgba_to_type);
        };


        template <typename T>
        class put_rgba : public rgba_to_type
        {
        public:
            explicit put_rgba() throw() : rgba_to_type() {}
            virtual ~put_rgba() throw() {}
            
            inline virtual size_t depth() const throw() { return sizeof(T); }
            inline virtual void operator()(void *addr, const rgba c) const throw()
            {
                assert(addr);
                *static_cast<T *>(addr) = convert<T,rgba>::from(c);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(put_rgba);
        };
    }
}


#endif
