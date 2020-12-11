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
        class comm_byte
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef memory::magazine<comm_byte> supply;     //!< alias
            typedef supply::list_               list_type;  //!< alias

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static supply & instance();          //!< singleton instance
            static supply & location() throw();  //!< singleton instance

            //! display as visible
            friend std::ostream & operator<<(std::ostream &os, const comm_byte &b);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            uint8_t       code; //!< held byte
            comm_byte    *next; //!< for list
            comm_byte    *prev; //!< for list


            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(comm_byte);
            comm_byte(const uint8_t) throw();
            comm_byte()  throw();
            ~comm_byte() throw();
            friend class memory::magazine<comm_byte>;
        };



    }

    //! magazine of byte declaration
    Y_MAGAZINE_DECL(net::comm_byte);

}


#endif
