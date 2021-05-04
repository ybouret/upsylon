//! \file

#ifndef Y_IOS_SCRIBE_INCLUDED
#define Y_IOS_SCRIBE_INCLUDED 1

#include "y/string.hpp"
#include "y/type/rtti.hpp"

namespace upsylon
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! base class to convert a type into a string
        //
        //______________________________________________________________________
        class scribe : public object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char * const CLSID; //!< "ios::scribe"

            //__________________________________________________________________
            //
            //  virtual interface
            //__________________________________________________________________
            virtual       ~scribe() throw();               //!< cleanup
            virtual string write(const void *) const = 0;  //!< virtual interface

            //__________________________________________________________________
            //
            // non-virtual interface
            //__________________________________________________________________
            const   rtti  &key() const throw(); //!< for intr_ptr

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const rtti  &info; //!< unique type info
            const size_t dims; //!< number of components: 0=infosize%dims

        protected:
            explicit scribe(const std::type_info &tid,
                            const size_t          num); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(scribe);

        public:

            //__________________________________________________________________
            //
            // API
            //__________________________________________________________________

            //! get a scribe by type info
            static const scribe &query(const std::type_info &tid);

            //! get a scribe by type info, wrapper
            template <typename T> inline
            static const scribe &query() { return query( typeid(T) ); }

            //! write any, wrapper
            template <typename T> inline
            static string conv(const T &data)
            {
                static const scribe &self = query<T>();
                return self.write(&data);
            }

            //! print to ostream
            template <typename OSTREAM, typename T> inline
            static OSTREAM &print(OSTREAM &os, const T &data)
            {
                const string str = conv(data);
                return (os<<str);
            }
        };

        
    }
}

#endif
