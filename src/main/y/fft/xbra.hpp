//! \file
#ifndef Y_MK_XBRA_INCLUDED
#define Y_MK_XBRA_INCLUDED 1
#include "y/os/platform.hpp"
namespace upsylon {
	//! eXtended Bit Reversal Algorithm
	struct xbra {

		//! cswap two words
		template <typename T> static inline void cswap2(T *a, T *b) throw() {
			const T tmp0 = a[0];
			const T tmp1 = a[1];
			a[0] = b[0];
			a[1] = b[1];
			b[0] = tmp0;
			b[1] = tmp1;
		};

//! implementing generic loop
#define Y_XBRA_LOOP(CODE)   \
const size_t n = size << 1; \
size_t       j = 1;         \
for(size_t i=1; i<n; i+=2)  \
{                           \
  if(j>i)                   \
  {                         \
    CODE(i,j);              \
  }                         \
  size_t m = size;          \
  while( (m>=2) && (j>m) )  \
  {                         \
    j -=  m;                \
    m >>= 1;                \
  }                         \
  j += m;                   \
}

//! code to run one array
#define Y_XBRA_SWAP(I,J) xbra::cswap2( &arr[I], &arr[J] )

		//! apply to arr[1..2*size]
		template <typename T> static inline void run_(T arr[], const size_t size) throw() {
			Y_XBRA_LOOP(Y_XBRA_SWAP)
		}

//! code to run two arrays
#define Y_XBRA_SWAP2(I,J) const size_t _I = (I); const size_t _J =(J); xbra::cswap2( &arr[_I], &arr[_J] ); xbra::cswap2( &brr[_I], &brr[_J] )

		//! apply to arr[1..2*size] and brr[1..2*size] simultaneously
		template <typename T> static inline void run_(T arr[], T brr[], const size_t size) throw() {
			Y_XBRA_LOOP(Y_XBRA_SWAP2)
		}

		static const size_t         size4=1;      //!< size
		static const unsigned short indx4[size4]; //!< indx
		static const unsigned short jndx4[size4]; //!< jndx

		static const size_t         size8=2;      //!< size
		static const unsigned short indx8[size8]; //!< indx
		static const unsigned short jndx8[size8]; //!< jndx

		static const size_t         size16=6;      //!< size
		static const unsigned short indx16[size16]; //!< indx
		static const unsigned short jndx16[size16]; //!< jndx

		static const size_t         size32=12;      //!< size
		static const unsigned short indx32[size32]; //!< indx
		static const unsigned short jndx32[size32]; //!< jndx

		static const size_t         size64=28;      //!< size
		static const unsigned short indx64[size64]; //!< indx
		static const unsigned short jndx64[size64]; //!< jndx

		static const size_t         size128=56;      //!< size
		static const unsigned short indx128[size128]; //!< indx
		static const unsigned short jndx128[size128]; //!< jndx

		static const size_t         size256=120;      //!< size
		static const unsigned short indx256[size256]; //!< indx
		static const unsigned short jndx256[size256]; //!< jndx

		static const size_t         size512=240;      //!< size
		static const unsigned short indx512[size512]; //!< indx
		static const unsigned short jndx512[size512]; //!< jndx

		static const size_t         size1024=496;      //!< size
		static const unsigned short indx1024[size1024]; //!< indx
		static const unsigned short jndx1024[size1024]; //!< jndx

		static const size_t         size2048=992;      //!< size
		static const unsigned short indx2048[size2048]; //!< indx
		static const unsigned short jndx2048[size2048]; //!< jndx

		static const size_t         size4096=2016;      //!< size
		static const unsigned short indx4096[size4096]; //!< indx
		static const unsigned short jndx4096[size4096]; //!< jndx

		static const size_t         size8192=4032;      //!< size
		static const unsigned short indx8192[size8192]; //!< indx
		static const unsigned short jndx8192[size8192]; //!< jndx

