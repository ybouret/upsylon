//! \file
#ifndef Y_NW_sort24_INCLUDED
#define Y_NW_sort24_INCLUDED
#include "../network.hpp"
namespace upsylon {
		//! network sort for 24 items
	template <> struct network_sort<24> {
		static const size_t I[123]; //!< I swap indices
		static const size_t J[123]; //!< J swap indices
		//! sort...
		template <typename T,typename COMPARE> static inline
		void on(T *a, COMPARE &compare) throw() {
			assert(NULL!=a);
			for(size_t k=0;k<123;++k){
				const T &aI=a[I[k]], &aJ=a[J[k]];
				if(compare(aJ,aI)<0) bswap(aI,aJ);
			}
		}
	};
}
#endif