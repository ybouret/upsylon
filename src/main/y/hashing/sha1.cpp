#include "y/hashing/sha1.hpp"
#include <cstring>

namespace upsylon
{
	
	namespace hashing
	{
		
		namespace RFC3174
		{
			
#define GET_ULONG_BE(n,b,i)                             \
{                                                       \
(n) = ( (unsigned long) (b)[(i)    ] << 24 )        \
| ( (unsigned long) (b)[(i) + 1] << 16 )        \
| ( (unsigned long) (b)[(i) + 2] <<  8 )        \
| ( (unsigned long) (b)[(i) + 3]       );       \
}
			
#define PUT_ULONG_BE(n,b,i)                             \
{                                                       \
(b)[(i)    ] = (unsigned char) ( (n) >> 24 );       \
(b)[(i) + 1] = (unsigned char) ( (n) >> 16 );       \
(b)[(i) + 2] = (unsigned char) ( (n) >>  8 );       \
(b)[(i) + 3] = (unsigned char) ( (n)       );       \
}
			
			
			static void  sha1_set( SHA1_CTX *ctx )
			{
				ctx->total[0] = 0;
				ctx->total[1] = 0;
				
				ctx->state[0] = 0x67452301;
				ctx->state[1] = 0xEFCDAB89;
				ctx->state[2] = 0x98BADCFE;
				ctx->state[3] = 0x10325476;
				ctx->state[4] = 0xC3D2E1F0;
			}
			
			static void sha1_process( SHA1_CTX *ctx, const uint8_t data[64] ) 
			{
				unsigned long temp, W[16], A, B, C, D, E;
				
				GET_ULONG_BE( W[ 0], data,  0 );
				GET_ULONG_BE( W[ 1], data,  4 );
				GET_ULONG_BE( W[ 2], data,  8 );
				GET_ULONG_BE( W[ 3], data, 12 );
				GET_ULONG_BE( W[ 4], data, 16 );
				GET_ULONG_BE( W[ 5], data, 20 );
				GET_ULONG_BE( W[ 6], data, 24 );
				GET_ULONG_BE( W[ 7], data, 28 );
				GET_ULONG_BE( W[ 8], data, 32 );
				GET_ULONG_BE( W[ 9], data, 36 );
				GET_ULONG_BE( W[10], data, 40 );
				GET_ULONG_BE( W[11], data, 44 );
				GET_ULONG_BE( W[12], data, 48 );
				GET_ULONG_BE( W[13], data, 52 );
				GET_ULONG_BE( W[14], data, 56 );
				GET_ULONG_BE( W[15], data, 60 );
				
#define S(x,n) ((x << n) | ((x & 0xFFFFFFFF) >> (32 - n)))
				
#define R(t)                                            \
(                                                       \
temp = W[(t -  3) & 0x0F] ^ W[(t - 8) & 0x0F] ^     \
W[(t - 14) & 0x0F] ^ W[ t      & 0x0F],      \
( W[t & 0x0F] = S(temp,1) )                         \
)
				
#define P(a,b,c,d,e,x)                                  \
{                                                       \
e += S(a,5) + F(b,c,d) + K + x; b = S(b,30);        \
}
				
				A = ctx->state[0];
				B = ctx->state[1];
				C = ctx->state[2];
				D = ctx->state[3];
				E = ctx->state[4];
				
#define F(x,y,z) (z ^ (x & (y ^ z)))
#define K 0x5A827999
				
				P( A, B, C, D, E, W[0]  );
				P( E, A, B, C, D, W[1]  );
				P( D, E, A, B, C, W[2]  );
				P( C, D, E, A, B, W[3]  );
				P( B, C, D, E, A, W[4]  );
				P( A, B, C, D, E, W[5]  );
				P( E, A, B, C, D, W[6]  );
				P( D, E, A, B, C, W[7]  );
				P( C, D, E, A, B, W[8]  );
				P( B, C, D, E, A, W[9]  );
				P( A, B, C, D, E, W[10] );
				P( E, A, B, C, D, W[11] );
				P( D, E, A, B, C, W[12] );
				P( C, D, E, A, B, W[13] );
				P( B, C, D, E, A, W[14] );
				P( A, B, C, D, E, W[15] );
				P( E, A, B, C, D, R(16) );
				P( D, E, A, B, C, R(17) );
				P( C, D, E, A, B, R(18) );
				P( B, C, D, E, A, R(19) );
				
#undef K
#undef F
				
#define F(x,y,z) (x ^ y ^ z)
#define K 0x6ED9EBA1
				
				P( A, B, C, D, E, R(20) );
				P( E, A, B, C, D, R(21) );
				P( D, E, A, B, C, R(22) );
				P( C, D, E, A, B, R(23) );
				P( B, C, D, E, A, R(24) );
				P( A, B, C, D, E, R(25) );
				P( E, A, B, C, D, R(26) );
				P( D, E, A, B, C, R(27) );
				P( C, D, E, A, B, R(28) );
				P( B, C, D, E, A, R(29) );
				P( A, B, C, D, E, R(30) );
				P( E, A, B, C, D, R(31) );
				P( D, E, A, B, C, R(32) );
				P( C, D, E, A, B, R(33) );
				P( B, C, D, E, A, R(34) );
				P( A, B, C, D, E, R(35) );
				P( E, A, B, C, D, R(36) );
				P( D, E, A, B, C, R(37) );
				P( C, D, E, A, B, R(38) );
				P( B, C, D, E, A, R(39) );
				
#undef K
#undef F
				
#define F(x,y,z) ((x & y) | (z & (x | y)))
#define K 0x8F1BBCDC
				
				P( A, B, C, D, E, R(40) );
				P( E, A, B, C, D, R(41) );
				P( D, E, A, B, C, R(42) );
				P( C, D, E, A, B, R(43) );
				P( B, C, D, E, A, R(44) );
				P( A, B, C, D, E, R(45) );
				P( E, A, B, C, D, R(46) );
				P( D, E, A, B, C, R(47) );
				P( C, D, E, A, B, R(48) );
				P( B, C, D, E, A, R(49) );
				P( A, B, C, D, E, R(50) );
				P( E, A, B, C, D, R(51) );
				P( D, E, A, B, C, R(52) );
				P( C, D, E, A, B, R(53) );
				P( B, C, D, E, A, R(54) );
				P( A, B, C, D, E, R(55) );
				P( E, A, B, C, D, R(56) );
				P( D, E, A, B, C, R(57) );
				P( C, D, E, A, B, R(58) );
				P( B, C, D, E, A, R(59) );
				
#undef K
#undef F
				
#define F(x,y,z) (x ^ y ^ z)
#define K 0xCA62C1D6
				
				P( A, B, C, D, E, R(60) );
				P( E, A, B, C, D, R(61) );
				P( D, E, A, B, C, R(62) );
				P( C, D, E, A, B, R(63) );
				P( B, C, D, E, A, R(64) );
				P( A, B, C, D, E, R(65) );
				P( E, A, B, C, D, R(66) );
				P( D, E, A, B, C, R(67) );
				P( C, D, E, A, B, R(68) );
				P( B, C, D, E, A, R(69) );
				P( A, B, C, D, E, R(70) );
				P( E, A, B, C, D, R(71) );
				P( D, E, A, B, C, R(72) );
				P( C, D, E, A, B, R(73) );
				P( B, C, D, E, A, R(74) );
				P( A, B, C, D, E, R(75) );
				P( E, A, B, C, D, R(76) );
				P( D, E, A, B, C, R(77) );
				P( C, D, E, A, B, R(78) );
				P( B, C, D, E, A, R(79) );
				
#undef K
#undef F
				
				ctx->state[0] += A;
				ctx->state[1] += B;
				ctx->state[2] += C;
				ctx->state[3] += D;
				ctx->state[4] += E;
			}
			
			
			static void  sha1_run( SHA1_CTX *ctx, const void *buf, const size_t buflen )
			{
				const uint8_t *input = (const uint8_t *)buf;
				uint32_t ilen = (uint32_t)buflen;
				uint32_t fill;
				uint32_t left;
				
				if( ilen <= 0 )
					return;
				
				left = ctx->total[0] & 0x3F;
				fill = 64 - left;
				
				ctx->total[0] += ilen;
				ctx->total[0] &= 0xFFFFFFFF;
				
				if( ctx->total[0] < (unsigned long) ilen )
					ctx->total[1]++;
				
				if( left && ilen >= fill )
				{
					memcpy( (void *) (ctx->buffer + left),
						   (void *) input, fill );
					sha1_process( ctx, ctx->buffer );
					input += fill;
					ilen  -= fill;
					left = 0;
				}
				
				while( ilen >= 64 )
				{
					sha1_process( ctx, input );
					input += 64;
					ilen  -= 64;
				}
				
				if( ilen > 0 )
				{
					memcpy( (void *) (ctx->buffer + left),
						   (void *) input, ilen );
				}
			}
			
