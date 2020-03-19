
#include "y/ios/istream.hpp"
#include "y/string.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{
    namespace ios
    {
        istream:: istream() throw()
        {
        }

        istream:: ~istream() throw()
        {
        }


        bool istream:: is_active()
        {
            char C=0;
            if(query(C))
            {
                store(C);
                return true;
            }
            else
            {
                return false;
            }
        }

        void istream:: input(void        *buffer,
                             const size_t buflen,
                             const char  *when)
        {
            assert(!(0==buffer&&buflen>0));
            char *target = static_cast<char *>(buffer);
            char  C      = 0;

            for(size_t i=0;i<buflen;++i)
            {
                if(!query(C))
                {
                    if(!when) when = "!";
                    throw imported::exception("End Of Input","istream.read(%u/%u) %s", unsigned(i+1), unsigned(buflen), when);
                }
                *(target++) = C;
            }

        }

        size_t istream:: try_get(void *buffer,const size_t buflen)
        {
            assert(!(0==buffer&&buflen>0));
            char *target = static_cast<char *>(buffer);
            char  C      = 0;

            size_t ans = 0;
            for(size_t i=0;i<buflen;++i)
            {
                if(!query(C))
                {
                    break;
                }
                *(target++) = C;
                ++ans;
            }
            return ans;
        }

        
        bool istream:: gets( string &line )
        {
            line.clear();
            return read_line::scan(*this,line) >= 0;
        }

#if defined(__GNUC__)
        const int read_line::failure;
        const int read_line::partial;
#endif

        int read_line:: scan(  istream &in, string &line )
        {
            static const int CR( '\r' );
            static const int LF( '\n' );

            const char  *org = (const char *)line.ro();
            const size_t len = line.length();
            const char  *end = org + len;

            int prev = -1;
            int last = -1;
            switch( len )
            {
                case 0:
                    break;

                case 1:
                    last = end[-1];
                    break;

                default:
                    assert( len >= 2 );
                    prev = end[-2];
                    last = end[-1];
                    break;
            }

            char next(0);
        TEST_STATUS:
            switch( prev )
            {
                    //--------------------------------------------------------------
                    // CR + ?
                    //--------------------------------------------------------------
                case CR:
                    if( last == LF )
                    {
                        line.trim(2);
                        return read_line:: found_crlf; //-- line is ready !
                    }
                    else
                    {
                        in.store( char(last) );
                        line.trim(1);
                        return read_line:: found_cr; //-- line is ready !
                    }

                    //--------------------------------------------------------------
                    // LF + ? => cut already !
                    //--------------------------------------------------------------
                case LF:
                    in.store( char(last) );
                    line.trim(1);
                    return read_line:: found_cr; //-- line is ready !
                    break;


                    //--------------------------------------------------------------
                    // pref is not CR/LF: what about last ?
                    //--------------------------------------------------------------
                default:
                    switch( last )
                {
                        //------------------------------------------------------
                        // xxxx[LF] => fast !
                        //------------------------------------------------------
                    case LF:
                        line.trim(1);
                        return read_line:: found_lf; //-- line is ready !
                        //break;

                        //------------------------------------------------------
                        // xxxx[CR] => try to go forward
                        //------------------------------------------------------
                    case CR:
                        if( !in.query(next) )
                            return read_line:: hangs_on_cr; //-- line is NOT ready !
                        else
                        {
                            line.trim(1); // remove CR
                            if( LF == ptrdiff_t(next) )
                            {
                                // early return !
                                return read_line:: found_crlf; //-- line is ready
                            }
                            else
                            {
                                in.store( next );            //-- for next line
                                return read_line:: found_cr;  //-- this line is ready
                            }
                        }
                        //break;

                    default: //-- forward
                        if( !in.query( next ) )
                            goto EOS;
                        else
                        {
                            line << next;
                            prev = last;
                            last = next;
                            goto TEST_STATUS;
                        }

                }

            }

        EOS:
            return line.size() > 0 ? (read_line:: partial) : (read_line::failure);
        }
    }
}

