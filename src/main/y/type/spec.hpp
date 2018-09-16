// \file
#ifndef Y_TYPE_SPEC_INCLUDED
#define Y_TYPE_SPEC_INCLUDED 1

#include "y/string.hpp"
#include "y/ptr/arc.hpp"
#include <typeinfo>

namespace upsylon
{

    //! type info wrapper
    class type_spec
    {
    public:
        const std::type_info       &info; //!< system type info
        const arc_ptr<const string> name; //!< shared lname

        virtual ~type_spec() throw();                 //!< destructor
        type_spec( const std::type_info & );          //!< intialize
        type_spec( const type_spec &other ) throw();  //!< copy
        
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

        //! return *name
        const string & key() const throw();

        //! display
        friend inline std::ostream & operator<<( std::ostream &os, const type_spec &ts )
        {
            os << '<' << *(ts.name) << '>';
            return os;
        }

    private:
        Y_DISABLE_ASSIGN(type_spec);
    };

}

#endif

