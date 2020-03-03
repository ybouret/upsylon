//! \file
#ifndef Y_NW_sort30_INCLUDED
#define Y_NW_sort30_INCLUDED
#include "../network.hpp"
namespace upsylon {
		//! network sort for 30 items
	template <> struct network_sort<30> {
		static const size_t I[198]; //!< I swap indices
		static const size_t J[198]; //!< J swap indices
		//! sort...
		template <typename T,typename COMPARE> static inline
		void on(T *a, COMPARE &compare) throw() {
			assert(NULL!=a);
			for(size_t k=0;k<198;++k){
				const T &aI=a[I[k]], &aJ=a[J[k]];
				if(compare(aJ,aI)<0) bswap(aI,aJ);
			}
		}
	};
}
#endif