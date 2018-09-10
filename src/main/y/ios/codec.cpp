#include "y/ios/codec.hpp"

namespace upsylon
{
    namespace ios
    {

        codec:: ~codec() throw()
        {
        }

        codec:: codec() throw() : istream(), ostream()
        {
        }


        string codec:: to_string( const void *data, const size_t size )
        {
            assert( !(NULL==data&&size>0) );

            reset();
            string      ans;
            char        C = 0;
            const char *p = (const char *)data;
            for(size_t i=size;i>0;--i)
            {
                write(*(p++));
                while(query(C))
                {
                    ans << C;
                }
            }
            flush();
            while(query(C))
            {
                ans << C;
            }
            return ans;
        }


        //////////

        q_codec:: ~q_codec() throw()
        {}

        q_codec:: q_codec() throw() : codec(), Q()
        {
        }

        bool q_codec:: query(char &C)
        {
            if(Q.size())
            {
                C = Q.front();
                Q.pop_front();
                return true;
            }
            else
            {
                return false;
            }
        }

        void q_codec:: store(const char C)
        {
            Q.push_front(C);
        }

    }

}

