//! \file
#ifndef Y_NW_sort3_INCLUDED
#define Y_NW_sort3_INCLUDED
#include "../network.hpp"
namespace upsylon {
		//! network sort for 3 items
	template <> struct network_sort<3> {
		static const size_t I[3]; //!< I swap indices
		static const size_t J[3]; //!< J swap indices
		//! sort...
		template <typename T,typename COMPARE> static inline
		void on(T *a, COMPARE &compare) throw() {
			assert(NULL!=a);
			for(size_t k=0;k<3;++k){
				const T &aI=a[I[k]], &aJ=a[J[k]];
				if(compare(aJ,aI)<0) bswap(aI,aJ);
			}
		}
	};
}
#endif