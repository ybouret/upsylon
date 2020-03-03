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
				const T &aI=a[I[k]], &aJ=a[J[k]];
				if(compare(aJ,aI)<0) bswap(aI,aJ);
			}
		}
	};
}
#endif