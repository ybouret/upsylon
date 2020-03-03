//! \file
#ifndef Y_NW_sort31_INCLUDED
#define Y_NW_sort31_INCLUDED
#include "../network.hpp"
namespace upsylon {
		//! network sort for 31 items
	template <> struct network_sort<31> {
		static const size_t I[206]; //!< I swap indices
		static const size_t J[206]; //!< J swap indices
		//! sort...
		template <typename T,typename COMPARE> static inline
		void on(T *a, COMPARE &compare) throw() {
			assert(NULL!=a);
			for(size_t k=0;k<206;++k){
				const T &aI=a[I[k]], &aJ=a[J[k]];
				if(compare(aJ,aI)<0) bswap(aI,aJ);
			}
		}
	};
}
#endif