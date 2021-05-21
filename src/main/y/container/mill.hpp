//! \file

#ifndef Y_CONTAINER_MILL_INCLUDED
#define Y_CONTAINER_MILL_INCLUDED 1

#include "y/string.hpp"
#include <typeinfo>

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! base class for a factory of a given type
    //
    //__________________________________________________________________________
    class mill
    {
    public:
        static const at_exit::longevity maximum_life_time; //!< from rtti
        static const char               CLID[];            //!< "factory"
        virtual                        ~mill() throw();    //!< cleanup
        const        string             typeName;          //!< from rtti

    protected:
        explicit mill( const std::type_info &); //!< setup from rtti
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(mill);
    };

}


#endif
