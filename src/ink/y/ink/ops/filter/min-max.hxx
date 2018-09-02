private:
template <typename T> struct __find_min
{
    const Pixmap<T> *_source;

    inline void operator()( const Tile &zone, lockable &)
    {
        assert(_source);
        if(zone.pixels)
        {
            const Pixmap<T> &source = * _source;
            Y_INK_AREA_LIMITS(zone);


            T vmin = source[ymin][xmin];
            for(unit_t y=ymax;y>=ymin;--y)
            {
                const typename Pixmap<T>::Row &tgt = source[y];
                for(unit_t x=xmax;x>=xmin;--x)
                {
                    const T v = tgt[x];
                    if(v<vmin) vmin = v;
                }
            }
            zone.cache.get<float>(0) = vmin;
        }
    }
};

template <typename T> struct __find_max
{
    const Pixmap<T> *_source;

    inline void operator()( const Tile &zone, lockable &)
    {
        assert(_source);
        if(zone.pixels)
        {
            const Pixmap<T> &source = * _source;
            Y_INK_AREA_LIMITS(zone);
            T vmax = source[ymin][xmin];
            for(unit_t y=ymax;y>=ymin;--y)
            {
                const typename Pixmap<T>::Row &tgt = source[y];
                for(unit_t x=xmax;x>=xmin;--x)
                {
                    const T v = tgt[x];
                    if(v>vmax) vmax= v;
                }
            }
            zone.cache.get<float>(0) = vmax;
        }
    }
};

template <typename T> struct __find_min_max
{
    const Pixmap<T> *_source;

    inline void operator()( const Tile &zone, lockable &)
    {
        assert(_source);
        if(zone.pixels)
        {
            const Pixmap<T> &source = * _source;
            Y_INK_AREA_LIMITS(zone);
            T vmax = source[ymin][xmin], vmin=vmax;
            for(unit_t y=ymax;y>=ymin;--y)
            {
                const typename Pixmap<T>::Row &tgt = source[y];
                for(unit_t x=xmax;x>=xmin;--x)
                {
                    const T v = tgt[x];
                    if(vmax<v)      vmax=v;
                    else if(v<vmin) vmin=v;
                }
            }
            zone.cache.get<float>(0) = vmin;
            zone.cache.get<float>(1) = vmax;
        }
    }
};

public:

