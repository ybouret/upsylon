//! \file
#ifndef Y_IOS_ISTREAM_INCLUDED
#define Y_IOS_ISTREAM_INCLUDED 1

#include "y/strfwd.hpp"
#include "y/ios/stream.hpp"
#include "y/os/endian.hpp"
#include "y/os/static-check.hpp"
#include "y/ios/gist.hpp"

namespace upsylon
{
    namespace ios
    {

        //! input stream interface
        class istream : public stream
        {
        public:
            //------------------------------------------------------------------
            //
            // virtual interface
            //
            //------------------------------------------------------------------

            //! destructor
            virtual ~istream() throw();

            //! query a new char if any
            virtual bool query( char &C ) = 0;

            //! unread a char
            virtual void store( char C ) = 0;

            //! test at least one more char, default query/store
            virtual bool is_active();

            //------------------------------------------------------------------
            //
            // non-virtual interface
            //
            //------------------------------------------------------------------

            //! get a new line
            bool gets( string &line );

            
            //! read at most buflen
            size_t try_get(void *buffer,const size_t buflen);

            
            //! try to query an integral type in network byte order, with optional COUNT of read bytes
            template <typename T> inline
            bool query_nbo(T &ans)
            {
                const size_t num = try_get(&ans,sizeof(T));
                if(num<sizeof(T))
                {
                    return false;
                }
                else
                {
                    ans = swap_be_as(ans);
                    return true;
                }
            }
            
            
            //! read a packed unsigned, with optional COUNT of read byte
            template <typename T>
            inline bool query_upack(T &ans, size_t *count=NULL)
            {
                Y_STATIC_CHECK(sizeof(T)<=8,T_is_too_large);
                ans = 0;
                uint8_t      store[8]    = { 0,0,0,0,0,0,0,0 };
                uint8_t      prolog      = 0; if(!query_nbo(prolog)) return false;
                size_t       extra_bytes = (prolog&0x0f);
                gist::assign(count,1);
                for(size_t i=0;i<extra_bytes;++i)
                {
                    if(!query_nbo(store[i])) return false;
                    gist::add_to(count,1);
                }
                
                while(extra_bytes-->0)
                {
                    gist::shl8(ans,int2type< (sizeof(T)>1) >());
                    ans |= store[extra_bytes];
                }
                
                {
                    const uint8_t B = uint8_t((prolog&0xf0) >> 4);
                    ans <<= 4;
                    ans |= B;
                }
                return true;
            }

        protected:
            //! constructor
            explicit istream() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(istream);
            static const char upack_prolog[];
            static const char upack_inside[];
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