			static const uint8_t sha1_padding[64] =
			{
				0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
			};
			
			
			static void sha1_compute( SHA1_CTX *ctx, uint8_t output[20] )
			{
				uint32_t last, padn;
				uint32_t high, low;
				uint8_t  msglen[8];
				
				high = ( ctx->total[0] >> 29 )
				| ( ctx->total[1] <<  3 );
				low  = ( ctx->total[0] <<  3 );
				
				PUT_ULONG_BE( high, msglen, 0 );
				PUT_ULONG_BE( low,  msglen, 4 );
				
				last = ctx->total[0] & 0x3F;
				padn = ( last < 56 ) ? ( 56 - last ) : ( 120 - last );
				
				sha1_run( ctx, sha1_padding, padn );
				sha1_run( ctx, msglen, 8 );
				
				PUT_ULONG_BE( ctx->state[0], output,  0 );
				PUT_ULONG_BE( ctx->state[1], output,  4 );
				PUT_ULONG_BE( ctx->state[2], output,  8 );
				PUT_ULONG_BE( ctx->state[3], output, 12 );
				PUT_ULONG_BE( ctx->state[4], output, 16 );
			}
		} // RFC1321
		
		
		sha1::sha1() throw() : function(__length, __window), ctx()
		{
		}
		
        const char sha1:: CLID[] = "sha1";
		
		
		void sha1:: set() throw()
		{
			RFC3174::sha1_set( &ctx );
		}
		
		void sha1:: run( const void *buffer, size_t buflen ) throw()
		{
			assert( !(buflen>0&&NULL==buffer) );
			RFC3174::sha1_run( &ctx, (const uint8_t *)buffer, buflen );
		}
		
		void sha1::get(void *output, size_t outlen ) throw()
		{
			assert( !(output==NULL&&outlen>0) );
			uint8_t  digest[20];
			RFC3174::sha1_compute( &ctx, digest );
			fill( output, outlen, digest, sizeof(digest) );
		}
		
		sha1:: ~sha1() throw()
		{
			memset( &ctx, 0 , sizeof(ctx) );
		}
		
	}
}

