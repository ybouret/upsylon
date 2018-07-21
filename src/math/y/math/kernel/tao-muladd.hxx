

template <typename T,typename U> static inline
void _muladd( array<T> &a, typename type_traits<T>::parameter_type x, const array<U> &b)
{
    assert(b.size()>=a.size());
    for(size_t i=a.size();i>0;--i) a[i] += x*static_cast<T>(b[i]);
}

template <typename T,typename U> static inline
void _muladd( array<T> &a, typename type_traits<T>::parameter_type x, const array<U> &b, concurrent::for_each &loop)
{
    __set<T,U> args = { &a, &b, &x };
    loop.run( __set<T,U>::call_muladd, &args );
}



template <typename T,typename U> static inline
void _mulsub( array<T> &a, typename type_traits<T>::parameter_type x, const array<U> &b)
{
    assert(b.size()>=a.size());
    for(size_t i=a.size();i>0;--i) a[i] -= x*static_cast<T>(b[i]);
}

template <typename T,typename U> static inline
void _mulsub( array<T> &a, typename type_traits<T>::parameter_type x, const array<U> &b, concurrent::for_each &loop)
{
    __set<T,U> args = { &a, &b, &x };
    loop.run( __set<T,U>::call_mulsub, &args );
}

template <typename T,typename U> static inline
void _mulset( array<T> &a, typename type_traits<T>::parameter_type x, const array<U> &b)
{
    assert(b.size()>=a.size());
    for(size_t i=a.size();i>0;--i) a[i] = x*static_cast<T>(b[i]);
}

template <typename T,typename U> static inline
void _mulset( array<T> &a, typename type_traits<T>::parameter_type x, const array<U> &b, concurrent::for_each &loop)
{
    __set<T,U> args = { &a, &b, &x };
    loop.run( __set<T,U>::call_mulset, &args );
}

