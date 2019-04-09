//! \file
#ifndef Y_IOS_SERIALIZE_INCLUDED
#define Y_IOS_SERIALIZE_INCLUDED 1

#include "y/ios/ostream.hpp"

namespace upsylon
{

    namespace ios
    {

        //! base class for portable binary object serialization
        class serializable
        {
        public:
            virtual ~serializable() throw(); //!< destructor

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual const char *className() const throw() = 0;
            virtual size_t      serialize( ios::ostream &fp ) const = 0;

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            size_t serialize_length() const;                       //!< serialize(/dev/null)
            size_t serialize_className( ios::ostream &fp ) const; //!< emit className
            size_t serialize_className_length() const;            //!< serialize_class_name(/dev/null)
            size_t serialize_class( ios::ostream &fp ) const;      //!< emit className+data
            size_t serialize_class_length() const;                 //!< serialize_class(/dev/null)

        protected:
            explicit serializable() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(serializable);
        };

    }
    
}

#endif

