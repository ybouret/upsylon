#include "y/information/translator/base64/decoder.hpp"
#include "y/exception.hpp"
#include "y/code/utils.hpp"

namespace upsylon {

    namespace Information {

        Base64Decoder:: ~Base64Decoder() throw()
        {
            clear();
        }

        const char * Base64Decoder:: name() const throw()
        {
            return DecoderID;
        }

        const char * Base64Decoder:: family() const throw()
        {
            return Base64::FMID;
        }

        void Base64Decoder:: clear() throw()
        {
            memset(target, 0, sizeof(target) );
            memset(source, 0, sizeof(source) );
        }

        void Base64Decoder:: reset() throw()
        {
            free();
            clear();
            status = waitForChar1;
        }

        Base64Decoder:: Base64Decoder() : TranslatorQueue(),
        status( waitForChar1 ),
        source(),
        target()
        {
            clear();
        }


        void Base64Decoder:: write(char C)
        {
            switch( status )
            {
                case waitForChar1: source[0] = C; status=waitForChar2; break;
                case waitForChar2: source[1] = C; status=waitForChar3; break;
                case waitForChar3: source[2] = C; status=waitForChar4; break;
                case waitForChar4: source[3] = C; status=waitForWrite; emit(); break;
                case waitForWrite: throw exception("Base64Decoder corrupted code!");
            }
        }

        void Base64Decoder:: flush()
        {
            emit();
        }

        void Base64Decoder:: emit1()
        {
            Base64::Decode2to1(target,source);
            push_back(target[0]);
        }

        void Base64Decoder:: emit2()
        {
            Base64::Decode3to2(target,source);
            push_back(target[0]);
            push_back(target[1]);
        }

        void Base64Decoder:: emit3()
        {
            Base64::Decode4to3(target,source);
            push_back(target[0]);
            push_back(target[1]);
            push_back(target[2]);
        }

        void Base64Decoder:: emit()
        {
            static const char fn[] = "Base64Decoder";

            switch (status)
            {
                case waitForChar1:
                    // no stored char
                    return;

                case waitForChar2:
                    // one stored char
                    throw exception("%s missing input after '%s'", fn, visible_char[ uint8_t(source[0]) ] );

                case waitForChar3:
                    // two stored char
                    emit1();
                    break;

                case waitForChar4:
                    // three stored char
                    if( Base64::padding == source[2] )
                    {
                        // only two effective chars
                        emit1();
                    }
                    else
                    {
                        emit2();
                    }
                    break;

                case waitForWrite:
                    // four stored char
                    if( Base64::padding == source[3] )
                    {
                        if(Base64::padding == source[2])
                        {
                            // only two effective chars
                            emit1();
                        }
                        else
                        {
                            // only three effective chars
                            emit2();
                        }
                    }
                    else
                    {
                        emit3();
                    }


            }

            status = waitForChar1;
        }
        


    }

}
