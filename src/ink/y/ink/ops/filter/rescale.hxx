private:
template <typename T,typename F2T> struct __rescale
{
    Pixmap<T> *_source;
    T          _vmin;
    T          _vmax;
    F2T       *_func;
    
    inline void operator()(const Tile &zone, lockable &)
    {
        assert(_source);
        assert(_func);
        if(zone.pixels)
        {
            Pixmap<T> &source = *_source;
            F2T       &func   = *_func;

            const float vmin   = float(_vmin);
            const float vmax   = float(_vmax);
            Y_INK_AREA_LIMITS(zone);

            if(vmax>vmin)
            {
                const float scale = 1.0f/(vmax-vmin);
                for(unit_t y=ymax;y>=ymin;--y)
                {
                    typename Pixmap<T>::Row &tgt = source[y];
                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        tgt[x] = func(scale*(float(tgt[x])-vmin));
                    }
                }
            }
            else
            {
                for(unit_t y=ymax;y>=ymin;--y)
                {
                    typename Pixmap<T>::Row &tgt = source[y];
                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        tgt[x] = 0;
                    }
                }
            }
        }
    }
};
public:
