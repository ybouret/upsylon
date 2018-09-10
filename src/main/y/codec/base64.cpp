#include "y/codec/base64.hpp"

namespace upsylon
{
    namespace ios
    {

        const char base64::encode_std[64] =
        {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', // 26
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', // 52
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
        };

        const char base64::encode_url[64] =
        {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', // 26
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', // 52
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_'
        };

        base64:: encoder:: encoder(bool use_pad, bool use_url) throw() :
        q_codec(),
        flag(wait0),
        data(),
        table(use_url?encode_url:encode_std),
        pad(use_pad)
        {
            memset(data,0,sizeof(data));
        }


        base64:: encoder:: ~encoder() throw()
        {
        }
        
        void base64:: encoder:: reset() throw()
        {
            flag = wait0;
            memset(data,0,sizeof(data));
        }

        void base64:: encoder:: write(char C)
        {
            switch(flag)
            {
                case wait0:
                    data[0] = C;
                    flag=wait1;
                    break;

                case wait1:
                    data[1] = C;
                    flag=wait2;
                    break;

                case wait2:
                    data[2] = C;
                    emit(3);
                    break;
            }
        }


        static inline void encode3to4( uint8_t *out, const uint8_t *data )
        {
            const uint8_t B0 = data[0];
            const uint8_t B1 = data[1];
            const uint8_t B2 = data[2];

            // 6 highest of B0
            out[0] = (B0&0xfc)>>2;

            // 2 lowest of B0 | 4 highest of B1
            out[1] = ( (B0&0x03)<<4) | ( (B1&0xf0) >> 4 );

            // 4 lowest of B1 | 2 highest of B2
            out[2] = ((B1&0x0f) <<2) | ( (B2&0xc0) >> 6);

            // 6 lowest of B2
            out[3] = (B2&0x3f);

            assert(out[0]<64);
            assert(out[1]<64);
            assert(out[2]<64);
            assert(out[3]<64);
        }

        static inline void encode2to3(uint8_t *out, const uint8_t *data )
        {
            const uint8_t B0 = data[0];
            const uint8_t B1 = data[1];

            // 6 highest of B0
            out[0] = (B0&0xfc)>>2;

            // 2 lowest of B0 | 4 highest of B1
            out[1] = ( (B0&0x03)<<4) | ( (B1&0xf0) >> 4 );

            // 4 lowest of B1 | 0
            out[2] = ((B1&0x0f) <<2);



            assert(out[0]<64);
            assert(out[1]<64);
            assert(out[2]<64);
        }

        static inline void encode1to2(uint8_t *out, const uint8_t *data )
        {
            const uint8_t B0 = data[0];


            // 6 highest of B0
            out[0] = (B0&0xfc)>>2;

            // 2 lowest of B0 | 0
            out[1] = ( (B0&0x03)<<4);

            assert(out[0]<64);
            assert(out[1]<64);
        }

        void base64:: encoder:: emit(const size_t n)
        {
            assert(n>0);
            assert(n<=3);
            uint8_t out[4];
            switch(n)
            {
                case 1:
                    encode1to2(out,data);
                    Q.push_back(table[out[0]]);
                    Q.push_back(table[out[1]]);
                    if(pad)
                    {
                        Q.push_back('=');
                        Q.push_back('=');
                    }
                    break;

                case 2:
                    encode2to3(out,data);
                    Q.push_back(table[out[0]]);
                    Q.push_back(table[out[1]]);
                    Q.push_back(table[out[2]]);
                    if(pad)
                    {
                        Q.push_back('=');
                    }
                    break;
                case 3:
                    encode3to4(out,data);
                    Q.push_back(table[out[0]]);
                    Q.push_back(table[out[1]]);
                    Q.push_back(table[out[2]]);
                    Q.push_back(table[out[3]]);
                    break;

                default:
                    break;
            }

            reset();
        }

        void base64:: encoder:: flush()
        {
            switch(flag)
            {
                case wait0:
                    // no nothing
                    break;

                case wait1:
                    emit(1);
                    break;

                case wait2:
                    emit(2);
                    break;
            }
        }

    }

}

