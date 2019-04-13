//! \file
#ifndef Y_IOS_SERIALIZE_INCLUDED
#define Y_IOS_SERIALIZE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    namespace hashing
    {
        class function; //!< forward declaration
    };

    namespace ios
    {
        class ostream; //!< forward declaration

        //! base class for portable binary object serialization
        class serializable
        {
        public:
            virtual ~serializable() throw(); //!< destructor

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual const char *className() const throw() = 0;           //!< class name, should be unique
            virtual size_t      serialize( ostream &fp ) const = 0; //!< write binary data and return written bytes

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            size_t serialize_length() const;                    //!< serialize(/dev/null)
            size_t serialize_className( ostream &fp ) const;    //!< emit className
            size_t serialize_className_length() const;          //!< serialize_class_name(/dev/null)
            size_t serialize_class( ostream &fp ) const;        //!< emit className+data
            size_t serialize_class_length() const;              //!< serialize_class(/dev/null)

            size_t hash_className_with( hashing::function &H ) const; //!< serialize_class_name( ohstream(H) )
            size_t hash_with( hashing::function &H ) const;           //!< serialize( ohstream(H) )
            size_t hash_class_with( hashing::function &H) const;      //!< serialize_class( ohstream(H) )
            
        protected:
            explicit serializable() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(serializable);
        };

    }
    
}

#endif

