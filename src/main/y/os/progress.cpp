
#include "y/os/progress.hpp"
#include "y/type/utils.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/block/zset.hpp"
#include <cmath>
#include <cstring>
#include <cstdio>
#include <iostream>

namespace upsylon
{

    progress:: progress() :
    stopwatch(),
    mark(ticks()),
    bips(0),
    last(0),
    percent(0),
    done(0),
    left(max_seconds),
    counter(0)
    {
    }

    progress:: ~progress() throw()
    {
        mark=0;
        bips=0;
        last=0;
        _bzset(percent);
        _bzset(done);
        _bzset(left);
    }

    void progress:: start() throw()
    {
        mark             = ticks();
        bips             = 0;
        last             = 0;
        _bzset(percent);
        _bzset(done);
        aliasing::_(left) = max_seconds;
    }

    uint64_t progress:: count() const
    {
        return ticks() - mark;
    }

    void progress:: update(double ratio)
    {
        ratio                = clamp<double>(0,ratio,1);
        aliasing::_(percent) = floor( ratio * 10000.0 + 0.5 ) / 100.0;
        if(ratio<=0)
        {
            aliasing::_(done) = 0;
            aliasing::_(left) = max_seconds;
        }
        else
        {
            bips  = ticks() - mark;
            real_time_clock   &self  = *this;
            aliasing::_(done) = self(bips);
            aliasing::_(left) = (1.0-ratio)/ratio * done;
        }
    }

    void progress:: format(char *fmt, const double tmx )
    {
        assert(fmt);
        memset(fmt,0,format_size);
        if(tmx>max_seconds||tmx<0)
        {
            static const char ms[16] = "+99D00:00:00.00";
            memcpy(fmt,ms,sizeof(ms));
        }
        else
        {
            // S = D * 86400 + H * 3600 + M * 60 + S
            const unsigned S0 = long( floor(tmx) );
            unsigned       S  = S0;
            const unsigned c = clamp<unsigned>(0,unsigned( floor(100.0*(tmx-S))),99);
            const unsigned D = S/86400;
            S -= D * 86400;
            const unsigned H = S/3600;
            S -= H * 3600;
            const unsigned M = S/60;
            S -= M * 60;
            assert(D*86400+H*3600+M*60+S==S0);
            assert(D<=max_days);
            snprintf(fmt,format_size-1," %02uD%02u:%02u:%02u.%02u", D, H, M, S, c);
        }

    }

    std::ostream & progress:: display(std::ostream &os) const
    {
        // running wheel selection
        static const char sym[] = { '-', '\\', '|', '/' };

        char C = sym[(++counter)%sizeof(sym)];
        if(left<=0)
        {
            C='*';
        }

        // constant length output
        os << '[' << C << ']';
        char fmt[format_size];
        snprintf(fmt,15," %6.2f%%",percent);
        os << fmt;
        format(fmt,done);
        os << " in" << fmt << " | ETA";
        format(fmt,left);
        os << fmt;
        return os;
    }

    void progress:: print(std::ostream &os,const double ratio,const double every)
    {
        update(ratio);
        if(done-last>=every || ratio>=1.0)
        {
            display(os) << '\r';
            last=done;
        }
    }



}
