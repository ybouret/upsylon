//! \file
#ifndef Y_DIGEST_INCLUDED
#define Y_DIGEST_INCLUDED 1

#include "y/string.hpp"

namespace upsylon
{


    //! fixed length buffer
    class digest : public memory::rw_buffer, public counted
    {
    public:

        explicit digest( const size_t n );            //!< a zero digest with n bytes
        digest( const digest & );                     //!< copy
        virtual ~digest() throw();                    //!< destructor
        digest & operator=( const digest & ) throw(); //!< assign WITH TRUNCATION


        void ldz() throw();                         //!< set to zero
        virtual const void *ro()     const throw(); //!< byte
        virtual size_t      length() const throw(); //!< size

        uint8_t &       operator[]( const size_t i ) throw();       //!< 0..size-1
        const uint8_t & operator[]( const size_t i ) const throw(); //!< 0..size-1

        //! display
        std::ostream & display( std::ostream &os ) const;

        //! output
        inline friend std::ostream & operator<<( std::ostream &os, const digest &d )
        {
            return d.display(os);
        }

        //! set hexadecimal value
        static digest hexadecimal(const char *text);
        
        //! randomize the value
        void rand() throw();

        const size_t size; //!< number of bytes
    private:
        size_t   blen;
        uint8_t *byte;
    };

}

#endif

