//! \file
#ifndef Y_NW_sort50_INCLUDED
#define Y_NW_sort50_INCLUDED
#include "../network.hpp"
namespace upsylon {
		//! network sort for 50 items
	template <> struct network_sort<50> {
		static const unsigned I[487]; //!< I swap indices
		static const unsigned J[487]; //!< J swap indices
		//! sort...
		template <typename T,typename COMPARE> static inline
		void on(T *a, COMPARE &compare) throw() {
			assert(NULL!=a);
			for(unsigned k=0;k<487;++k){
				T &aI=a[I[k]], &aJ=a[J[k]];
				if(compare(aJ,aI)<0) bswap<T>(aI,aJ);
			}
		}
		//! co-sorting...
		template <typename T,typename U,typename COMPARE> static inline
		void co(T *a, U *b, COMPARE &compare) throw() {
			assert(NULL!=a);
			assert(NULL!=b);
			for(unsigned k=0;k<487;++k){
				const unsigned  i = I[k],   j = J[k];
				T             &aI = a[i], &aJ = a[j];
				if(compare(aJ,aI)<0) { bswap<T>(aI,aJ); bswap<U>(b[i],b[j]); }
			}
		}
	};
}
#endif