		//! dispatching calls for one array
		template <typename T> static inline void run(T arr[], const size_t size) throw() {
			switch(size) {
				case 0: break;
				case 1: break;
				case 2: break;
				case 4: for(size_t i=0;i<xbra::size4;++i) Y_XBRA_SWAP(indx4[i],jndx4[i]); break;
				case 8: for(size_t i=0;i<xbra::size8;++i) Y_XBRA_SWAP(indx8[i],jndx8[i]); break;
				case 16: for(size_t i=0;i<xbra::size16;++i) Y_XBRA_SWAP(indx16[i],jndx16[i]); break;
				case 32: for(size_t i=0;i<xbra::size32;++i) Y_XBRA_SWAP(indx32[i],jndx32[i]); break;
				case 64: for(size_t i=0;i<xbra::size64;++i) Y_XBRA_SWAP(indx64[i],jndx64[i]); break;
				case 128: for(size_t i=0;i<xbra::size128;++i) Y_XBRA_SWAP(indx128[i],jndx128[i]); break;
				case 256: for(size_t i=0;i<xbra::size256;++i) Y_XBRA_SWAP(indx256[i],jndx256[i]); break;
				case 512: for(size_t i=0;i<xbra::size512;++i) Y_XBRA_SWAP(indx512[i],jndx512[i]); break;
				case 1024: for(size_t i=0;i<xbra::size1024;++i) Y_XBRA_SWAP(indx1024[i],jndx1024[i]); break;
				case 2048: for(size_t i=0;i<xbra::size2048;++i) Y_XBRA_SWAP(indx2048[i],jndx2048[i]); break;
				case 4096: for(size_t i=0;i<xbra::size4096;++i) Y_XBRA_SWAP(indx4096[i],jndx4096[i]); break;
				case 8192: for(size_t i=0;i<xbra::size8192;++i) Y_XBRA_SWAP(indx8192[i],jndx8192[i]); break;
				default: run_(arr,size);
			}
		}

		//! dispatching calls for two arrays
		template <typename T> static inline void run(T arr[], T brr[], const size_t size) throw() {
			switch(size) {
				case 0: break;
				case 1: break;
				case 2: break;
				case 4: for(size_t i=0;i<xbra::size4;++i) { Y_XBRA_SWAP2(indx4[i],jndx4[i]); }break;
				case 8: for(size_t i=0;i<xbra::size8;++i) { Y_XBRA_SWAP2(indx8[i],jndx8[i]); }break;
				case 16: for(size_t i=0;i<xbra::size16;++i) { Y_XBRA_SWAP2(indx16[i],jndx16[i]); }break;
				case 32: for(size_t i=0;i<xbra::size32;++i) { Y_XBRA_SWAP2(indx32[i],jndx32[i]); }break;
				case 64: for(size_t i=0;i<xbra::size64;++i) { Y_XBRA_SWAP2(indx64[i],jndx64[i]); }break;
				case 128: for(size_t i=0;i<xbra::size128;++i) { Y_XBRA_SWAP2(indx128[i],jndx128[i]); }break;
				case 256: for(size_t i=0;i<xbra::size256;++i) { Y_XBRA_SWAP2(indx256[i],jndx256[i]); }break;
				case 512: for(size_t i=0;i<xbra::size512;++i) { Y_XBRA_SWAP2(indx512[i],jndx512[i]); }break;
				case 1024: for(size_t i=0;i<xbra::size1024;++i) { Y_XBRA_SWAP2(indx1024[i],jndx1024[i]); }break;
				case 2048: for(size_t i=0;i<xbra::size2048;++i) { Y_XBRA_SWAP2(indx2048[i],jndx2048[i]); }break;
				case 4096: for(size_t i=0;i<xbra::size4096;++i) { Y_XBRA_SWAP2(indx4096[i],jndx4096[i]); }break;
				case 8192: for(size_t i=0;i<xbra::size8192;++i) { Y_XBRA_SWAP2(indx8192[i],jndx8192[i]); }break;
				default: run_(arr,brr,size);
			}
		}
	};

