//! \file

#ifndef Y_COLOR_TYPE_TO_RGBA_INCLUDED
#define Y_COLOR_TYPE_TO_RGBA_INCLUDED 1

#include "y/color/convert.hpp"

namespace upsylon
{
    class type_to_rgba : public color_converter
    {
    public:
        virtual ~type_to_rgba() throw();

        //! convert address
        virtual rgba operator()(const void *) const throw() = 0;

    protected:
        explicit type_to_rgba() throw();

    private:
        Y_DISABLE_COPY_AND_ASSIGN(type_to_rgba);
    };

    template <typename T>
    class get_rgba : public type_to_rgba
    {
    public:
        inline virtual ~get_rgba() throw() {}
        inline explicit get_rgba() throw() : type_to_rgba() {}

        inline virtual size_t depth() const throw() { return sizeof(T); }
        inline virtual rgba   operator()(const void *addr) const throw()
        {
            assert(addr);
            return convert_color<rgba,T>::from( *static_cast<const T *>(addr) );
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(get_rgba);
    };
}


#endif
