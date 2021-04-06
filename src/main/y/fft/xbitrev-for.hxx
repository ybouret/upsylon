template <typename T> struct xbitrev_for<T,0> {
	static inline void run(T []) throw() {}
};

template <typename T> struct xbitrev_for<T,1> {
	static inline void run(T []) throw() {}
};

template <typename T> struct xbitrev_for<T,2> {
	static inline void run(T []) throw() {}
};

template <typename T> struct xbitrev_for<T,4> {
	static inline void run(T arr[]) throw() {
	assert(arr);
	for(size_t i=0;i<    1;++i) Y_XBITREV_SWAP(xbitrev::indx4[i],xbitrev::jndx4[i]);
	};
};

template <typename T> struct xbitrev_for<T,8> {
	static inline void run(T arr[]) throw() {
	assert(arr);
	for(size_t i=0;i<    2;++i) Y_XBITREV_SWAP(xbitrev::indx8[i],xbitrev::jndx8[i]);
	};
};

template <typename T> struct xbitrev_for<T,16> {
	static inline void run(T arr[]) throw() {
	assert(arr);
	for(size_t i=0;i<    6;++i) Y_XBITREV_SWAP(xbitrev::indx16[i],xbitrev::jndx16[i]);
	};
};

template <typename T> struct xbitrev_for<T,32> {
	static inline void run(T arr[]) throw() {
	assert(arr);
	for(size_t i=0;i<   12;++i) Y_XBITREV_SWAP(xbitrev::indx32[i],xbitrev::jndx32[i]);
	};
};

template <typename T> struct xbitrev_for<T,64> {
	static inline void run(T arr[]) throw() {
	assert(arr);
	for(size_t i=0;i<   28;++i) Y_XBITREV_SWAP(xbitrev::indx64[i],xbitrev::jndx64[i]);
	};
};

template <typename T> struct xbitrev_for<T,128> {
	static inline void run(T arr[]) throw() {
	assert(arr);
	for(size_t i=0;i<   56;++i) Y_XBITREV_SWAP(xbitrev::indx128[i],xbitrev::jndx128[i]);
	};
};

template <typename T> struct xbitrev_for<T,256> {
	static inline void run(T arr[]) throw() {
	assert(arr);
	for(size_t i=0;i<  120;++i) Y_XBITREV_SWAP(xbitrev::indx256[i],xbitrev::jndx256[i]);
	};
};

template <typename T> struct xbitrev_for<T,512> {
	static inline void run(T arr[]) throw() {
	assert(arr);
	for(size_t i=0;i<  240;++i) Y_XBITREV_SWAP(xbitrev::indx512[i],xbitrev::jndx512[i]);
	};
};

template <typename T> struct xbitrev_for<T,1024> {
	static inline void run(T arr[]) throw() {
	assert(arr);
	for(size_t i=0;i<  496;++i) Y_XBITREV_SWAP(xbitrev::indx1024[i],xbitrev::jndx1024[i]);
	};
};

template <typename T> struct xbitrev_for<T,2048> {
	static inline void run(T arr[]) throw() {
	assert(arr);
	for(size_t i=0;i<  992;++i) Y_XBITREV_SWAP(xbitrev::indx2048[i],xbitrev::jndx2048[i]);
	};
};

template <typename T> struct xbitrev_for<T,4096> {
	static inline void run(T arr[]) throw() {
	assert(arr);
	for(size_t i=0;i< 2016;++i) Y_XBITREV_SWAP(xbitrev::indx4096[i],xbitrev::jndx4096[i]);
	};
};

template <typename T> struct xbitrev_for<T,8192> {
	static inline void run(T arr[]) throw() {
	assert(arr);
	for(size_t i=0;i< 4032;++i) Y_XBITREV_SWAP(xbitrev::indx8192[i],xbitrev::jndx8192[i]);
	};
};