	//! generic call
	template <typename T, const size_t SIZE> struct xbra_for {
		static inline void run(T arr[])          throw() { xbra::run_(arr,SIZE);     } //!< one array
		static inline void run(T arr[], T brr[]) throw() { xbra::run_(arr,brr,SIZE); } //!< two arrays
	};

	//! no items, safe-guard
	template <typename T> struct xbra_for<T,0> {
		static inline void run(T [])       throw() { } //!< one array
		static inline void run(T [], T []) throw() { } //!< two arrays
	};

	//! for SIZE=1
	template <typename T> struct xbra_for<T,1> {
		static inline void run(T [])       throw() { } //!< one array
		static inline void run(T [], T []) throw() { } //!< two arrays
	};

	//! for SIZE=2
	template <typename T> struct xbra_for<T,2> {
		static inline void run(T [])       throw() { } //!< one array
		static inline void run(T [], T []) throw() { } //!< two arrays
	};

	//! for SIZE=4
	template <typename T> struct xbra_for<T,4> {
		//! one array
		static inline void run(T arr[])          throw() {
			for(size_t i=0;i<xbra::size4;++i) Y_XBRA_SWAP(xbra::indx4[i],xbra::jndx4[i]);
		}
		//! two arrays
		static inline void run(T arr[], T brr[]) throw() {
			for(size_t i=0;i<xbra::size4;++i) { Y_XBRA_SWAP2(xbra::indx4[i],xbra::jndx4[i]); }
		}
	};

	//! for SIZE=8
	template <typename T> struct xbra_for<T,8> {
		//! one array
		static inline void run(T arr[])          throw() {
			for(size_t i=0;i<xbra::size8;++i) Y_XBRA_SWAP(xbra::indx8[i],xbra::jndx8[i]);
		}
		//! two arrays
		static inline void run(T arr[], T brr[]) throw() {
			for(size_t i=0;i<xbra::size8;++i) { Y_XBRA_SWAP2(xbra::indx8[i],xbra::jndx8[i]); }
		}
	};

	//! for SIZE=16
	template <typename T> struct xbra_for<T,16> {
		//! one array
		static inline void run(T arr[])          throw() {
			for(size_t i=0;i<xbra::size16;++i) Y_XBRA_SWAP(xbra::indx16[i],xbra::jndx16[i]);
		}
		//! two arrays
		static inline void run(T arr[], T brr[]) throw() {
			for(size_t i=0;i<xbra::size16;++i) { Y_XBRA_SWAP2(xbra::indx16[i],xbra::jndx16[i]); }
		}
	};

	//! for SIZE=32
	template <typename T> struct xbra_for<T,32> {
		//! one array
		static inline void run(T arr[])          throw() {
			for(size_t i=0;i<xbra::size32;++i) Y_XBRA_SWAP(xbra::indx32[i],xbra::jndx32[i]);
		}
		//! two arrays
		static inline void run(T arr[], T brr[]) throw() {
			for(size_t i=0;i<xbra::size32;++i) { Y_XBRA_SWAP2(xbra::indx32[i],xbra::jndx32[i]); }
		}
	};

	//! for SIZE=64
	template <typename T> struct xbra_for<T,64> {
		//! one array
		static inline void run(T arr[])          throw() {
			for(size_t i=0;i<xbra::size64;++i) Y_XBRA_SWAP(xbra::indx64[i],xbra::jndx64[i]);
		}
		//! two arrays
		static inline void run(T arr[], T brr[]) throw() {
			for(size_t i=0;i<xbra::size64;++i) { Y_XBRA_SWAP2(xbra::indx64[i],xbra::jndx64[i]); }
		}
	};

