//! \file
#ifndef Y_NW_sort11_INCLUDED
#define Y_NW_sort11_INCLUDED
#include "../network.hpp"
namespace upsylon {
		//! network sort for 11 items
	template <> struct network_sort<11> {
		static const size_t I[35]; //!< I swap indices
		static const size_t J[35]; //!< J swap indices
		//! sort...
		template <typename T,typename COMPARE> static inline
		void on(T *a, COMPARE &compare) throw() {
			assert(NULL!=a);
			for(size_t k=0;k<35;++k){
				const T &aI=a[I[k]], &aJ=a[J[k]];
				if(compare(aJ,aI)<0) bswap(aI,aJ);
			}
		}
	};
}
#endif