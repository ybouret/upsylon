
//! a -= b, sequential
template <typename T,typename U> static inline
void _sub( array<T> &a, const array<U> &b)
{
    assert(b.size()>=a.size());
    for(size_t i=a.size();i>0;--i) a[i] -= static_cast<T>(b[i]);
}

//! a -= b, parallel
template <typename T,typename U> static inline
void _sub( array<T> &a, const array<U> &b, concurrent::for_each &loop)
{
    __set<T,U> args = { &a, &b, NULL };
    loop.run( __set<T,U>::call_sub, &args );
}