	//! for SIZE=128
	template <typename T> struct xbra_for<T,128> {
		//! one array
		static inline void run(T arr[])          throw() {
			for(size_t i=0;i<xbra::size128;++i) Y_XBRA_SWAP(xbra::indx128[i],xbra::jndx128[i]);
		}
		//! two arrays
		static inline void run(T arr[], T brr[]) throw() {
			for(size_t i=0;i<xbra::size128;++i) { Y_XBRA_SWAP2(xbra::indx128[i],xbra::jndx128[i]); }
		}
	};

	//! for SIZE=256
	template <typename T> struct xbra_for<T,256> {
		//! one array
		static inline void run(T arr[])          throw() {
			for(size_t i=0;i<xbra::size256;++i) Y_XBRA_SWAP(xbra::indx256[i],xbra::jndx256[i]);
		}
		//! two arrays
		static inline void run(T arr[], T brr[]) throw() {
			for(size_t i=0;i<xbra::size256;++i) { Y_XBRA_SWAP2(xbra::indx256[i],xbra::jndx256[i]); }
		}
	};

	//! for SIZE=512
	template <typename T> struct xbra_for<T,512> {
		//! one array
		static inline void run(T arr[])          throw() {
			for(size_t i=0;i<xbra::size512;++i) Y_XBRA_SWAP(xbra::indx512[i],xbra::jndx512[i]);
		}
		//! two arrays
		static inline void run(T arr[], T brr[]) throw() {
			for(size_t i=0;i<xbra::size512;++i) { Y_XBRA_SWAP2(xbra::indx512[i],xbra::jndx512[i]); }
		}
	};

	//! for SIZE=1024
	template <typename T> struct xbra_for<T,1024> {
		//! one array
		static inline void run(T arr[])          throw() {
			for(size_t i=0;i<xbra::size1024;++i) Y_XBRA_SWAP(xbra::indx1024[i],xbra::jndx1024[i]);
		}
		//! two arrays
		static inline void run(T arr[], T brr[]) throw() {
			for(size_t i=0;i<xbra::size1024;++i) { Y_XBRA_SWAP2(xbra::indx1024[i],xbra::jndx1024[i]); }
		}
	};

	//! for SIZE=2048
	template <typename T> struct xbra_for<T,2048> {
		//! one array
		static inline void run(T arr[])          throw() {
			for(size_t i=0;i<xbra::size2048;++i) Y_XBRA_SWAP(xbra::indx2048[i],xbra::jndx2048[i]);
		}
		//! two arrays
		static inline void run(T arr[], T brr[]) throw() {
			for(size_t i=0;i<xbra::size2048;++i) { Y_XBRA_SWAP2(xbra::indx2048[i],xbra::jndx2048[i]); }
		}
	};

	//! for SIZE=4096
	template <typename T> struct xbra_for<T,4096> {
		//! one array
		static inline void run(T arr[])          throw() {
			for(size_t i=0;i<xbra::size4096;++i) Y_XBRA_SWAP(xbra::indx4096[i],xbra::jndx4096[i]);
		}
		//! two arrays
		static inline void run(T arr[], T brr[]) throw() {
			for(size_t i=0;i<xbra::size4096;++i) { Y_XBRA_SWAP2(xbra::indx4096[i],xbra::jndx4096[i]); }
		}
	};

	//! for SIZE=8192
	template <typename T> struct xbra_for<T,8192> {
		//! one array
		static inline void run(T arr[])          throw() {
			for(size_t i=0;i<xbra::size8192;++i) Y_XBRA_SWAP(xbra::indx8192[i],xbra::jndx8192[i]);
		}
		//! two arrays
		static inline void run(T arr[], T brr[]) throw() {
			for(size_t i=0;i<xbra::size8192;++i) { Y_XBRA_SWAP2(xbra::indx8192[i],xbra::jndx8192[i]); }
		}
	};
}
#endif
