//! \file
#ifndef Y_NW_sort15_INCLUDED
#define Y_NW_sort15_INCLUDED
#include "../network.hpp"
namespace upsylon {
		//! network sort for 15 items
	template <> struct network_sort<15> {
		static const size_t I[56]; //!< I swap indices
		static const size_t J[56]; //!< J swap indices
		//! sort...
		template <typename T,typename COMPARE> static inline
		void on(T *a, COMPARE &compare) throw() {
			assert(NULL!=a);
			for(size_t k=0;k<56;++k){
				T &aI=a[I[k]], &aJ=a[J[k]];
				if(compare(aJ,aI)<0) memops::swap<T>(aI,aJ);
			}
		}
		template <typename T,typename U,typename COMPARE> static inline
		void co(T *a, U *b, COMPARE &compare) throw() {
			assert(NULL!=a);
			assert(NULL!=b);
			for(size_t k=0;k<56;++k){
				const size_t  i = I[k],   j = J[k];
				T           &aI = a[i], &aJ = a[j];
				if(compare(aJ,aI)<0) { memops::swap<T>(aI,aJ); memops::swap<U>(b[i],b[j]); }
			}
		}
	};
}
#endif