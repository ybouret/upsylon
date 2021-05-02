//! \file
#ifndef Y_MEMORY_BUFFER_INCLUDED
#define Y_MEMORY_BUFFER_INCLUDED 1

#include "y/os/platform.hpp"
#include "y/ios/case.hpp"

namespace upsylon
{
    namespace memory
    {
        //______________________________________________________________________
        //
        //
        //! read-only buffer
        //
        //______________________________________________________________________
        class ro_buffer
        {
        public:
            //------------------------------------------------------------------
            // virtual interface
            //------------------------------------------------------------------
            virtual            ~ro_buffer()             throw();     //!< destructor
            virtual const void *ro()              const throw() = 0; //!< read only memory of length()
            virtual size_t      length()          const throw() = 0; //!< valid length() byte

            //------------------------------------------------------------------
            // non virtual interface
            //------------------------------------------------------------------
            uint8_t               byte_at(size_t i) const throw();                     //!< virtual never ending byte access
            const char           *hexa_at(const size_t, const CharCase) const throw(); //!< for code
            
            //! see as<T>
            template <typename T> inline
            const T *as() const throw() { return static_cast<const T *>( ro() ); }

            //! count how many T are possible in this buffer
            template <typename T> inline
            size_t   count() const throw() { return length() / sizeof(T); }

            //! check all zero
            bool is_zeroed() const throw();

            //! template to display
            template <typename OSTREAM> inline
            OSTREAM & print_to( OSTREAM &os, const CharCase t=Lowercase ) const
            {
                const size_t n = length();
                for(size_t i=0;i<n;++i)
                {
                    os << hexa_at(i,t);
                }
                return os;
            }
            
        protected:
            explicit ro_buffer() throw();                            //!< constructor

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ro_buffer);
        };

        //______________________________________________________________________
        //
        //
        //! read-write buffer
        //
        //______________________________________________________________________
        class rw_buffer : public ro_buffer
        {
        public:
            virtual ~rw_buffer() throw(); //!< destructor
            void *   rw()        throw(); //!< assuming writable area
            void     reverse()   throw(); //!< using mreverse

            //! see as<T>
            template <typename T> inline
            T *as() throw() { return static_cast<T *>( rw() ); }
            
        protected:
            explicit rw_buffer() throw(); //!< constructor

        private:
            Y_DISABLE_COPY_AND_ASSIGN(rw_buffer);
        };
    }
}

#endif

