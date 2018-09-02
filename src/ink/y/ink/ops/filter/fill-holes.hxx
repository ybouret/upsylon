
private:
template <typename T> struct __fill_holes
{
    Pixmap<T> * _source;
    inline void operator()( const Area &a, lockable &)
    {
        assert(_source);
        Pixmap<T> &source = *_source;
        Y_INK_AREA_LIMITS(a);

        T pix[4] = { Pixel<T>::Zero, Pixel<T>::Zero, Pixel<T>::Zero, Pixel<T>::Zero};
        for(unit_t y=ymax;y>=ymin;--y)
        {
            typename Pixmap<T>::Row &src = source[y];
            for(unit_t x=xmax;x>=xmin;--x)
            {
                T &C = src[x];
                if( !Pixel<T>::IsZero(C) ) continue;

                const  coord p(x,y);
                bool   is_hole = true;
                size_t count   = 0;
                for(size_t k=0;k<4;++k)
                {
                    const coord q = p+Crux::Delta[k];
                    if(!source.has(q)) continue;
                    if( Pixel<T>::IsZero( (pix[count] = source[q] ) ) )
                    {
                        is_hole = false;
                        break;
                    }
                    ++count;
                }
                if(is_hole)
                {
                    assert(count>0);
                    C = Pixel<T>::Average(pix,count);
                }
            }
        }
    }
};

template <typename T> struct __fill_holes2
{
    Pixmap<T>       * _target;
    const Pixmap<T> * _source;

    inline void operator()( const Area &a, lockable &)
    {
        assert(_target);
        assert(_source);
        Pixmap<T>       &target = *_target;
        const Pixmap<T> &source = *_source;
        Y_INK_AREA_LIMITS(a);
        for(unit_t y=ymax;y>=ymin;--y)
        {
            typename Pixmap<T>::Row       &tgt = target[y];
            const typename Pixmap<T>::Row &src = source[y];
            for(unit_t x=xmax;x>=xmin;--x)
            {
                T &C = tgt[x];
                if( !Pixel<T>::IsZero(C) ) continue;
                const  coord p(x,y);
                bool   is_hole = true;
                for(size_t k=0;k<4;++k)
                {
                    const coord q = p+Crux::Delta[k];
                    if(!target.has(q)) continue;
                    if( Pixel<T>::IsZero( target[q] ) )
                    {
                        is_hole = false;
                        break;
                    }
                }
                if(is_hole)
                {
                    C = src[x];
                }
            }
        }


    }

};

public:
