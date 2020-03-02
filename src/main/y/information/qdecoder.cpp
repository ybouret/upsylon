
#include "y/information/qdecoder.hpp"

namespace upsylon {

    namespace Information {

        QDecoder:: ~QDecoder() throw()
        {}


        QDecoder:: QDecoder() throw() : IOBits::Broker()
        {
        }
    }

    namespace Information {

        QReader:: QReader() throw() : QDecoder()
        {
        }

        QReader:: ~QReader() throw()
        {
        }


        void QReader:: startBits() throw()
        {
        }


        QDecoder::Status QReader::queryBits(char &C, IOBits &Q)
        {
            if( Q.size() < 8 ) return Partial; else
            {
                C = Q.pop<char>();
                return Success;
            }
        }

        Y_INFORMATION_BROKER(QReader);
        
    }

}

