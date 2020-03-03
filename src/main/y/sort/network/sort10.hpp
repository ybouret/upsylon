//! \file
#ifndef Y_NW_sort10_INCLUDED
#define Y_NW_sort10_INCLUDED
#include "../network.hpp"
namespace upsylon {
		//! network sort for 10 items
	template <> struct network_sort<10> {
		static const size_t I[29]; //!< I swap indices
		static const size_t J[29]; //!< J swap indices
		//! sort...
		template <typename T,typename COMPARE> static inline
		void on(T *a, COMPARE &compare) throw() {
			assert(NULL!=a);
			for(size_t k=0;k<29;++k){
				const T &aI=a[I[k]], &aJ=a[J[k]];
				if(compare(aJ,aI)<0) bswap(aI,aJ);
			}
		}
	};
}
#endif