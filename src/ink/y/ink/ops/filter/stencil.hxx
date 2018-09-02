private:
template <typename T, typename U> struct __stencil
{
    Pixmap<float>   *_target;
    const Pixmap<T> *_source;
    const matrix<U> *_block;

    inline void operator()(const Tile &zone, lockable &)
    {
        assert(_target);assert(_source);assert(_block);
        if(zone.pixels)
        {
            Pixmap<float>   &target = * _target;
            const Pixmap<T> &source = * _source;
            const matrix<U> &block  = * _block;

            const unit_t mx = unit_t(block.cols);
            const unit_t my = unit_t(block.rows);
            const unit_t dx = (mx-1)/2;
            const unit_t dy = (my-1)/2;
            Y_INK_AREA_LIMITS(zone);
            for(unit_t y=ymax;y>=ymin;--y)
            {
                Pixmap<float>::Row            &tgt = target[y];
                const unit_t local_y=y-dy;
                for(unit_t x=xmax;x>=xmin;--x)
                {
                    float ans = 0;
                    const unit_t local_x=x-dx;
                    for(unit_t iy=1,ly=local_y;iy<=my;++iy,++ly)
                    {
                        const unit_t                   zfy   = source.zfy(ly);
                        const typename Pixmap<T>::Row &src_y = source[ zfy ];
                        const array<U>                &blk_y = block[iy];
                        for(unit_t ix=1,lx=local_x;ix<=mx;++ix,++lx)
                        {
                            const unit_t zfx = source.zfx(lx);
                            const float  b   = float(blk_y[ix]);
                            const float  s   = float(src_y[zfx]);
                            ans += b*s;
                        }
                    }
                    tgt[x] = ans;
                }
            }
        }
    }
};
public:
