//! \file

#ifndef Y_IOS_CONVEYOR_DERIVED_INCLUDED
#define Y_IOS_CONVEYOR_DERIVED_INCLUDED 1

#include "y/ios/conveyor.hpp"
#include "y/type/args.hpp"

namespace upsylon {
    
    namespace ios
    {
        //! conveyor for serializalbe classes
        template <typename T>
        class derived_conveyor : public conveyor
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type); //!< classes
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! setup
            inline explicit derived_conveyor() throw() :
            conveyor(comms::variable,typeid(typename type_traits<T>::mutable_type))
            {}
            
            //! cleanup
            inline virtual ~derived_conveyor() throw() {}

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            
            //! local copy by '='
            inline virtual void copy(void *target, const void *source) const
            {
                assert(target);
                assert(source);
                *static_cast<T*>( target ) = *static_cast<const T *>(source);
            }
            
            //! serialize
            inline virtual size_t save(ios::ostream &target, const void *source) const
            {
                return static_cast<const T*>(source)->serialize(target);
            }
            
            //! based on the existence of a T::read
            inline virtual size_t load(void *target, ios::istream &source) const
            {
                static const char *which = *type_name_of<T>();
                size_t             shift = 0;
                const_type         temp  = mutable_type::read(source,shift,which);
                *static_cast<mutable_type *>(target)  = temp;
                return shift;
            }
            
            virtual const std::type_info &get_type_info() const throw()
            {
                return typeid( typename type_traits<T>::mutable_type );
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(derived_conveyor);
        };
    }
    
}

#endif



