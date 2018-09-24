#include "y/ios/codec.hpp"
#include "y/ios/imstream.hpp"
#include "y/ios/osstream.hpp"

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


        void codec:: filter( ios::ostream &output, ios::istream &input )
        {
            char C=0;
            // read from input
            while( input.query(C) )
            {
                // process
                this->write(C);
                // save to output
                while( this->query(C) )
                {
                    output.write(C);
                }
            }

            // finalize
            this->flush();
            // save to output
            while( this->query(C) )
            {
                output.write(C);
            }
        }

        string codec:: to_string( const void *data, const size_t size )
        {
            assert( !(NULL==data&&size>0) );
            ios::imstream input(data,size);
            string        ans;
            ios::osstream output(ans);

            reset();
            filter(output,input);
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

        void q_codec:: store(char C)
        {
            Q.push_front(C);
        }

    }

}

