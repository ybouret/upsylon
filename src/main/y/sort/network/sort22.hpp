//! \file
#ifndef Y_NW_sort22_INCLUDED
#define Y_NW_sort22_INCLUDED
#include "../network.hpp"
namespace upsylon {
		//! network sort for 22 items
	template <> struct network_sort<22> {
		static const size_t I[108]; //!< I swap indices
		static const size_t J[108]; //!< J swap indices
		//! sort...
		template <typename T,typename COMPARE> static inline
		void on(T *a, COMPARE &compare) throw() {
			assert(NULL!=a);
			for(size_t k=0;k<108;++k){
				const T &aI=a[I[k]], &aJ=a[J[k]];
				if(compare(aJ,aI)<0) bswap(aI,aJ);
			}
		}
	};
}
#endif