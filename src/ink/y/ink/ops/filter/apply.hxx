
private:
template <typename T,typename U,typename FUNC>
struct __apply
{
    Pixmap<T>       *_target;
    const Pixmap<U> *_source;
    FUNC            *_func;

    inline void operator()(const Area &tile, lockable &)
    {
        assert(_target);
        assert(_source);
        assert(_func);
        Pixmap<T>       &target = * _target;
        const Pixmap<U> &source = * _source;
        FUNC            &func   = * _func;
        Y_INK_AREA_LIMITS(tile);

        for(unit_t y=ymax;y>=ymin;--y)
        {
            typename       Pixmap<T>::Row &tgt = target[y];
            const typename Pixmap<U>::Row &src = source[y];
            for(unit_t x=xmax;x>=xmin;--x)
            {
                tgt[x] = func(src[x]);
            }
        }
    }
};
public:
