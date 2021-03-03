//! \file

#ifndef Y_COLOR_TYPE_TO_RGBA_INCLUDED
#define Y_COLOR_TYPE_TO_RGBA_INCLUDED 1

#include "y/graphic/color/convert.hpp"

namespace upsylon
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! type to rgba conversion
        //
        //______________________________________________________________________
        class type_to_rgba : public converter
        {
        public:
            virtual ~type_to_rgba() throw(); //!< cleanup

            //! convert (valid) address
            virtual rgba operator()(const void *) const throw() = 0;

        protected:
            explicit type_to_rgba() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(type_to_rgba);
        };

        //______________________________________________________________________
        //
        //
        //! default conversion
        //
        //______________________________________________________________________
        template <typename T>
        class get_rgba : public type_to_rgba
        {
        public:
            inline virtual       ~get_rgba() throw() {}                       //!< cleanup
            inline explicit       get_rgba() throw() : type_to_rgba() {}      //!< setup
            inline virtual size_t depth() const throw() { return sizeof(T); } //!< source depth

            //! use generic functions
            inline virtual rgba   operator()(const void *addr) const throw()
            {
                assert(addr);
                return convert<rgba,T>::from( *static_cast<const T *>(addr) );
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(get_rgba);
        };
    }
}


#endif
