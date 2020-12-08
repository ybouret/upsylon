//! \file
#ifndef Y_NET_COMM_BYTE_INCLUDED
#define Y_NET_COMM_BYTE_INCLUDED 1


#include "y/memory/magazine.hpp"

namespace upsylon
{
    namespace net
    {
        //______________________________________________________________________
        //
        //
        //! single dynamic byte
        //
        //______________________________________________________________________
        class io_byte
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef memory::magazine<io_byte> supply;     //!< alias
            typedef supply::list_             list_type;  //!< alias

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static supply & instance();          //!< singleton instance
            static supply & location() throw();  //!< singleton instance

            //! display as visible
            friend std::ostream & operator<<(std::ostream &os, const io_byte &b);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const uint8_t code; //!< held byte
            io_byte      *next; //!< for list
            io_byte      *prev; //!< for list


            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(io_byte);
            io_byte(const uint8_t) throw();
            ~io_byte() throw();
            friend class memory::magazine<io_byte>;
        };



    }

    //! magazine of byte declaration
    Y_MAGAZINE_DECL(net::io_byte);

}


#endif
