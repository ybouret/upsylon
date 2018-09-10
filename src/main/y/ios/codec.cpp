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

