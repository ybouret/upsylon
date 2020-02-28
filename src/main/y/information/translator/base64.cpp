#include "y/information/translator/base64.hpp"
#include "y/os/error.hpp"

namespace upsylon {

    namespace Information {

        const char Base64:: FMID[] = "Base64";
        
        size_t Base64:: BytesFor(const size_t inputLength, const Mode m) throw()
        {
            const size_t num3 = inputLength/3;
            const size_t rem  = inputLength%3;

            switch(m)
            {
                case Raw:
                    switch(rem)
                    {
                        case 2:  return 4*num3 + 3;
                        case 1:  return 4*num3 + 2;
                        case 0:  return 4*num3;
                    }
                    break;

                case Pad:
                    return (rem>0) ? (4*(num3+1)) : 4*num3;

            }

            fatal_error("Base64::BytesFor(corrupted code)");
            return 0;
        }


        const char Base64::std_table[64] =
        {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
            'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
            'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
            'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
            'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
            'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
            'w', 'x', 'y', 'z', '0', '1', '2', '3',
            '4', '5', '6', '7', '8', '9', '+', '/'
        };

        const char Base64::url_table[64] =
        {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
            'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
            'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
            'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
            'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
            'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
            'w', 'x', 'y', 'z', '0', '1', '2', '3',
            '4', '5', '6', '7', '8', '9', '-', '_'
        };

        const uint8_t Base64::inv_table[256] =
        {
#include "base64-itable.inc"
        };

        namespace {

            //! [6bits|..] -> [00|6bits]
            static inline uint8_t B0toC0( const uint8_t B0 ) throw()
            {
                const uint8_t ans = B0 >> 2; assert(ans<64);
                return ans;
            }

            //! [......|2bits] -> [00|2bits|0000]
            static inline uint8_t B0toC1( const uint8_t B0 ) throw()
            {
                const uint8_t ans = (B0&0x3) << 4; assert(ans<64);
                return ans;
            }

            //! [4bits|....] -> [0000|4bits]
            static inline uint8_t B1toC1( const uint8_t B1 ) throw()
            {
                const uint8_t ans = B1 >> 4; assert(ans<64);
                return ans;
            }

            //! [....|4bits] -> [00|4bits|00]
            static inline uint8_t B1toC2( const uint8_t B1 ) throw()
            {
                const uint8_t ans = (B1&0xf) << 2; assert(ans<64);
                return ans;
            }

            //! [2bits|......] -> [000000|2bits]
            static inline uint8_t B2toC2(const uint8_t B2) throw()
            {
                const uint8_t ans = B2 >> 6; assert(ans<64);
                return ans;
            }

            //! [..|6bits] -> [00|6bits]
            static inline uint8_t B2toC3(const uint8_t B2) throw()
            {
                const uint8_t ans = (B2 & 63); assert(ans<64);
                return ans;
            }

        }


        void Base64:: Encode3to4(void *target, const void *source, const char *table) throw()
        {
            assert(target);
            assert(source);
            assert(table);
            const uint8_t *b = static_cast<const uint8_t *>(source);
            char          *c = static_cast<char          *>(target);

            const uint8_t  B0 = b[0];
            const uint8_t  B1 = b[1];
            const uint8_t  B2 = b[2];
            const unsigned C0 = B0toC0(B0);               assert(C0<64);
            const unsigned C1 = B0toC1(B0) | B1toC1(B1);  assert(C1<64);
            const unsigned C2 = B1toC2(B1) | B2toC2(B2);  assert(C2<64);
            const unsigned C3 = B2toC3(B2);               assert(C3<64);
            c[0] = table[C0];
            c[1] = table[C1];
            c[2] = table[C2];
            c[3] = table[C3];
        }

        void Base64:: Encode2to3(void *target, const void *source, const char *table) throw()
        {
            assert(target);
            assert(source);
            assert(table);
            const uint8_t *b = static_cast<const uint8_t *>(source);
            char          *c = static_cast<char          *>(target);

            const uint8_t  B0 = b[0];
            const uint8_t  B1 = b[1];
            //const uint8_t  B2 = b[2];
            const unsigned C0 = B0toC0(B0);               assert(C0<64);
            const unsigned C1 = B0toC1(B0) | B1toC1(B1);  assert(C1<64);
            const unsigned C2 = B1toC2(B1);               assert(C2<64);
            c[0] = table[C0];
            c[1] = table[C1];
            c[2] = table[C2];
        }


