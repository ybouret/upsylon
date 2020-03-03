//! \file
#ifndef Y_NW_sort16_INCLUDED
#define Y_NW_sort16_INCLUDED
#include "../network.hpp"
namespace upsylon {
		//! network sort for 16 items
	template <> struct network_sort<16> {
		static const size_t I[61]; //!< I swap indices
		static const size_t J[61]; //!< J swap indices
		//! sort...
		template <typename T,typename COMPARE> static inline
		void on(T *a, COMPARE &compare) throw() {
			assert(NULL!=a);
			for(size_t k=0;k<61;++k){
				const T &aI=a[I[k]], &aJ=a[J[k]];
				if(compare(aJ,aI)<0) bswap(aI,aJ);
			}
		}
	};
}
#endif