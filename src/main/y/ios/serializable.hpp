//! \file
#ifndef Y_IOS_SERIALIZE_INCLUDED
#define Y_IOS_SERIALIZE_INCLUDED 1

#include "y/os/platform.hpp"
#include "y/ios/iosfwd.hpp"
#include "y/strfwd.hpp"

namespace upsylon
{

    class digest;
    
    namespace hashing
    {
        class function; //!< forward declaration
     };

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
            virtual const char *className()  const throw() = 0; //!< class name, should be unique
            virtual size_t      serialize(ostream &) const = 0; //!< write binary data and return written bytes

            //__________________________________________________________________
            //
            // non virtual interface : helpers
            //__________________________________________________________________
            size_t serialize_length()             const; //!< serialize(/dev/null)
            size_t serialize_className(ostream &) const; //!< emit className
            size_t serialize_className_length()   const; //!< serialize_class_name(/dev/null)
            size_t serialize_class(ostream &)     const; //!< emit className+data
            size_t serialize_class_length()       const; //!< serialize_class(/dev/null)

            //__________________________________________________________________
            //
            // non virtual interface : hashing
            //__________________________________________________________________
            size_t hash_className_with(hashing::function &) const; //!< serialize_class_name( ohstream(H) )
            size_t hash_with(hashing::function &)           const; //!< serialize( ohstream(H) )
            size_t hash_class_with(hashing::function &)     const; //!< serialize_class( ohstream(H) )
            digest md(hashing::function &) const; //!< H.set(); (void) hash_with(...); return H.md();


            //__________________________________________________________________
            //
            // non virtual interface : conversion
            //__________________________________________________________________
            string to_binary()    const; //!< serialize into a string
            string to_base64()    const; //!< serialize and convert into base64
            string to_visible()   const; //!< serialize to a visible string
            string to_printable() const; //!< serialize to a printable string
            
            size_t save_to(const string &binaryFile, const bool append=false) const; //!< save to file
            size_t save_to(const char   *binaryFile, const bool append=false) const; //!< save to file, wrapper


        protected:
            explicit serializable() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(serializable);
        };

    }
    
}

#endif

