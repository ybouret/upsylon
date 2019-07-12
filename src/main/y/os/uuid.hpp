//! \file
#ifndef Y_OS_UUID_INCLUDED
#define Y_OS_UUID_INCLUDED 1

#include "y/strfwd.hpp"
#include "y/memory/buffer.hpp"
#include <iostream>

namespace upsylon
{
    //! universal unique identifier
    class uuid : public memory::ro_buffer
    {
    public:
        static const size_t BITS = 128;       //!< bits
        static const size_t BYTES = BITS>>3;  //!< bytes

        virtual ~uuid() throw();                    //!< destructor
        explicit uuid();                            //!< build from crypto bits
        uuid( const uuid &other ) throw();          //!< copy
        virtual const void *ro() const throw();     //! byte address
        virtual size_t      length() const throw(); //! bytes

        //! output
        friend std::ostream & operator<<( std::ostream &, const uuid &);

        //! same as output
        string to_string() const;


    private:
        uint8_t byte[BYTES];
        Y_DISABLE_ASSIGN(uuid);
    };
}



#endif

