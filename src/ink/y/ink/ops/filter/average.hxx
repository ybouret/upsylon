private:
template <typename T>
struct __collect
{
    Pixmap<T>       * _target;
    const Pixmap<T> * _source;
    T              (*proc)(T *arr, const size_t num);
    inline void operator()( const Area &a, lockable &)
    {
        assert(_target);
        assert(_source);
        Pixmap<T>       &target = *_target;
        const Pixmap<T> &source = *_source;
        assert(proc);

        Y_INK_AREA_LIMITS(a);
        T pix[9] =
        {
            Pixel<T>::Zero, Pixel<T>::Zero, Pixel<T>::Zero,
            Pixel<T>::Zero, Pixel<T>::Zero, Pixel<T>::Zero,
            Pixel<T>::Zero, Pixel<T>::Zero, Pixel<T>::Zero
        };
        for(unit_t y=ymax;y>=ymin;--y)
        {
            typename Pixmap<T>::Row       &tgt = target[y];
            const typename Pixmap<T>::Row &src = source[y];
            for(unit_t x=xmax;x>=xmin;--x)
            {
                size_t count = 0;
                pix[count++] = src[x];
                const coord p(x,y);
                for(size_t k=0;k<8;++k)
                {
                    const coord q = p + Crux::Delta[k];
                    if(source.has(q))
                    {
                        pix[count++] = source[q];
                    }
                }
                tgt[x] = proc(pix,count);
            }
        }
    }

};

public:
