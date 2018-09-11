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

        int base64:: find_in_table(const char *table, const char C) throw()
        {
            assert(table);
            for(int i=0;i<64;++i)
            {
                if(C==table[i]) return i;
            }
            return -1;
        }

        const short base64::decoded[256] =
        {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, 62, -1, 63,
            52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
            -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
            15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, 63,
            -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
        };

    }

}

namespace upsylon
{
    namespace ios
    {

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
            Q.free();
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

#include "y/code/utils.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace ios
    {
        base64:: decoder:: decoder() throw() :
        count(0),
        input()
        {
            memset(input,0xff,sizeof(input));
        }

        base64:: decoder:: ~decoder() throw()
        {
            
        }

        void base64:: decoder:: reset() throw()
        {
            count = 0;
            memset(input,0xff,sizeof(input));
            Q.free();
        }

        void base64::decoder:: write(char C)
        {
            static const char fn[] = "base64::decoder: ";
            assert(count<4);

            if(C=='=')
            {
                switch(count)
                {

                    case 2:
                        assert(-1==input[2]);
                        count=3;
                        break;
                    case 3:
                        assert(-1==input[3]);
                        count=4;
                        break;
                    default:
                        throw exception("%sunexpected '='",fn);
                }
            }
            else
            {
                const int B = decoded[ uint8_t(C) ];
                if(B<0) throw exception("%sinvalid char '%s'", fn, visible_char[uint8_t(C)] );
                input[count++] = uint8_t(B);
            }
            if(count>=4)
            {
                assert(4==count);
                emit();
            }
        }

        void base64:: decoder:: flush()
        {
            emit();
        }

        void base64:: decoder:: emit()
        {
            assert(count<=4);

        }

    }

}

