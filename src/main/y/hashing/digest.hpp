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

        explicit digest( const size_t n, const uint8_t b=0 ); //!< a zero digest with n bytes equal to b
        digest( const digest & );                     //!< copy
        virtual ~digest() throw();                    //!< destructor
        digest & operator=( const digest & ) throw(); //!< assign WITH TRUNCATION
        digest & operator=( const memory::ro_buffer &) throw(); //!< assign WITH TRUNCATION

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
        static digest hex(const char *txt, const size_t len);
        static digest hex(const char *text);
        static digest hex( const string &);

        //! randomize the value
        void rand() throw();

        const size_t size; //!< number of bytes

        //! compare byte-wise result
        friend bool operator==( const digest &lhs, const digest &rhs) throw();

        //! compare to hexadecimal value
        inline bool equals_hex(const char   *txt, const size_t len) const
        {
            const digest &lhs = *this;
            const digest  rhs = hex(txt,len);
            return lhs==rhs;
        }

        inline bool equals_hex(const char   *s) const
        {
            const digest &lhs = *this;
            const digest  rhs = hex(s);
            return lhs==rhs;
        }

        inline bool equals_hex(const string &s) const
        {
            const digest &lhs = *this;
            const digest &rhs = hex(s);
            return lhs==rhs;
        }

        //! test against hexadecimal value
        inline friend bool operator==(const digest &lhs, const char *txt) { return lhs.equals_hex(txt); }

        //! test against hexadecimal value
        inline friend bool operator==(const char *txt,  const digest &rhs) { return rhs.equals_hex(txt); }

        //! testing against hexadecimal value
        inline friend bool operator==(const digest &lhs, const string &txt) { return lhs.equals_hex(txt); }

        //! testing against hexadecimal value
        inline friend bool operator==(const string &txt,  const digest &rhs) { return rhs.equals_hex(txt); }

        //! set value, assuming same size
        void _set( const digest &other ) throw();

        //! byte-wise XOR, assuming same sizes
        void _xor( const digest &other ) throw();

        //! byte-size set to lhs XOR rhs, assuming same sizes
        void _xor( const digest &lhs, const digest &rhs ) throw();

        //! byte-wyse swap, assuming same sizes
        void _swp( digest &other ) throw();

        //! increase, with overflow
        void _inc( const uint8_t delta ) throw();

    private:
        size_t   blen;
        uint8_t *byte;

    };

}

#endif