        void Base64:: Encode1to2(void *target, const void *source, const char *table) throw()
        {
            assert(target);
            assert(source);
            assert(table);
            const uint8_t *b = static_cast<const uint8_t *>(source);
            char          *c = static_cast<char          *>(target);

            const uint8_t  B0 = b[0];
            const unsigned C0 = B0toC0(B0);  assert(C0<64);
            const unsigned C1 = B0toC1(B0);  assert(C1<64);
            c[0] = table[C0];
            c[1] = table[C1];
        }


        namespace {

            //! [00|6bits] -> [6bits|00]
            static inline uint8_t C0toB0(const uint8_t C0) throw()
            {
                return C0 << 2;
            }

            //! [00|2+4bits] -> [000000|2bits]
            static inline uint8_t C1toB0(const uint8_t C1) throw()
            {
                return C1 >> 4;
            }

            //! [00|2+4bits] -> [4bits|0000]
            static inline uint8_t C1toB1(const uint8_t C1) throw()
            {
                return C1<<4;
            }

            //! [00|4+2bits] -> [0000|4bits]
            static inline uint8_t C2toB1(const uint8_t C2) throw()
            {
                return C2 >> 2;
            }

            //! [00|4+2bits] -> [2bits|000000]
            static inline uint8_t C2toB2(const uint8_t C2) throw()
            {
                return C2 << 6;
            }



        }

        void Base64:: Decode4to3(void *target, const void *source) throw()
        {
            assert(target);
            assert(source);
            const uint8_t *c = static_cast<const uint8_t *>(source);
            uint8_t       *b = static_cast<uint8_t       *>(target);

            const uint8_t C0 =  inv_table[ c[0] ];
            const uint8_t C1 =  inv_table[ c[1] ];
            const uint8_t C2 =  inv_table[ c[2] ];
            const uint8_t C3 =  inv_table[ c[3] ];

            b[0] = C0toB0(C0) | C1toB0(C1);
            b[1] = C1toB1(C1) | C2toB1(C2);
            b[2] = C2toB2(C2) | C3;
        }

        void Base64:: Decode3to2(void *target, const void *source) throw()
        {
            assert(target);
            assert(source);
            const uint8_t *c = static_cast<const uint8_t *>(source);
            uint8_t       *b = static_cast<uint8_t       *>(target);

            const uint8_t C0 =  inv_table[ c[0] ];
            const uint8_t C1 =  inv_table[ c[1] ];
            const uint8_t C2 =  inv_table[ c[2] ];

            b[0] = C0toB0(C0) | C1toB0(C1);
            b[1] = C1toB1(C1) | C2toB1(C2);
            b[2] = C2toB2(C2);
        }

        void Base64:: Decode2to1(void *target, const void *source) throw()
        {
            assert(target);
            assert(source);
            const uint8_t *c = static_cast<const uint8_t *>(source);
            uint8_t       *b = static_cast<uint8_t       *>(target);

            const uint8_t C0 =  inv_table[ c[0] ];
            const uint8_t C1 =  inv_table[ c[1] ];
            b[0] = C0toB0(C0) | C1toB0(C1);
            b[1] = C1toB1(C1);
        }

        void Base64:: Encode(void        *target,
                             const void  *source,
                             const size_t length,
                             const char  *table,
                             const Mode   mode) throw()
        {
            size_t      num3 = length/3;
            char       *tgt  = static_cast<char       *>(target);
            const char *src  = static_cast<const char *>(source);
            while(num3-- > 0)
            {
                Encode3to4(tgt, src, table);
                tgt+=4;
                src+=3;
            }
            const size_t rem3 = length%3;
            switch (mode) {
                case Pad:
                    switch(rem3)
                    {
                        case 1: Encode1to2(tgt,src,table); tgt+=2; tgt[0] = tgt[1] = padding; break;
                        case 2: Encode2to3(tgt,src,table); tgt+=3; tgt[0] = padding;          break;
                        default:
                            assert(0==rem3);
                            break;
                    }
                    break;
                    break;
                    
                case Raw:
                    switch(rem3)
                    {
                        case 1: Encode1to2(tgt,src,table); break;
                        case 2: Encode2to3(tgt,src,table); break;
                        default:
                            assert(0==rem3);
                            break;
                    }
                    break;
            }
        }
        
    }

}
