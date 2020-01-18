// \file
#ifndef Y_TYPE_SPEC_INCLUDED
#define Y_TYPE_SPEC_INCLUDED 1

#include "y/string.hpp"
#include "y/type/traits.hpp"
#include <typeinfo>

namespace upsylon
{

    //! type info wrapper
    class type_spec : public counted_object
    {
    public:

        const std::type_info &info;  //!< system type_info
        const string          name_; //!< system name
        const string          name;  //!< human readable name

        const std::type_info &key() const throw();                    //!< info
        explicit              type_spec(const std::type_info &tid);   //!< setup
        explicit              type_spec(const std::type_info &tid,
                                        const string         &known); //!< setup
        virtual              ~type_spec() throw();                    //!< cleanup

        //! forward equality operator
        inline friend bool operator==( const type_spec &lhs, const type_spec &rhs ) throw()
        {
            return lhs.info == rhs.info;
        }

        //! forward difference operator
        inline friend bool operator!=( const type_spec &lhs, const type_spec &rhs ) throw()
        {
            return lhs.info != rhs.info;
        }

        //! use the 'before' API
        inline friend bool operator<(const type_spec &lhs, const type_spec &rhs) throw()
        {
            return lhs.info.before(rhs.info);
        }

        //! display
        friend inline std::ostream & operator<<( std::ostream &os, const type_spec &ts )
        {
            os << '<' << *(ts.name) << '>';
            return os;
        }

        //! declare (possibly multiple) type info
        static const type_spec & of( const std::type_info &tid );

        //! template find type
        template <typename T> static inline
        const type_spec & of(void)
        {
            return of( typeid( typename type_traits<T>::mutable_type ) );
        }

        static void display( std::ostream &os );

    private:
        Y_DISABLE_COPY_AND_ASSIGN( type_spec );
        void setName();
    };


  



}

#endif

