
#include "y/information/translator/base64/encoder.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Information {

        void Base64Encoder:: clear() throw()
        {
            memset(source, 0, sizeof(source));
            memset(target, 0, sizeof(target));
        }

        Base64Encoder:: ~Base64Encoder() throw()
        {
            clear();
        }


        Base64Encoder:: Base64Encoder(const Base64::Mode m)  :
        TranslatorQueue(4),
        mode(m),
        status( waitForByte1 ),
        table( Base64::std_table ),
        source(),
        target()
        {
            clear();
        }


        void Base64Encoder:: reset() throw()
        {
            free();
            clear();
            status = waitForByte1;
        }

        const char * Base64Encoder:: name() const throw()
        {
            return EncoderID;
        }

        const char * Base64Encoder:: family() const throw()
        {
            return Base64::FMID;
        }

        void Base64Encoder:: write(char C)
        {
            switch( status )
            {
                case waitForByte1: source[0] = C; status = waitForByte2; break;
                case waitForByte2: source[1] = C; status = waitForByte3; break;
                case waitForByte3: source[2] = C; status = waitForWrite; emit(false); break;
                case waitForWrite: throw exception("corrupted Base64Encoder");
            }
        }

        void Base64Encoder:: flush()
        {
            emit(true);
        }

        void Base64Encoder:: emit(const bool flushing)
        {
            assert(table);
            switch (status) {
                case waitForByte1:
                    // no stored byte
                    return;

                case waitForByte2:
                    // one stored byte
                    Base64::Encode1to2(target, source, table);
                    push_back(target[0]);
                    push_back(target[1]);
                    if(flushing)
                    {
                        switch(mode)
                        {
                            case Base64::Pad:
                                push_back(Base64::padding);
                                push_back(Base64::padding);
                                break;

                            default:
                                break;
                        }
                    }
                    break;

                case waitForByte3:
                    // two stored bytes
                    Base64::Encode2to3(target, source, table);
                    push_back(target[0]);
                    push_back(target[1]);
                    push_back(target[2]);
                    if(flushing)
                    {
                        switch(mode)
                        {
                            case Base64::Pad:
                                push_back(Base64::padding);
                                break;
                            default: break;
                        }
                    }
                    break;

                case waitForWrite:
                    // three stored bytes
                    Base64::Encode3to4(target, source, table);
                    push_back(target[0]);
                    push_back(target[1]);
                    push_back(target[2]);
                    push_back(target[3]);
                    break;
            }

            status = waitForByte1;
        }
    }

}


