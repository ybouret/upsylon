//! \file
#ifndef Y_IOS_ISTREAM_INCLUDED
#define Y_IOS_ISTREAM_INCLUDED 1

#include "y/ios/stream.hpp"
#include "y/string.hpp"
#include "y/os/endian.hpp"

namespace upsylon
{
    namespace ios
    {

        //! input stream interface
        class istream : public stream
        {
        public:
            //! destructor
            virtual ~istream() throw();

            //! query a new char if any
            virtual bool query( char &C ) = 0;

            //! unread a char
            virtual void store( char C ) = 0;

            //! test at least one more char, default query/store
            virtual bool is_active();

            //! get a new line
            bool gets( string &line );

            //! read exactly buflen bytes
            void input(void *buffer,const size_t buflen);

            //! read at most bulen
            size_t try_get(void *buffer,const size_t buflen);

            //! get an integral type
            template <typename T>
            inline T read() { T ans(0); input(&ans,sizeof(T)); return swap_be_as<T>(ans); }

            //! read a packed unsigned
            template <typename T>
            inline T read_upack()
            {
                const size_t prolog      = read<uint8_t>();
                size_t       extra_bytes = check_extra_bytes(prolog&0x0f,sizeof(T));
                T            ans( (prolog&0xf0) >> 4 );
                while( extra_bytes-- > 0 )
                {
                    ans <<= 8;
                    ans  |= T( read<uint8_t>() );
                }
                return ans;
            }

        protected:
            //! constructor
            explicit istream() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(istream);
            size_t check_extra_bytes( const size_t extra_bytes, const size_t sz) const;
        };

        //! read line algorithm
        struct read_line
        {

            static const int failure      = -2; //!< no end of line detected, and no read chars
            static const int partial      = -1; //!< no end of line detected, but some chars are read

            static const int hangs_on_cr  =  0; //!< a CR is detected but currently out of input => KEEP CR and leave the choice to the user

            static const int found_lf     =  1; //!< a LF is detected => remove LF and line is ready
            static const int found_crlf   =  2; //!< a CRLF is detected => remove CRLF and line is ready
            static const int found_cr     =  3; //!< a CR + char is detected => remove CR, unread char and line is ready

            static int scan( istream &in, string &line ); //!< scan input, grow current line

        };
    }
}

#endif
