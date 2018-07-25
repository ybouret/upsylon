//! \file
#ifndef Y_NWSRT_INCLUDED
#define Y_NWSRT_INCLUDED 1
#include "y/type/bswap.hpp"
namespace upsylon {
	//! network sorting
	struct nwsrt {
		static const size_t max_size=32; //!< max handled case
		//!built-in version on 2 items
		template <typename T> static inline void on2(T *a) {
			assert(a);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 3 items
		template <typename T> static inline void on3(T *a) {
			assert(a);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 4 items
		template <typename T> static inline void on4(T *a) {
			assert(a);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 5 items
		template <typename T> static inline void on5(T *a) {
			assert(a);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 6 items
		template <typename T> static inline void on6(T *a) {
			assert(a);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 7 items
		template <typename T> static inline void on7(T *a) {
			assert(a);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 8 items
		template <typename T> static inline void on8(T *a) {
			assert(a);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 9 items
		template <typename T> static inline void on9(T *a) {
			assert(a);
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 10 items
		template <typename T> static inline void on10(T *a) {
			assert(a);
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 11 items
		template <typename T> static inline void on11(T *a) {
			assert(a);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 12 items
		template <typename T> static inline void on12(T *a) {
			assert(a);
			{ T &aI = a[ 0]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 13 items
		template <typename T> static inline void on13(T *a) {
			assert(a);
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 14 items
		template <typename T> static inline void on14(T *a) {
			assert(a);
			{ T &aI = a[ 0]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 15 items
		template <typename T> static inline void on15(T *a) {
			assert(a);
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 16 items
		template <typename T> static inline void on16(T *a) {
			assert(a);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 17 items
		template <typename T> static inline void on17(T *a) {
			assert(a);
			{ T &aI = a[ 5]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 18 items
		template <typename T> static inline void on18(T *a) {
			assert(a);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 19 items
		template <typename T> static inline void on19(T *a) {
			assert(a);
			{ T &aI = a[ 4]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 20 items
		template <typename T> static inline void on20(T *a) {
			assert(a);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 21 items
		template <typename T> static inline void on21(T *a) {
			assert(a);
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 22 items
		template <typename T> static inline void on22(T *a) {
			assert(a);
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 23 items
		template <typename T> static inline void on23(T *a) {
			assert(a);
			{ T &aI = a[ 1]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 24 items
		template <typename T> static inline void on24(T *a) {
			assert(a);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 25 items
		template <typename T> static inline void on25(T *a) {
			assert(a);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 26 items
		template <typename T> static inline void on26(T *a) {
			assert(a);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[24]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 27 items
		template <typename T> static inline void on27(T *a) {
			assert(a);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[25]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[24]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[24]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 28 items
		template <typename T> static inline void on28(T *a) {
			assert(a);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[24]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[26]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[24]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[25]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[25]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 29 items
		template <typename T> static inline void on29(T *a) {
			assert(a);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[25]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[27]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[25]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[26]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[26]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[24]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[24]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[24]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[24]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 30 items
		template <typename T> static inline void on30(T *a) {
			assert(a);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[24]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[26]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[28]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[26]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[27]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[27]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[25]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[24]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[25]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[24]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[25]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[25]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 31 items
		template <typename T> static inline void on31(T *a) {
			assert(a);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[25]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[27]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[29]; T &aJ = a[30]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[24]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[27]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[28]; T &aJ = a[30]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[24]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[28]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[26]; T &aJ = a[30]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[24]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[25]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[30]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[24]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[26]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[30]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[25]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[26]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[26]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 32 items
		template <typename T> static inline void on32(T *a) {
			assert(a);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[24]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[26]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[28]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[30]; T &aJ = a[31]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[24]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[25]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[28]; T &aJ = a[30]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[29]; T &aJ = a[31]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[25]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[29]; T &aJ = a[30]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[24]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[27]; T &aJ = a[31]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[15]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[25]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[26]; T &aJ = a[30]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[16]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[31]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[25]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[27]; T &aJ = a[30]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 0]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[31]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[26]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[27]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[30]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[27]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[17]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[30]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[18]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[30]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 1]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[30]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 2]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[29]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[12]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[19]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[20]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[21]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[22]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[23]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 3]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[28]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 4]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[27]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 5]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[26]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 6]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[25]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 7]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[24]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 8]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[23]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[ 9]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[22]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[10]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[21]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[11]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[20]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[13]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[18]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) bswap(aI,aJ); }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) bswap(aI,aJ); }
		}
		//!built-in version on 0-32 items
		template <typename T> static inline void on(T *a, const size_t n) {
			switch(n) {
				case  2: on2(a);  break;
				case  3: on3(a);  break;
				case  4: on4(a);  break;
				case  5: on5(a);  break;
				case  6: on6(a);  break;
				case  7: on7(a);  break;
				case  8: on8(a);  break;
				case  9: on9(a);  break;
				case 10: on10(a); break;
				case 11: on11(a); break;
				case 12: on12(a); break;
				case 13: on13(a); break;
				case 14: on14(a); break;
				case 15: on15(a); break;
				case 16: on16(a); break;
				case 17: on17(a); break;
				case 18: on18(a); break;
				case 19: on19(a); break;
				case 20: on20(a); break;
				case 21: on21(a); break;
				case 22: on22(a); break;
				case 23: on23(a); break;
				case 24: on24(a); break;
				case 25: on25(a); break;
				case 26: on26(a); break;
				case 27: on27(a); break;
				case 28: on28(a); break;
				case 29: on29(a); break;
				case 30: on30(a); break;
				case 31: on31(a); break;
				case 32: on32(a); break;
				default: break;
			}
		}
		//!built-in version on 2 items
		template <typename T,typename U> static inline void on2(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
		}
		//!built-in version on 3 items
		template <typename T,typename U> static inline void on3(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
		}
		//!built-in version on 4 items
		template <typename T,typename U> static inline void on4(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
		}
		//!built-in version on 5 items
		template <typename T,typename U> static inline void on5(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 3]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
		}
		//!built-in version on 6 items
		template <typename T,typename U> static inline void on6(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 5]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 3]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
		}
		//!built-in version on 7 items
		template <typename T,typename U> static inline void on7(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 4]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 5]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 3]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 5]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
		}
		//!built-in version on 8 items
		template <typename T,typename U> static inline void on8(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 4]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 7]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 5]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 6]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
		}
		//!built-in version on 9 items
		template <typename T,typename U> static inline void on9(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 5]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 7]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 8]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 7]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 7]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
		}
		//!built-in version on 10 items
		template <typename T,typename U> static inline void on10(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 9]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 5]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 7]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 6]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 9]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 9]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 7]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 8]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 8]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 5]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
		}
		//!built-in version on 11 items
		template <typename T,typename U> static inline void on11(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[10]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 4]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 7]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[10]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 8]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 9]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 4]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 8]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[10]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[10]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
		}
		//!built-in version on 12 items
		template <typename T,typename U> static inline void on12(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 0]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 5]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 7]); } }
			{ T &aI = a[ 4]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[10]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 6]); } }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[11]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 9]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 8]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 3]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[10]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 8]); } }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[11]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 8]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 7]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 5]); } }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[10]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 9]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[10]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
		}
		//!built-in version on 13 items
		template <typename T,typename U> static inline void on13(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 8]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 9]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 7]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[11]); } }
			{ T &aI = a[ 1]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[12]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 6]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 8]); } }
			{ T &aI = a[ 7]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[12]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[12]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 6]); } }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 8]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 9]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 7]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 8]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 9]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[10]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
		}
		//!built-in version on 14 items
		template <typename T,typename U> static inline void on14(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 0]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 6]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[12]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 1]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[10]); } }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[13]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 6]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 7]); } }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[13]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 8]); } }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[12]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 9]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[13]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 7]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 8]); } }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[11]); } }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[10]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 8]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 9]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[12]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 9]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[10]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 9]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
		}
		//!built-in version on 15 items
		template <typename T,typename U> static inline void on15(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[ 3]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[11]); } }
			{ T &aI = a[ 2]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[10]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 9]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[ 1]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[14]); } }
			{ T &aI = a[11]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[14]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[10]); } }
			{ T &aI = a[ 0]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[12]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 5]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 9]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[14]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[13]); } }
			{ T &aI = a[ 3]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[12]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 5]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[11]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[12]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[13]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 7]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[14]); } }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[13]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 5]); } }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[11]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 9]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[12]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 8]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
		}
		//!built-in version on 16 items
		template <typename T,typename U> static inline void on16(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[15]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[10]); } }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[14]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 7]); } }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[15]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[14]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 5]); } }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[13]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 4]); } }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[12]); } }
			{ T &aI = a[ 7]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[15]); } }
			{ T &aI = a[ 6]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[14]); } }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[13]); } }
			{ T &aI = a[ 4]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[12]); } }
			{ T &aI = a[ 3]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[11]); } }
			{ T &aI = a[ 2]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[10]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 9]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 8]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[12]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 8]); } }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[11]); } }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[10]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 9]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 8]); } }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[13]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 9]); } }
			{ T &aI = a[ 5]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[12]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[14]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[12]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[13]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[10]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 8]); } }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[12]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
		}
		//!built-in version on 17 items
		template <typename T,typename U> static inline void on17(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 5]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[11]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 9]); } }
			{ T &aI = a[ 7]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[12]); } }
			{ T &aI = a[ 0]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[14]); } }
			{ T &aI = a[ 2]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[16]); } }
			{ T &aI = a[ 1]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[15]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 8]); } }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[13]); } }
			{ T &aI = a[ 3]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[10]); } }
			{ T &aI = a[ 8]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[13]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 7]); } }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[12]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[16]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 6]); } }
			{ T &aI = a[10]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[15]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[13]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 4]); } }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[16]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 9]); } }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[14]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[15]); } }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[10]); } }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[11]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[10]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[13]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[14]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[12]); } }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[14]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 8]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 5]); } }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[12]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[ 3]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[10]); } }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[15]); } }
			{ T &aI = a[11]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[14]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 8]); } }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[12]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 6]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
		}
		//!built-in version on 18 items
		template <typename T,typename U> static inline void on18(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[10]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[12]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[17]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 4]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 5]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 7]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[12]); } }
			{ T &aI = a[11]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[16]); } }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[15]); } }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[17]); } }
			{ T &aI = a[ 7]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[16]); } }
			{ T &aI = a[ 6]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[17]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[14]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[ 9]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[15]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 1]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[13]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 8]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[14]); } }
			{ T &aI = a[ 5]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[12]); } }
			{ T &aI = a[ 3]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[15]); } }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[13]); } }
			{ T &aI = a[ 4]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[10]); } }
			{ T &aI = a[ 2]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[11]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 8]); } }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[16]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 9]); } }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[13]); } }
			{ T &aI = a[ 5]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[11]); } }
			{ T &aI = a[ 3]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[10]); } }
			{ T &aI = a[ 4]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[15]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 8]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 9]); } }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[11]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[16]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 6]); } }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[13]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 8]); } }
			{ T &aI = a[11]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[14]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[14]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[15]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
		}
		//!built-in version on 19 items
		template <typename T,typename U> static inline void on19(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 4]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[10]); } }
			{ T &aI = a[ 3]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[12]); } }
			{ T &aI = a[ 0]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[16]); } }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[14]); } }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[11]); } }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[13]); } }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[17]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 5]); } }
			{ T &aI = a[ 9]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[18]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 5]); } }
			{ T &aI = a[11]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[16]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[ 6]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[15]); } }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[12]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[17]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 8]); } }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[18]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 5]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[16]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 7]); } }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[18]); } }
			{ T &aI = a[ 4]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[15]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 6]); } }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[16]); } }
			{ T &aI = a[ 2]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[11]); } }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[13]); } }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[10]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[18]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 9]); } }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[15]); } }
			{ T &aI = a[12]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[17]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 3]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 7]); } }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[14]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[14]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[13]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[17]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[10]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[15]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[13]); } }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[12]); } }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[11]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 5]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[11]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 8]); } }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[11]); } }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[14]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[13]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 8]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 9]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
		}
		//!built-in version on 20 items
		template <typename T,typename U> static inline void on20(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[19]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[15]); } }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[19]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[10]); } }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[14]); } }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[18]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 7]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[19]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[18]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 5]); } }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[17]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 4]); } }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[16]); } }
			{ T &aI = a[ 7]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[19]); } }
			{ T &aI = a[ 6]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[18]); } }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[17]); } }
			{ T &aI = a[ 4]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[16]); } }
			{ T &aI = a[ 3]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[15]); } }
			{ T &aI = a[ 2]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[14]); } }
			{ T &aI = a[ 1]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[13]); } }
			{ T &aI = a[ 0]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[12]); } }
			{ T &aI = a[ 2]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[19]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 8]); } }
			{ T &aI = a[11]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[16]); } }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[18]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[ 5]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[15]); } }
			{ T &aI = a[ 9]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[14]); } }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[13]); } }
			{ T &aI = a[ 6]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[12]); } }
			{ T &aI = a[ 0]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[19]); } }
			{ T &aI = a[ 1]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[18]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[15]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[14]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 9]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[12]); } }
			{ T &aI = a[13]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[16]); } }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[18]); } }
			{ T &aI = a[ 0]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[15]); } }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[14]); } }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[11]); } }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[10]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[13]); } }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[17]); } }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[19]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 8]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[12]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[15]); } }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[18]); } }
			{ T &aI = a[ 1]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[17]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[10]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[19]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
		}
		//!built-in version on 21 items
		template <typename T,typename U> static inline void on21(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 9]); } }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[15]); } }
			{ T &aI = a[ 1]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[19]); } }
			{ T &aI = a[ 2]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[14]); } }
			{ T &aI = a[ 6]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[18]); } }
			{ T &aI = a[ 0]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[17]); } }
			{ T &aI = a[ 3]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[20]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 8]); } }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[16]); } }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[13]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 7]); } }
			{ T &aI = a[13]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[19]); } }
			{ T &aI = a[ 2]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[11]); } }
			{ T &aI = a[ 9]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[18]); } }
			{ T &aI = a[ 4]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[12]); } }
			{ T &aI = a[ 8]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[16]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[17]); } }
			{ T &aI = a[ 0]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[10]); } }
			{ T &aI = a[10]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[20]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 6]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[18]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[16]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[19]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[14]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[20]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[11]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[20]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[ 5]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[12]); } }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[13]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 9]); } }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[20]); } }
			{ T &aI = a[ 8]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[15]); } }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[14]); } }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[19]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[10]); } }
			{ T &aI = a[ 3]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[12]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[15]); } }
			{ T &aI = a[11]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[18]); } }
			{ T &aI = a[ 9]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[14]); } }
			{ T &aI = a[ 8]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[16]); } }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[19]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[17]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 7]); } }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[14]); } }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[16]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 8]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 9]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[19]); } }
			{ T &aI = a[11]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[17]); } }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[16]); } }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[12]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 6]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[15]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[18]); } }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[13]); } }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[18]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[15]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[14]); } }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[10]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[13]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[12]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
		}
		//!built-in version on 22 items
		template <typename T,typename U> static inline void on22(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 8]); } }
			{ T &aI = a[13]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[19]); } }
			{ T &aI = a[ 3]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[15]); } }
			{ T &aI = a[ 6]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[18]); } }
			{ T &aI = a[ 1]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[16]); } }
			{ T &aI = a[ 5]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[20]); } }
			{ T &aI = a[ 0]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[17]); } }
			{ T &aI = a[ 4]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[21]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[14]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 4]); } }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[21]); } }
			{ T &aI = a[ 3]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[12]); } }
			{ T &aI = a[ 9]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[18]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[20]); } }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[13]); } }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[14]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[16]); } }
			{ T &aI = a[ 1]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[20]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 3]); } }
			{ T &aI = a[18]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[21]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[15]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[19]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 5]); } }
			{ T &aI = a[16]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[21]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[12]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 7]); } }
			{ T &aI = a[14]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[19]); } }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[13]); } }
			{ T &aI = a[ 8]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[15]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[21]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 9]); } }
			{ T &aI = a[12]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[18]); } }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[11]); } }
			{ T &aI = a[10]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[17]); } }
			{ T &aI = a[ 8]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[13]); } }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[16]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 6]); } }
			{ T &aI = a[15]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[18]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[20]); } }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[12]); } }
			{ T &aI = a[ 4]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[14]); } }
			{ T &aI = a[ 7]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[17]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[20]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[19]); } }
			{ T &aI = a[12]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[17]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 9]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 5]); } }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[20]); } }
			{ T &aI = a[11]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[18]); } }
			{ T &aI = a[ 3]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[10]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 7]); } }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[17]); } }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[12]); } }
			{ T &aI = a[ 8]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[13]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 5]); } }
			{ T &aI = a[16]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[19]); } }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[15]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[14]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[19]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[13]); } }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[11]); } }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[18]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 9]); } }
			{ T &aI = a[12]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[15]); } }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[11]); } }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[13]); } }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[16]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
		}
		//!built-in version on 23 items
		template <typename T,typename U> static inline void on23(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 1]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[20]); } }
			{ T &aI = a[ 2]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[21]); } }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[13]); } }
			{ T &aI = a[ 9]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[17]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 7]); } }
			{ T &aI = a[15]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[22]); } }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[11]); } }
			{ T &aI = a[ 6]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[12]); } }
			{ T &aI = a[10]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[16]); } }
			{ T &aI = a[ 8]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[18]); } }
			{ T &aI = a[14]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[19]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 8]); } }
			{ T &aI = a[ 4]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[14]); } }
			{ T &aI = a[11]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[18]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[20]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 9]); } }
			{ T &aI = a[13]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[22]); } }
			{ T &aI = a[ 5]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[15]); } }
			{ T &aI = a[ 7]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[17]); } }
			{ T &aI = a[ 1]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[10]); } }
			{ T &aI = a[12]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[21]); } }
			{ T &aI = a[ 8]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[19]); } }
			{ T &aI = a[17]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[22]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 5]); } }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[21]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[ 6]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[15]); } }
			{ T &aI = a[ 7]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[16]); } }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[19]); } }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[22]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[11]); } }
			{ T &aI = a[17]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[20]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 5]); } }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[12]); } }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[13]); } }
			{ T &aI = a[19]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[22]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 3]); } }
			{ T &aI = a[11]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[14]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[15]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[18]); } }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[21]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[17]); } }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[11]); } }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[16]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 9]); } }
			{ T &aI = a[13]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[19]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 7]); } }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[18]); } }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[14]); } }
			{ T &aI = a[ 7]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[15]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[18]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[21]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[13]); } }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[20]); } }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[14]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[21]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[13]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[18]); } }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[12]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[17]); } }
			{ T &aI = a[11]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[16]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 9]); } }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[20]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 8]); } }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[17]); } }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[10]); } }
			{ T &aI = a[13]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[16]); } }
			{ T &aI = a[12]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[15]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[20]); } }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[19]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 9]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[12]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[19]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[15]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[18]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[17]); } }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[14]); } }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[11]); } }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[12]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[16]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[13]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[14]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
		}
		//!built-in version on 24 items
		template <typename T,typename U> static inline void on24(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[19]); } }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[21]); } }
			{ T &aI = a[22]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[23]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[10]); } }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[15]); } }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[19]); } }
			{ T &aI = a[21]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[23]); } }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[14]); } }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[18]); } }
			{ T &aI = a[20]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[22]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[18]); } }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[22]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 4]); } }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[11]); } }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[16]); } }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[23]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[10]); } }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[17]); } }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[22]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 7]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 8]); } }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[19]); } }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[20]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 9]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[21]); } }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[18]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 4]); } }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[11]); } }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[16]); } }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[23]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 8]); } }
			{ T &aI = a[15]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[20]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[10]); } }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[17]); } }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[22]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[21]); } }
			{ T &aI = a[ 0]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[12]); } }
			{ T &aI = a[11]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[23]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[10]); } }
			{ T &aI = a[13]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[16]); } }
			{ T &aI = a[19]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[22]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[17]); } }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[20]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[16]); } }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[21]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[18]); } }
			{ T &aI = a[ 1]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[13]); } }
			{ T &aI = a[10]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[22]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[20]); } }
			{ T &aI = a[ 2]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[14]); } }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[17]); } }
			{ T &aI = a[ 6]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[18]); } }
			{ T &aI = a[ 9]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[21]); } }
			{ T &aI = a[ 3]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[15]); } }
			{ T &aI = a[ 4]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[16]); } }
			{ T &aI = a[ 7]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[19]); } }
			{ T &aI = a[ 8]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[20]); } }
			{ T &aI = a[ 2]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[12]); } }
			{ T &aI = a[11]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[21]); } }
			{ T &aI = a[ 6]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[14]); } }
			{ T &aI = a[ 9]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[17]); } }
			{ T &aI = a[ 3]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[13]); } }
			{ T &aI = a[10]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[20]); } }
			{ T &aI = a[ 4]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[12]); } }
			{ T &aI = a[ 7]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[15]); } }
			{ T &aI = a[ 8]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[16]); } }
			{ T &aI = a[11]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[19]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[22]); } }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[13]); } }
			{ T &aI = a[10]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[18]); } }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[12]); } }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[15]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[20]); } }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[13]); } }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[14]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[17]); } }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[12]); } }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[16]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[13]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[18]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
		}
		//!built-in version on 25 items
		template <typename T,typename U> static inline void on25(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[20]); } }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[22]); } }
			{ T &aI = a[23]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[24]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[14]); } }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[17]); } }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[20]); } }
			{ T &aI = a[21]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[23]); } }
			{ T &aI = a[22]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[24]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[11]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[17]); } }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[19]); } }
			{ T &aI = a[22]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[23]); } }
			{ T &aI = a[20]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[24]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 3]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 9]); } }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[10]); } }
			{ T &aI = a[ 5]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[11]); } }
			{ T &aI = a[12]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[15]); } }
			{ T &aI = a[13]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[16]); } }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[22]); } }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[23]); } }
			{ T &aI = a[17]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[24]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[10]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 6]); } }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[16]); } }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[15]); } }
			{ T &aI = a[18]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[21]); } }
			{ T &aI = a[20]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[23]); } }
			{ T &aI = a[11]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[24]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 7]); } }
			{ T &aI = a[ 4]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[10]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[21]); } }
			{ T &aI = a[20]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[22]); } }
			{ T &aI = a[16]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[23]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 8]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 6]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 9]); } }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[10]); } }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[21]); } }
			{ T &aI = a[12]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[19]); } }
			{ T &aI = a[15]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[22]); } }
			{ T &aI = a[17]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[23]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 7]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 9]); } }
			{ T &aI = a[12]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[18]); } }
			{ T &aI = a[13]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[20]); } }
			{ T &aI = a[14]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[21]); } }
			{ T &aI = a[16]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[22]); } }
			{ T &aI = a[10]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[23]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 9]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 7]); } }
			{ T &aI = a[14]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[20]); } }
			{ T &aI = a[13]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[18]); } }
			{ T &aI = a[17]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[22]); } }
			{ T &aI = a[11]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[23]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 6]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 8]); } }
			{ T &aI = a[14]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[19]); } }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[20]); } }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[21]); } }
			{ T &aI = a[ 0]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[13]); } }
			{ T &aI = a[ 9]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[22]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[18]); } }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[19]); } }
			{ T &aI = a[17]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[20]); } }
			{ T &aI = a[ 0]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[12]); } }
			{ T &aI = a[ 8]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[21]); } }
			{ T &aI = a[10]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[22]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[15]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[18]); } }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[19]); } }
			{ T &aI = a[ 1]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[14]); } }
			{ T &aI = a[ 7]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[20]); } }
			{ T &aI = a[11]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[22]); } }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[18]); } }
			{ T &aI = a[ 2]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[15]); } }
			{ T &aI = a[ 1]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[12]); } }
			{ T &aI = a[ 6]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[19]); } }
			{ T &aI = a[ 8]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[20]); } }
			{ T &aI = a[11]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[21]); } }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[18]); } }
			{ T &aI = a[ 2]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[14]); } }
			{ T &aI = a[ 3]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[16]); } }
			{ T &aI = a[ 7]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[19]); } }
			{ T &aI = a[10]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[20]); } }
			{ T &aI = a[ 2]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[13]); } }
			{ T &aI = a[ 4]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[17]); } }
			{ T &aI = a[ 5]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[18]); } }
			{ T &aI = a[ 8]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[19]); } }
			{ T &aI = a[11]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[20]); } }
			{ T &aI = a[ 2]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[12]); } }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[17]); } }
			{ T &aI = a[ 4]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[16]); } }
			{ T &aI = a[ 3]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[13]); } }
			{ T &aI = a[ 9]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[19]); } }
			{ T &aI = a[ 5]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[16]); } }
			{ T &aI = a[ 3]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[12]); } }
			{ T &aI = a[ 4]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[14]); } }
			{ T &aI = a[10]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[19]); } }
			{ T &aI = a[ 5]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[15]); } }
			{ T &aI = a[ 4]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[12]); } }
			{ T &aI = a[11]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[19]); } }
			{ T &aI = a[ 9]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[16]); } }
			{ T &aI = a[10]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[17]); } }
			{ T &aI = a[ 5]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[14]); } }
			{ T &aI = a[ 8]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[15]); } }
			{ T &aI = a[11]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[18]); } }
			{ T &aI = a[10]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[16]); } }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[13]); } }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[14]); } }
			{ T &aI = a[11]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[17]); } }
			{ T &aI = a[ 5]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[12]); } }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[13]); } }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[14]); } }
			{ T &aI = a[11]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[16]); } }
			{ T &aI = a[ 6]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[12]); } }
			{ T &aI = a[ 8]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[13]); } }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[14]); } }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[15]); } }
			{ T &aI = a[ 7]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[12]); } }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[13]); } }
			{ T &aI = a[11]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[14]); } }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[12]); } }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[13]); } }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[12]); } }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[12]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
		}
		//!built-in version on 26 items
		template <typename T,typename U> static inline void on26(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[18]); } }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[21]); } }
			{ T &aI = a[22]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[23]); } }
			{ T &aI = a[24]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[24],b[25]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[12]); } }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[15]); } }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[18]); } }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[21]); } }
			{ T &aI = a[22]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[24]); } }
			{ T &aI = a[23]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[25]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[12]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[15]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[18]); } }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[20]); } }
			{ T &aI = a[23]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[24]); } }
			{ T &aI = a[21]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[25]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 3]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[10]); } }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[11]); } }
			{ T &aI = a[ 5]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[12]); } }
			{ T &aI = a[13]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[16]); } }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[17]); } }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[23]); } }
			{ T &aI = a[20]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[24]); } }
			{ T &aI = a[18]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[25]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 9]); } }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[11]); } }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[17]); } }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[16]); } }
			{ T &aI = a[19]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[22]); } }
			{ T &aI = a[21]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[24]); } }
			{ T &aI = a[12]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[25]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[10]); } }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[11]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
			{ T &aI = a[20]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[22]); } }
			{ T &aI = a[21]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[23]); } }
			{ T &aI = a[17]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[24]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 7]); } }
			{ T &aI = a[ 3]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[10]); } }
			{ T &aI = a[ 5]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[11]); } }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[22]); } }
			{ T &aI = a[13]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[20]); } }
			{ T &aI = a[16]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[23]); } }
			{ T &aI = a[18]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[24]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 6]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 8]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 9]); } }
			{ T &aI = a[ 4]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[10]); } }
			{ T &aI = a[13]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[19]); } }
			{ T &aI = a[14]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[21]); } }
			{ T &aI = a[15]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[22]); } }
			{ T &aI = a[17]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[23]); } }
			{ T &aI = a[11]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[24]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 8]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 6]); } }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[10]); } }
			{ T &aI = a[15]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[21]); } }
			{ T &aI = a[14]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[19]); } }
			{ T &aI = a[18]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[23]); } }
			{ T &aI = a[ 0]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[13]); } }
			{ T &aI = a[12]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[24]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 7]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 8]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 9]); } }
			{ T &aI = a[15]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[20]); } }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[21]); } }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[22]); } }
			{ T &aI = a[ 1]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[14]); } }
			{ T &aI = a[10]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[23]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 7]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 8]); } }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[19]); } }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[20]); } }
			{ T &aI = a[18]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[21]); } }
			{ T &aI = a[ 1]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[13]); } }
			{ T &aI = a[ 9]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[22]); } }
			{ T &aI = a[12]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[23]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 6]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[16]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[19]); } }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[20]); } }
			{ T &aI = a[ 2]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[15]); } }
			{ T &aI = a[ 8]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[21]); } }
			{ T &aI = a[10]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[22]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[19]); } }
			{ T &aI = a[ 2]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[14]); } }
			{ T &aI = a[ 3]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[16]); } }
			{ T &aI = a[ 7]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[20]); } }
			{ T &aI = a[11]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[22]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[19]); } }
			{ T &aI = a[ 2]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[13]); } }
			{ T &aI = a[ 4]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[17]); } }
			{ T &aI = a[ 8]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[20]); } }
			{ T &aI = a[12]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[22]); } }
			{ T &aI = a[11]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[21]); } }
			{ T &aI = a[ 5]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[18]); } }
			{ T &aI = a[ 4]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[16]); } }
			{ T &aI = a[ 3]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[13]); } }
			{ T &aI = a[ 6]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[19]); } }
			{ T &aI = a[10]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[20]); } }
			{ T &aI = a[12]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[21]); } }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[17]); } }
			{ T &aI = a[ 4]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[14]); } }
			{ T &aI = a[ 7]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[19]); } }
			{ T &aI = a[12]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[20]); } }
			{ T &aI = a[ 5]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[16]); } }
			{ T &aI = a[ 4]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[13]); } }
			{ T &aI = a[ 8]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[19]); } }
			{ T &aI = a[ 5]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[15]); } }
			{ T &aI = a[ 9]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[19]); } }
			{ T &aI = a[ 5]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[14]); } }
			{ T &aI = a[10]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[19]); } }
			{ T &aI = a[ 8]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[15]); } }
			{ T &aI = a[ 9]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[16]); } }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[13]); } }
			{ T &aI = a[11]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[19]); } }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[14]); } }
			{ T &aI = a[10]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[17]); } }
			{ T &aI = a[12]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[19]); } }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[13]); } }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[14]); } }
			{ T &aI = a[10]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[16]); } }
			{ T &aI = a[11]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[18]); } }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[13]); } }
			{ T &aI = a[12]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[18]); } }
			{ T &aI = a[11]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[16]); } }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[14]); } }
			{ T &aI = a[ 8]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[13]); } }
			{ T &aI = a[12]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[17]); } }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[15]); } }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[16]); } }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[13]); } }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[13]); } }
			{ T &aI = a[12]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[15]); } }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[13]); } }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[14]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
		}
		//!built-in version on 27 items
		template <typename T,typename U> static inline void on27(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[19]); } }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[22]); } }
			{ T &aI = a[23]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[24]); } }
			{ T &aI = a[25]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[25],b[26]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[12]); } }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[15]); } }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[18]); } }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[19]); } }
			{ T &aI = a[20]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[22]); } }
			{ T &aI = a[23]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[25]); } }
			{ T &aI = a[24]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[24],b[26]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[12]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[18]); } }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[19]); } }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[21]); } }
			{ T &aI = a[24]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[24],b[25]); } }
			{ T &aI = a[22]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[26]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 3]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[10]); } }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[11]); } }
			{ T &aI = a[ 5]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[12]); } }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[17]); } }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[18]); } }
			{ T &aI = a[20]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[24]); } }
			{ T &aI = a[21]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[25]); } }
			{ T &aI = a[19]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[26]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 9]); } }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[11]); } }
			{ T &aI = a[13]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[16]); } }
			{ T &aI = a[15]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[18]); } }
			{ T &aI = a[20]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[23]); } }
			{ T &aI = a[22]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[25]); } }
			{ T &aI = a[12]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[26]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[10]); } }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[11]); } }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[16]); } }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[17]); } }
			{ T &aI = a[21]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[23]); } }
			{ T &aI = a[22]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[24]); } }
			{ T &aI = a[13]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[20]); } }
			{ T &aI = a[18]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[25]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 7]); } }
			{ T &aI = a[ 3]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[10]); } }
			{ T &aI = a[ 5]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[11]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
			{ T &aI = a[22]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[23]); } }
			{ T &aI = a[14]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[21]); } }
			{ T &aI = a[17]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[24]); } }
			{ T &aI = a[19]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[25]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 6]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 8]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 9]); } }
			{ T &aI = a[ 4]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[10]); } }
			{ T &aI = a[15]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[22]); } }
			{ T &aI = a[14]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[20]); } }
			{ T &aI = a[16]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[23]); } }
			{ T &aI = a[19]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[24]); } }
			{ T &aI = a[11]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[25]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 8]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 6]); } }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[10]); } }
			{ T &aI = a[15]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[21]); } }
			{ T &aI = a[17]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[23]); } }
			{ T &aI = a[ 0]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[14]); } }
			{ T &aI = a[12]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[25]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 7]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 8]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 9]); } }
			{ T &aI = a[15]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[20]); } }
			{ T &aI = a[18]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[23]); } }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[21]); } }
			{ T &aI = a[ 0]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[13]); } }
			{ T &aI = a[10]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[24]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 7]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 8]); } }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[23]); } }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[20]); } }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[22]); } }
			{ T &aI = a[ 1]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[15]); } }
			{ T &aI = a[12]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[24]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 6]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[17]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[20]); } }
			{ T &aI = a[19]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[22]); } }
			{ T &aI = a[ 2]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[16]); } }
			{ T &aI = a[ 1]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[13]); } }
			{ T &aI = a[ 9]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[23]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[20]); } }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[21]); } }
			{ T &aI = a[ 2]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[15]); } }
			{ T &aI = a[ 3]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[17]); } }
			{ T &aI = a[ 8]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[22]); } }
			{ T &aI = a[10]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[23]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[20]); } }
			{ T &aI = a[ 2]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[14]); } }
			{ T &aI = a[ 4]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[18]); } }
			{ T &aI = a[ 7]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[21]); } }
			{ T &aI = a[11]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[23]); } }
			{ T &aI = a[ 2]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[13]); } }
			{ T &aI = a[ 5]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[19]); } }
			{ T &aI = a[ 4]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[17]); } }
			{ T &aI = a[ 3]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[14]); } }
			{ T &aI = a[ 6]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[20]); } }
			{ T &aI = a[ 8]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[21]); } }
			{ T &aI = a[12]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[23]); } }
			{ T &aI = a[11]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[22]); } }
			{ T &aI = a[ 5]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[18]); } }
			{ T &aI = a[ 3]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[13]); } }
			{ T &aI = a[ 4]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[15]); } }
			{ T &aI = a[ 7]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[20]); } }
			{ T &aI = a[10]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[21]); } }
			{ T &aI = a[12]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[22]); } }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[17]); } }
			{ T &aI = a[ 4]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[13]); } }
			{ T &aI = a[ 8]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[20]); } }
			{ T &aI = a[12]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[21]); } }
			{ T &aI = a[ 5]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[16]); } }
			{ T &aI = a[ 9]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[20]); } }
			{ T &aI = a[ 5]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[15]); } }
			{ T &aI = a[10]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[20]); } }
			{ T &aI = a[ 9]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[16]); } }
			{ T &aI = a[ 5]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[14]); } }
			{ T &aI = a[11]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[20]); } }
			{ T &aI = a[ 8]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[15]); } }
			{ T &aI = a[10]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[17]); } }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[13]); } }
			{ T &aI = a[12]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[20]); } }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[14]); } }
			{ T &aI = a[10]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[16]); } }
			{ T &aI = a[11]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[18]); } }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[13]); } }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[14]); } }
			{ T &aI = a[12]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[19]); } }
			{ T &aI = a[11]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[16]); } }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[13]); } }
			{ T &aI = a[12]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[18]); } }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[14]); } }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[15]); } }
			{ T &aI = a[ 8]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[13]); } }
			{ T &aI = a[12]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[17]); } }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[16]); } }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[13]); } }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[13]); } }
			{ T &aI = a[12]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[15]); } }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[13]); } }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[14]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
		}
		//!built-in version on 28 items
		template <typename T,typename U> static inline void on28(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[18]); } }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[20]); } }
			{ T &aI = a[22]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[23]); } }
			{ T &aI = a[24]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[24],b[25]); } }
			{ T &aI = a[26]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[26],b[27]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[12]); } }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[13]); } }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[16]); } }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[19]); } }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[20]); } }
			{ T &aI = a[21]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[23]); } }
			{ T &aI = a[24]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[24],b[26]); } }
			{ T &aI = a[25]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[25],b[27]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[13]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[19]); } }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[20]); } }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[22]); } }
			{ T &aI = a[25]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[25],b[26]); } }
			{ T &aI = a[23]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[27]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 4]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 5]); } }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[11]); } }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[12]); } }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[13]); } }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[18]); } }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[19]); } }
			{ T &aI = a[21]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[25]); } }
			{ T &aI = a[22]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[26]); } }
			{ T &aI = a[20]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[27]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 3]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 5]); } }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[10]); } }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[12]); } }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[17]); } }
			{ T &aI = a[16]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[19]); } }
			{ T &aI = a[21]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[24]); } }
			{ T &aI = a[23]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[26]); } }
			{ T &aI = a[13]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[27]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[10]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 7]); } }
			{ T &aI = a[ 5]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[12]); } }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[17]); } }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[18]); } }
			{ T &aI = a[22]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[24]); } }
			{ T &aI = a[23]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[25]); } }
			{ T &aI = a[14]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[21]); } }
			{ T &aI = a[19]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[26]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 8]); } }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[11]); } }
			{ T &aI = a[ 6]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[12]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[23]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[24]); } }
			{ T &aI = a[15]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[22]); } }
			{ T &aI = a[18]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[25]); } }
			{ T &aI = a[20]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[26]); } }
			{ T &aI = a[ 0]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[14]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 9]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 7]); } }
			{ T &aI = a[ 3]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[10]); } }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[11]); } }
			{ T &aI = a[16]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[23]); } }
			{ T &aI = a[15]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[21]); } }
			{ T &aI = a[17]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[24]); } }
			{ T &aI = a[20]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[25]); } }
			{ T &aI = a[12]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[26]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 8]); } }
			{ T &aI = a[ 4]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[10]); } }
			{ T &aI = a[16]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[22]); } }
			{ T &aI = a[18]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[24]); } }
			{ T &aI = a[ 1]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[15]); } }
			{ T &aI = a[11]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[25]); } }
			{ T &aI = a[13]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[26]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 7]); } }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[10]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 8]); } }
			{ T &aI = a[16]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[21]); } }
			{ T &aI = a[19]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[24]); } }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[22]); } }
			{ T &aI = a[ 1]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[14]); } }
			{ T &aI = a[13]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[25]); } }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[10]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 7]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 9]); } }
			{ T &aI = a[20]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[24]); } }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[21]); } }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[23]); } }
			{ T &aI = a[ 2]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[16]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 7]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 9]); } }
			{ T &aI = a[18]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[21]); } }
			{ T &aI = a[20]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[23]); } }
			{ T &aI = a[ 2]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[15]); } }
			{ T &aI = a[ 3]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[17]); } }
			{ T &aI = a[10]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[24]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[21]); } }
			{ T &aI = a[20]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[22]); } }
			{ T &aI = a[ 2]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[14]); } }
			{ T &aI = a[ 4]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[18]); } }
			{ T &aI = a[ 9]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[23]); } }
			{ T &aI = a[11]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[24]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[21]); } }
			{ T &aI = a[ 4]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[17]); } }
			{ T &aI = a[ 5]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[19]); } }
			{ T &aI = a[ 3]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[14]); } }
			{ T &aI = a[ 8]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[22]); } }
			{ T &aI = a[12]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[24]); } }
			{ T &aI = a[ 6]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[20]); } }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[17]); } }
			{ T &aI = a[ 4]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[15]); } }
			{ T &aI = a[ 7]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[21]); } }
			{ T &aI = a[ 9]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[22]); } }
			{ T &aI = a[13]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[24]); } }
			{ T &aI = a[12]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[23]); } }
			{ T &aI = a[ 6]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[19]); } }
			{ T &aI = a[ 4]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[14]); } }
			{ T &aI = a[ 5]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[16]); } }
			{ T &aI = a[ 8]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[21]); } }
			{ T &aI = a[11]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[22]); } }
			{ T &aI = a[13]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[23]); } }
			{ T &aI = a[ 6]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[18]); } }
			{ T &aI = a[ 5]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[14]); } }
			{ T &aI = a[ 9]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[21]); } }
			{ T &aI = a[13]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[22]); } }
			{ T &aI = a[ 6]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[17]); } }
			{ T &aI = a[10]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[21]); } }
			{ T &aI = a[ 6]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[16]); } }
			{ T &aI = a[11]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[21]); } }
			{ T &aI = a[10]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[17]); } }
			{ T &aI = a[ 6]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[15]); } }
			{ T &aI = a[12]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[21]); } }
			{ T &aI = a[ 9]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[16]); } }
			{ T &aI = a[11]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[18]); } }
			{ T &aI = a[ 6]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[14]); } }
			{ T &aI = a[13]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[21]); } }
			{ T &aI = a[ 8]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[15]); } }
			{ T &aI = a[11]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[17]); } }
			{ T &aI = a[12]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[19]); } }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[14]); } }
			{ T &aI = a[ 9]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[15]); } }
			{ T &aI = a[13]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[20]); } }
			{ T &aI = a[12]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[17]); } }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[14]); } }
			{ T &aI = a[13]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[19]); } }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[15]); } }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[16]); } }
			{ T &aI = a[ 9]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[14]); } }
			{ T &aI = a[13]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[18]); } }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[17]); } }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[14]); } }
			{ T &aI = a[11]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[14]); } }
			{ T &aI = a[13]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[16]); } }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[14]); } }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[15]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
		}
		//!built-in version on 29 items
		template <typename T,typename U> static inline void on29(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[18]); } }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[20]); } }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[22]); } }
			{ T &aI = a[23]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[24]); } }
			{ T &aI = a[25]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[25],b[26]); } }
			{ T &aI = a[27]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[27],b[28]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[12]); } }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[13]); } }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[16]); } }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[19]); } }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[20]); } }
			{ T &aI = a[21]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[23]); } }
			{ T &aI = a[22]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[24]); } }
			{ T &aI = a[25]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[25],b[27]); } }
			{ T &aI = a[26]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[26],b[28]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[13]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[19]); } }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[20]); } }
			{ T &aI = a[22]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[23]); } }
			{ T &aI = a[26]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[26],b[27]); } }
			{ T &aI = a[21]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[25]); } }
			{ T &aI = a[24]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[24],b[28]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 4]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 5]); } }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[11]); } }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[12]); } }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[13]); } }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[18]); } }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[19]); } }
			{ T &aI = a[22]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[26]); } }
			{ T &aI = a[23]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[27]); } }
			{ T &aI = a[20]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[28]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 3]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 5]); } }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[10]); } }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[12]); } }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[17]); } }
			{ T &aI = a[16]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[19]); } }
			{ T &aI = a[22]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[25]); } }
			{ T &aI = a[24]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[24],b[27]); } }
			{ T &aI = a[13]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[28]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[10]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 7]); } }
			{ T &aI = a[ 5]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[12]); } }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[17]); } }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[18]); } }
			{ T &aI = a[23]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[25]); } }
			{ T &aI = a[24]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[24],b[26]); } }
			{ T &aI = a[14]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[22]); } }
			{ T &aI = a[19]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[27]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 8]); } }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[11]); } }
			{ T &aI = a[ 6]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[12]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[24]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[24],b[25]); } }
			{ T &aI = a[14]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[21]); } }
			{ T &aI = a[15]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[23]); } }
			{ T &aI = a[18]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[26]); } }
			{ T &aI = a[20]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[27]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 9]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 7]); } }
			{ T &aI = a[ 3]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[10]); } }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[11]); } }
			{ T &aI = a[16]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[24]); } }
			{ T &aI = a[15]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[21]); } }
			{ T &aI = a[17]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[25]); } }
			{ T &aI = a[20]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[26]); } }
			{ T &aI = a[12]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[27]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 8]); } }
			{ T &aI = a[ 4]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[10]); } }
			{ T &aI = a[16]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[23]); } }
			{ T &aI = a[18]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[25]); } }
			{ T &aI = a[ 0]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[15]); } }
			{ T &aI = a[11]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[26]); } }
			{ T &aI = a[13]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[27]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 7]); } }
			{ T &aI = a[ 5]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[10]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 8]); } }
			{ T &aI = a[16]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[22]); } }
			{ T &aI = a[19]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[25]); } }
			{ T &aI = a[ 0]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[14]); } }
			{ T &aI = a[13]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[26]); } }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[10]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 7]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 9]); } }
			{ T &aI = a[16]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[21]); } }
			{ T &aI = a[20]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[25]); } }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[22]); } }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[23]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 7]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 9]); } }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[21]); } }
			{ T &aI = a[20]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[24]); } }
			{ T &aI = a[ 1]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[16]); } }
			{ T &aI = a[10]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[25]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[18]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[21]); } }
			{ T &aI = a[20]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[23]); } }
			{ T &aI = a[ 2]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[17]); } }
			{ T &aI = a[ 1]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[14]); } }
			{ T &aI = a[ 9]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[24]); } }
			{ T &aI = a[11]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[25]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[21]); } }
			{ T &aI = a[20]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[22]); } }
			{ T &aI = a[ 2]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[16]); } }
			{ T &aI = a[ 3]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[18]); } }
			{ T &aI = a[ 8]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[23]); } }
			{ T &aI = a[12]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[25]); } }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[21]); } }
			{ T &aI = a[ 2]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[15]); } }
			{ T &aI = a[ 4]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[19]); } }
			{ T &aI = a[ 7]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[22]); } }
			{ T &aI = a[ 9]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[23]); } }
			{ T &aI = a[13]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[25]); } }
			{ T &aI = a[12]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[24]); } }
			{ T &aI = a[ 2]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[14]); } }
			{ T &aI = a[ 4]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[18]); } }
			{ T &aI = a[ 5]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[20]); } }
			{ T &aI = a[ 6]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[21]); } }
			{ T &aI = a[ 8]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[22]); } }
			{ T &aI = a[11]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[23]); } }
			{ T &aI = a[13]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[24]); } }
			{ T &aI = a[ 6]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[20]); } }
			{ T &aI = a[ 5]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[18]); } }
			{ T &aI = a[ 3]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[14]); } }
			{ T &aI = a[ 4]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[15]); } }
			{ T &aI = a[ 9]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[22]); } }
			{ T &aI = a[13]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[23]); } }
			{ T &aI = a[ 6]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[19]); } }
			{ T &aI = a[ 4]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[14]); } }
			{ T &aI = a[ 5]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[16]); } }
			{ T &aI = a[10]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[22]); } }
			{ T &aI = a[ 6]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[18]); } }
			{ T &aI = a[ 5]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[14]); } }
			{ T &aI = a[11]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[22]); } }
			{ T &aI = a[ 6]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[17]); } }
			{ T &aI = a[12]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[22]); } }
			{ T &aI = a[10]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[18]); } }
			{ T &aI = a[11]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[19]); } }
			{ T &aI = a[ 6]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[16]); } }
			{ T &aI = a[13]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[22]); } }
			{ T &aI = a[ 9]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[17]); } }
			{ T &aI = a[11]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[18]); } }
			{ T &aI = a[12]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[20]); } }
			{ T &aI = a[ 6]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[15]); } }
			{ T &aI = a[ 8]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[16]); } }
			{ T &aI = a[13]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[21]); } }
			{ T &aI = a[12]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[18]); } }
			{ T &aI = a[ 6]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[14]); } }
			{ T &aI = a[ 7]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[15]); } }
			{ T &aI = a[ 9]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[16]); } }
			{ T &aI = a[13]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[20]); } }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[14]); } }
			{ T &aI = a[ 9]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[15]); } }
			{ T &aI = a[13]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[19]); } }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[16]); } }
			{ T &aI = a[ 8]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[14]); } }
			{ T &aI = a[13]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[18]); } }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[15]); } }
			{ T &aI = a[ 9]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[14]); } }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[17]); } }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[14]); } }
			{ T &aI = a[13]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[16]); } }
			{ T &aI = a[11]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[14]); } }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[15]); } }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[14]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
		}
		//!built-in version on 30 items
		template <typename T,typename U> static inline void on30(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[19]); } }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[21]); } }
			{ T &aI = a[22]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[23]); } }
			{ T &aI = a[24]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[24],b[25]); } }
			{ T &aI = a[26]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[26],b[27]); } }
			{ T &aI = a[28]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[28],b[29]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[13]); } }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[14]); } }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[17]); } }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[20]); } }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[21]); } }
			{ T &aI = a[22]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[24]); } }
			{ T &aI = a[23]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[25]); } }
			{ T &aI = a[26]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[26],b[28]); } }
			{ T &aI = a[27]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[27],b[29]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[11]); } }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[14]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[20]); } }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[21]); } }
			{ T &aI = a[23]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[24]); } }
			{ T &aI = a[27]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[27],b[28]); } }
			{ T &aI = a[22]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[26]); } }
			{ T &aI = a[25]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[25],b[29]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 4]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 5]); } }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[12]); } }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[13]); } }
			{ T &aI = a[ 6]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[14]); } }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[19]); } }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[20]); } }
			{ T &aI = a[23]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[27]); } }
			{ T &aI = a[24]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[24],b[28]); } }
			{ T &aI = a[21]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[29]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 3]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 5]); } }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[11]); } }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[13]); } }
			{ T &aI = a[15]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[18]); } }
			{ T &aI = a[17]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[20]); } }
			{ T &aI = a[23]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[26]); } }
			{ T &aI = a[25]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[25],b[28]); } }
			{ T &aI = a[14]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[29]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[12]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 8]); } }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[13]); } }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[18]); } }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[19]); } }
			{ T &aI = a[24]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[24],b[26]); } }
			{ T &aI = a[25]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[25],b[27]); } }
			{ T &aI = a[15]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[23]); } }
			{ T &aI = a[20]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[28]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 7]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 9]); } }
			{ T &aI = a[ 4]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[12]); } }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[13]); } }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[18]); } }
			{ T &aI = a[25]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[25],b[26]); } }
			{ T &aI = a[15]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[22]); } }
			{ T &aI = a[16]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[24]); } }
			{ T &aI = a[19]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[27]); } }
			{ T &aI = a[21]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[28]); } }
			{ T &aI = a[ 2]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[10]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 7]); } }
			{ T &aI = a[ 3]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[11]); } }
			{ T &aI = a[ 6]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[12]); } }
			{ T &aI = a[17]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[25]); } }
			{ T &aI = a[16]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[22]); } }
			{ T &aI = a[18]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[26]); } }
			{ T &aI = a[21]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[27]); } }
			{ T &aI = a[ 0]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[15]); } }
			{ T &aI = a[13]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[28]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 9]); } }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[11]); } }
			{ T &aI = a[17]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[24]); } }
			{ T &aI = a[19]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[26]); } }
			{ T &aI = a[ 1]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[16]); } }
			{ T &aI = a[12]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[27]); } }
			{ T &aI = a[14]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[28]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 8]); } }
			{ T &aI = a[ 5]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[11]); } }
			{ T &aI = a[17]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[23]); } }
			{ T &aI = a[20]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[26]); } }
			{ T &aI = a[ 1]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[15]); } }
			{ T &aI = a[14]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[27]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 7]); } }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[11]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 8]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 9]); } }
			{ T &aI = a[17]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[22]); } }
			{ T &aI = a[21]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[26]); } }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[23]); } }
			{ T &aI = a[20]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[24]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 7]); } }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[10]); } }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[22]); } }
			{ T &aI = a[21]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[25]); } }
			{ T &aI = a[ 2]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[17]); } }
			{ T &aI = a[11]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[26]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 7]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 9]); } }
			{ T &aI = a[19]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[22]); } }
			{ T &aI = a[21]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[24]); } }
			{ T &aI = a[ 2]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[16]); } }
			{ T &aI = a[ 3]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[18]); } }
			{ T &aI = a[10]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[25]); } }
			{ T &aI = a[12]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[26]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[20]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[22]); } }
			{ T &aI = a[21]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[23]); } }
			{ T &aI = a[ 2]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[15]); } }
			{ T &aI = a[ 4]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[19]); } }
			{ T &aI = a[ 9]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[24]); } }
			{ T &aI = a[13]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[26]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[22]); } }
			{ T &aI = a[ 4]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[18]); } }
			{ T &aI = a[ 5]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[20]); } }
			{ T &aI = a[ 3]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[15]); } }
			{ T &aI = a[ 8]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[23]); } }
			{ T &aI = a[10]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[24]); } }
			{ T &aI = a[14]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[26]); } }
			{ T &aI = a[ 6]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[21]); } }
			{ T &aI = a[ 5]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[18]); } }
			{ T &aI = a[ 4]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[16]); } }
			{ T &aI = a[ 7]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[22]); } }
			{ T &aI = a[10]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[23]); } }
			{ T &aI = a[13]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[24]); } }
			{ T &aI = a[14]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[25]); } }
			{ T &aI = a[ 6]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[20]); } }
			{ T &aI = a[ 4]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[15]); } }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[17]); } }
			{ T &aI = a[ 8]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[22]); } }
			{ T &aI = a[12]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[23]); } }
			{ T &aI = a[14]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[24]); } }
			{ T &aI = a[ 6]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[19]); } }
			{ T &aI = a[ 5]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[15]); } }
			{ T &aI = a[ 9]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[22]); } }
			{ T &aI = a[14]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[23]); } }
			{ T &aI = a[ 6]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[18]); } }
			{ T &aI = a[10]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[22]); } }
			{ T &aI = a[ 6]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[17]); } }
			{ T &aI = a[11]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[22]); } }
			{ T &aI = a[10]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[18]); } }
			{ T &aI = a[ 6]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[16]); } }
			{ T &aI = a[12]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[22]); } }
			{ T &aI = a[ 9]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[17]); } }
			{ T &aI = a[11]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[19]); } }
			{ T &aI = a[ 6]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[15]); } }
			{ T &aI = a[13]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[22]); } }
			{ T &aI = a[ 8]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[16]); } }
			{ T &aI = a[10]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[17]); } }
			{ T &aI = a[12]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[20]); } }
			{ T &aI = a[14]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[22]); } }
			{ T &aI = a[ 7]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[15]); } }
			{ T &aI = a[10]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[16]); } }
			{ T &aI = a[12]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[19]); } }
			{ T &aI = a[13]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[21]); } }
			{ T &aI = a[ 8]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[15]); } }
			{ T &aI = a[14]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[21]); } }
			{ T &aI = a[13]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[19]); } }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[16]); } }
			{ T &aI = a[ 9]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[15]); } }
			{ T &aI = a[14]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[20]); } }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[17]); } }
			{ T &aI = a[10]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[15]); } }
			{ T &aI = a[14]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[19]); } }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[15]); } }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[18]); } }
			{ T &aI = a[12]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[15]); } }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[17]); } }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[15]); } }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[16]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
		}
		//!built-in version on 31 items
		template <typename T,typename U> static inline void on31(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[18]); } }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[20]); } }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[22]); } }
			{ T &aI = a[23]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[24]); } }
			{ T &aI = a[25]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[25],b[26]); } }
			{ T &aI = a[27]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[27],b[28]); } }
			{ T &aI = a[29]; T &aJ = a[30]; if(aJ<aI) { bswap(aI,aJ); bswap(b[29],b[30]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[10]); } }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[13]); } }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[14]); } }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[17]); } }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[18]); } }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[21]); } }
			{ T &aI = a[20]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[22]); } }
			{ T &aI = a[23]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[25]); } }
			{ T &aI = a[24]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[24],b[26]); } }
			{ T &aI = a[27]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[27],b[29]); } }
			{ T &aI = a[28]; T &aJ = a[30]; if(aJ<aI) { bswap(aI,aJ); bswap(b[28],b[30]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[11]); } }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[14]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[21]); } }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[19]); } }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[22]); } }
			{ T &aI = a[24]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[24],b[25]); } }
			{ T &aI = a[28]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[28],b[29]); } }
			{ T &aI = a[23]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[27]); } }
			{ T &aI = a[26]; T &aJ = a[30]; if(aJ<aI) { bswap(aI,aJ); bswap(b[26],b[30]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 4]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 5]); } }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[12]); } }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[13]); } }
			{ T &aI = a[ 6]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[14]); } }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[20]); } }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[21]); } }
			{ T &aI = a[24]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[24],b[28]); } }
			{ T &aI = a[25]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[25],b[29]); } }
			{ T &aI = a[15]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[23]); } }
			{ T &aI = a[22]; T &aJ = a[30]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[30]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 3]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 5]); } }
			{ T &aI = a[ 8]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[11]); } }
			{ T &aI = a[10]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[13]); } }
			{ T &aI = a[16]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[19]); } }
			{ T &aI = a[18]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[21]); } }
			{ T &aI = a[24]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[24],b[27]); } }
			{ T &aI = a[26]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[26],b[29]); } }
			{ T &aI = a[14]; T &aJ = a[30]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[30]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[12]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 8]); } }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[13]); } }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[19]); } }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[20]); } }
			{ T &aI = a[25]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[25],b[27]); } }
			{ T &aI = a[26]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[26],b[28]); } }
			{ T &aI = a[16]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[24]); } }
			{ T &aI = a[21]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[29]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 7]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 9]); } }
			{ T &aI = a[ 4]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[12]); } }
			{ T &aI = a[ 6]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[13]); } }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[19]); } }
			{ T &aI = a[26]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[26],b[27]); } }
			{ T &aI = a[16]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[23]); } }
			{ T &aI = a[17]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[25]); } }
			{ T &aI = a[20]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[28]); } }
			{ T &aI = a[22]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[29]); } }
			{ T &aI = a[ 2]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[10]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 7]); } }
			{ T &aI = a[ 3]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[11]); } }
			{ T &aI = a[ 6]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[12]); } }
			{ T &aI = a[18]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[26]); } }
			{ T &aI = a[17]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[23]); } }
			{ T &aI = a[19]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[27]); } }
			{ T &aI = a[22]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[28]); } }
			{ T &aI = a[ 0]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[16]); } }
			{ T &aI = a[13]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[29]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 9]); } }
			{ T &aI = a[ 4]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[11]); } }
			{ T &aI = a[18]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[25]); } }
			{ T &aI = a[20]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[27]); } }
			{ T &aI = a[ 0]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[15]); } }
			{ T &aI = a[ 1]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[17]); } }
			{ T &aI = a[12]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[28]); } }
			{ T &aI = a[14]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[29]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 8]); } }
			{ T &aI = a[ 5]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[11]); } }
			{ T &aI = a[18]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[24]); } }
			{ T &aI = a[21]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[27]); } }
			{ T &aI = a[ 1]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[15]); } }
			{ T &aI = a[14]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[28]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 7]); } }
			{ T &aI = a[ 6]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[11]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 8]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 9]); } }
			{ T &aI = a[18]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[23]); } }
			{ T &aI = a[22]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[27]); } }
			{ T &aI = a[20]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[24]); } }
			{ T &aI = a[21]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[25]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 7]); } }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[10]); } }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[23]); } }
			{ T &aI = a[22]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[26]); } }
			{ T &aI = a[ 2]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[18]); } }
			{ T &aI = a[11]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[27]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 7]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 9]); } }
			{ T &aI = a[20]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[23]); } }
			{ T &aI = a[22]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[25]); } }
			{ T &aI = a[ 2]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[17]); } }
			{ T &aI = a[ 3]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[19]); } }
			{ T &aI = a[10]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[26]); } }
			{ T &aI = a[12]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[27]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[21]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[23]); } }
			{ T &aI = a[22]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[24]); } }
			{ T &aI = a[ 2]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[16]); } }
			{ T &aI = a[ 4]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[20]); } }
			{ T &aI = a[ 9]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[25]); } }
			{ T &aI = a[13]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[27]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[22]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[23]); } }
			{ T &aI = a[ 2]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[15]); } }
			{ T &aI = a[ 4]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[19]); } }
			{ T &aI = a[ 5]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[21]); } }
			{ T &aI = a[ 8]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[24]); } }
			{ T &aI = a[10]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[25]); } }
			{ T &aI = a[14]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[27]); } }
			{ T &aI = a[ 6]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[22]); } }
			{ T &aI = a[ 5]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[19]); } }
			{ T &aI = a[ 3]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[15]); } }
			{ T &aI = a[ 4]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[16]); } }
			{ T &aI = a[ 7]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[23]); } }
			{ T &aI = a[10]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[24]); } }
			{ T &aI = a[13]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[25]); } }
			{ T &aI = a[14]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[26]); } }
			{ T &aI = a[ 6]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[21]); } }
			{ T &aI = a[ 4]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[15]); } }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[17]); } }
			{ T &aI = a[ 8]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[23]); } }
			{ T &aI = a[12]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[24]); } }
			{ T &aI = a[14]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[25]); } }
			{ T &aI = a[ 6]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[20]); } }
			{ T &aI = a[ 5]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[15]); } }
			{ T &aI = a[ 9]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[23]); } }
			{ T &aI = a[14]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[24]); } }
			{ T &aI = a[ 6]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[19]); } }
			{ T &aI = a[10]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[23]); } }
			{ T &aI = a[ 6]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[18]); } }
			{ T &aI = a[11]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[23]); } }
			{ T &aI = a[ 6]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[17]); } }
			{ T &aI = a[12]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[23]); } }
			{ T &aI = a[10]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[18]); } }
			{ T &aI = a[11]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[19]); } }
			{ T &aI = a[ 6]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[16]); } }
			{ T &aI = a[13]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[23]); } }
			{ T &aI = a[ 9]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[17]); } }
			{ T &aI = a[12]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[20]); } }
			{ T &aI = a[ 6]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[15]); } }
			{ T &aI = a[14]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[23]); } }
			{ T &aI = a[ 8]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[16]); } }
			{ T &aI = a[10]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[17]); } }
			{ T &aI = a[12]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[19]); } }
			{ T &aI = a[13]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[21]); } }
			{ T &aI = a[ 7]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[15]); } }
			{ T &aI = a[10]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[16]); } }
			{ T &aI = a[14]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[22]); } }
			{ T &aI = a[13]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[19]); } }
			{ T &aI = a[ 8]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[15]); } }
			{ T &aI = a[14]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[21]); } }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[16]); } }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[17]); } }
			{ T &aI = a[ 9]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[15]); } }
			{ T &aI = a[14]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[20]); } }
			{ T &aI = a[10]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[15]); } }
			{ T &aI = a[14]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[19]); } }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[15]); } }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[18]); } }
			{ T &aI = a[12]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[15]); } }
			{ T &aI = a[14]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[17]); } }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[15]); } }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[16]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
		}
		//!built-in version on 32 items
		template <typename T,typename U> static inline void on32(T *a, U *b) {
			assert(a); assert(b);
			{ T &aI = a[ 0]; T &aJ = a[ 1]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 1]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 3]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 5]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 7]); } }
			{ T &aI = a[ 8]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[ 9]); } }
			{ T &aI = a[10]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[11]); } }
			{ T &aI = a[12]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[13]); } }
			{ T &aI = a[14]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[15]); } }
			{ T &aI = a[16]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[17]); } }
			{ T &aI = a[18]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[19]); } }
			{ T &aI = a[20]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[21]); } }
			{ T &aI = a[22]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[23]); } }
			{ T &aI = a[24]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[24],b[25]); } }
			{ T &aI = a[26]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[26],b[27]); } }
			{ T &aI = a[28]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[28],b[29]); } }
			{ T &aI = a[30]; T &aJ = a[31]; if(aJ<aI) { bswap(aI,aJ); bswap(b[30],b[31]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 2]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 3]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 3]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 6]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 7]); } }
			{ T &aI = a[ 8]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[10]); } }
			{ T &aI = a[ 9]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[11]); } }
			{ T &aI = a[12]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[14]); } }
			{ T &aI = a[13]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[15]); } }
			{ T &aI = a[16]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[18]); } }
			{ T &aI = a[17]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[19]); } }
			{ T &aI = a[20]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[22]); } }
			{ T &aI = a[21]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[23]); } }
			{ T &aI = a[24]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[24],b[26]); } }
			{ T &aI = a[25]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[25],b[27]); } }
			{ T &aI = a[28]; T &aJ = a[30]; if(aJ<aI) { bswap(aI,aJ); bswap(b[28],b[30]); } }
			{ T &aI = a[29]; T &aJ = a[31]; if(aJ<aI) { bswap(aI,aJ); bswap(b[29],b[31]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 2]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 2]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 6]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 4]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 7]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 7]); } }
			{ T &aI = a[ 9]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[10]); } }
			{ T &aI = a[13]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[14]); } }
			{ T &aI = a[ 8]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[12]); } }
			{ T &aI = a[11]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[15]); } }
			{ T &aI = a[17]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[18]); } }
			{ T &aI = a[21]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[22]); } }
			{ T &aI = a[16]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[20]); } }
			{ T &aI = a[19]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[23]); } }
			{ T &aI = a[25]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[25],b[26]); } }
			{ T &aI = a[29]; T &aJ = a[30]; if(aJ<aI) { bswap(aI,aJ); bswap(b[29],b[30]); } }
			{ T &aI = a[24]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[24],b[28]); } }
			{ T &aI = a[27]; T &aJ = a[31]; if(aJ<aI) { bswap(aI,aJ); bswap(b[27],b[31]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 5]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 6]); } }
			{ T &aI = a[ 9]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[13]); } }
			{ T &aI = a[10]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[14]); } }
			{ T &aI = a[ 0]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[ 8]); } }
			{ T &aI = a[ 7]; T &aJ = a[15]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[15]); } }
			{ T &aI = a[17]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[21]); } }
			{ T &aI = a[18]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[22]); } }
			{ T &aI = a[25]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[25],b[29]); } }
			{ T &aI = a[26]; T &aJ = a[30]; if(aJ<aI) { bswap(aI,aJ); bswap(b[26],b[30]); } }
			{ T &aI = a[16]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[16],b[24]); } }
			{ T &aI = a[23]; T &aJ = a[31]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[31]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 4]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 6]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 6]); } }
			{ T &aI = a[ 9]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[12]); } }
			{ T &aI = a[11]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[14]); } }
			{ T &aI = a[17]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[20]); } }
			{ T &aI = a[19]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[22]); } }
			{ T &aI = a[25]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[25],b[28]); } }
			{ T &aI = a[27]; T &aJ = a[30]; if(aJ<aI) { bswap(aI,aJ); bswap(b[27],b[30]); } }
			{ T &aI = a[ 0]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 0],b[16]); } }
			{ T &aI = a[15]; T &aJ = a[31]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[31]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 4]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 5]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 5]); } }
			{ T &aI = a[10]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[12]); } }
			{ T &aI = a[11]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[13]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 9]); } }
			{ T &aI = a[ 6]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[14]); } }
			{ T &aI = a[18]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[20]); } }
			{ T &aI = a[19]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[21]); } }
			{ T &aI = a[26]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[26],b[28]); } }
			{ T &aI = a[27]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[27],b[29]); } }
			{ T &aI = a[17]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[25]); } }
			{ T &aI = a[22]; T &aJ = a[30]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[30]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 4]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 4]); } }
			{ T &aI = a[11]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[12]); } }
			{ T &aI = a[ 1]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[ 8]); } }
			{ T &aI = a[ 2]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[10]); } }
			{ T &aI = a[ 5]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[13]); } }
			{ T &aI = a[ 7]; T &aJ = a[14]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[14]); } }
			{ T &aI = a[19]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[20]); } }
			{ T &aI = a[27]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[27],b[28]); } }
			{ T &aI = a[17]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[17],b[24]); } }
			{ T &aI = a[18]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[26]); } }
			{ T &aI = a[21]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[29]); } }
			{ T &aI = a[23]; T &aJ = a[30]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[30]); } }
			{ T &aI = a[ 3]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[11]); } }
			{ T &aI = a[ 2]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[ 8]); } }
			{ T &aI = a[ 4]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[12]); } }
			{ T &aI = a[ 7]; T &aJ = a[13]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[13]); } }
			{ T &aI = a[19]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[27]); } }
			{ T &aI = a[18]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[18],b[24]); } }
			{ T &aI = a[20]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[28]); } }
			{ T &aI = a[23]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[29]); } }
			{ T &aI = a[ 1]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[17]); } }
			{ T &aI = a[14]; T &aJ = a[30]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[30]); } }
			{ T &aI = a[ 3]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[10]); } }
			{ T &aI = a[ 5]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[12]); } }
			{ T &aI = a[19]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[26]); } }
			{ T &aI = a[21]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[28]); } }
			{ T &aI = a[ 1]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 1],b[16]); } }
			{ T &aI = a[ 2]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[18]); } }
			{ T &aI = a[13]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[29]); } }
			{ T &aI = a[15]; T &aJ = a[30]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[30]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 9]); } }
			{ T &aI = a[ 6]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[12]); } }
			{ T &aI = a[19]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[25]); } }
			{ T &aI = a[22]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[28]); } }
			{ T &aI = a[ 2]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 2],b[16]); } }
			{ T &aI = a[15]; T &aJ = a[29]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[29]); } }
			{ T &aI = a[ 3]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[ 8]); } }
			{ T &aI = a[ 7]; T &aJ = a[12]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[12]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 9]); } }
			{ T &aI = a[ 6]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[10]); } }
			{ T &aI = a[19]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[19],b[24]); } }
			{ T &aI = a[23]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[28]); } }
			{ T &aI = a[21]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[25]); } }
			{ T &aI = a[22]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[26]); } }
			{ T &aI = a[ 4]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[ 8]); } }
			{ T &aI = a[ 7]; T &aJ = a[11]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[11]); } }
			{ T &aI = a[20]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[20],b[24]); } }
			{ T &aI = a[23]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[27]); } }
			{ T &aI = a[ 3]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[19]); } }
			{ T &aI = a[12]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[28]); } }
			{ T &aI = a[ 5]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[ 8]); } }
			{ T &aI = a[ 7]; T &aJ = a[10]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[10]); } }
			{ T &aI = a[21]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[21],b[24]); } }
			{ T &aI = a[23]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[26]); } }
			{ T &aI = a[ 3]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[18]); } }
			{ T &aI = a[ 4]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[20]); } }
			{ T &aI = a[11]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[27]); } }
			{ T &aI = a[13]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[28]); } }
			{ T &aI = a[ 6]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[ 8]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 9]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 9]); } }
			{ T &aI = a[22]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[22],b[24]); } }
			{ T &aI = a[23]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[25]); } }
			{ T &aI = a[ 3]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[17]); } }
			{ T &aI = a[ 5]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[21]); } }
			{ T &aI = a[10]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[26]); } }
			{ T &aI = a[14]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[28]); } }
			{ T &aI = a[ 7]; T &aJ = a[ 8]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[ 8]); } }
			{ T &aI = a[23]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[23],b[24]); } }
			{ T &aI = a[ 3]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 3],b[16]); } }
			{ T &aI = a[ 5]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[20]); } }
			{ T &aI = a[ 6]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[22]); } }
			{ T &aI = a[ 9]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[25]); } }
			{ T &aI = a[11]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[26]); } }
			{ T &aI = a[15]; T &aJ = a[28]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[28]); } }
			{ T &aI = a[ 7]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[23]); } }
			{ T &aI = a[ 6]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[20]); } }
			{ T &aI = a[ 4]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 4],b[16]); } }
			{ T &aI = a[ 5]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[17]); } }
			{ T &aI = a[ 8]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[24]); } }
			{ T &aI = a[11]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[25]); } }
			{ T &aI = a[14]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[26]); } }
			{ T &aI = a[15]; T &aJ = a[27]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[27]); } }
			{ T &aI = a[ 7]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[22]); } }
			{ T &aI = a[ 5]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 5],b[16]); } }
			{ T &aI = a[ 6]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[18]); } }
			{ T &aI = a[ 9]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[24]); } }
			{ T &aI = a[13]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[25]); } }
			{ T &aI = a[15]; T &aJ = a[26]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[26]); } }
			{ T &aI = a[ 7]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[21]); } }
			{ T &aI = a[ 6]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 6],b[16]); } }
			{ T &aI = a[10]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[24]); } }
			{ T &aI = a[15]; T &aJ = a[25]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[25]); } }
			{ T &aI = a[ 7]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[20]); } }
			{ T &aI = a[11]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[24]); } }
			{ T &aI = a[ 7]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[19]); } }
			{ T &aI = a[12]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[24]); } }
			{ T &aI = a[ 7]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[18]); } }
			{ T &aI = a[13]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[24]); } }
			{ T &aI = a[11]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[19]); } }
			{ T &aI = a[12]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[20]); } }
			{ T &aI = a[ 7]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[17]); } }
			{ T &aI = a[14]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[24]); } }
			{ T &aI = a[10]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[18]); } }
			{ T &aI = a[13]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[21]); } }
			{ T &aI = a[ 7]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 7],b[16]); } }
			{ T &aI = a[15]; T &aJ = a[24]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[24]); } }
			{ T &aI = a[ 9]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[17]); } }
			{ T &aI = a[11]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[18]); } }
			{ T &aI = a[13]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[20]); } }
			{ T &aI = a[14]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[22]); } }
			{ T &aI = a[ 8]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 8],b[16]); } }
			{ T &aI = a[11]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[17]); } }
			{ T &aI = a[15]; T &aJ = a[23]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[23]); } }
			{ T &aI = a[14]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[20]); } }
			{ T &aI = a[ 9]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[ 9],b[16]); } }
			{ T &aI = a[15]; T &aJ = a[22]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[22]); } }
			{ T &aI = a[13]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[17]); } }
			{ T &aI = a[14]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[18]); } }
			{ T &aI = a[10]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[10],b[16]); } }
			{ T &aI = a[15]; T &aJ = a[21]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[21]); } }
			{ T &aI = a[11]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[11],b[16]); } }
			{ T &aI = a[15]; T &aJ = a[20]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[20]); } }
			{ T &aI = a[12]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[12],b[16]); } }
			{ T &aI = a[15]; T &aJ = a[19]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[19]); } }
			{ T &aI = a[13]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[13],b[16]); } }
			{ T &aI = a[15]; T &aJ = a[18]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[18]); } }
			{ T &aI = a[14]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[14],b[16]); } }
			{ T &aI = a[15]; T &aJ = a[17]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[17]); } }
			{ T &aI = a[15]; T &aJ = a[16]; if(aJ<aI) { bswap(aI,aJ); bswap(b[15],b[16]); } }
		}
		//!built-in version on 0-32 items
		template <typename T, typename U> static inline void on(T *a, U *b, const size_t n) {
			switch(n) {
				case  2: on2(a,b);  break;
				case  3: on3(a,b);  break;
				case  4: on4(a,b);  break;
				case  5: on5(a,b);  break;
				case  6: on6(a,b);  break;
				case  7: on7(a,b);  break;
				case  8: on8(a,b);  break;
				case  9: on9(a,b);  break;
				case 10: on10(a,b); break;
				case 11: on11(a,b); break;
				case 12: on12(a,b); break;
				case 13: on13(a,b); break;
				case 14: on14(a,b); break;
				case 15: on15(a,b); break;
				case 16: on16(a,b); break;
				case 17: on17(a,b); break;
				case 18: on18(a,b); break;
				case 19: on19(a,b); break;
				case 20: on20(a,b); break;
				case 21: on21(a,b); break;
				case 22: on22(a,b); break;
				case 23: on23(a,b); break;
				case 24: on24(a,b); break;
				case 25: on25(a,b); break;
				case 26: on26(a,b); break;
				case 27: on27(a,b); break;
				case 28: on28(a,b); break;
				case 29: on29(a,b); break;
				case 30: on30(a,b); break;
				case 31: on31(a,b); break;
				case 32: on32(a,b); break;
				default: break;
			}
		}
	};
}
#endif
