#include "y/ios/bin2dat.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace ios
    {
        bin2dat:: ~bin2dat() throw()
        {
        }

        bin2dat:: bin2dat(const size_t w) :
        count(0),
        width( max_of<size_t>(w,1) ),
        human(width,as_capacity)
        {
        }

        void bin2dat:: reset() throw()
        {
            count = 0;
            human.clear();
        }


        void bin2dat:: write(ios::ostream &fp, char C, const bool isLast)
        {
            assert(count<width);
            fp("%3u",unsigned(uint8_t(C)));   // 3 chars
            if(!isLast) { fp << ',' << ' '; } // 2 chars

            if(C>=32&&C<127)
            {
                human << C;
            }
            else
            {
                human << '.';
            }

            ++count;
            if(count>=width||isLast)
            {
                assert(human.size()==count);
                for(size_t i=count;i<width;++i)
                {
                    fp << "     ";
                }
                if(isLast)
                {
                    fp << "  ";
                }
                fp << " // |";
                fp << human;
                for(size_t i=count;i<width;++i) fp << ' ';
                fp << "|\n";
                reset();
            }
        }

        void bin2dat:: write(ios::ostream &fp, const void *data, const size_t size)
        {
            assert(!(NULL==data&&size>0));
            const char   *buf = static_cast<const char *>(data);
            const size_t  mark = size-1;
            for(size_t i=0;i<size;++i)
            {
                write(fp,buf[i],i==mark);
            }
        }

        void bin2dat:: write(ios::ostream &fp, const memory::ro_buffer &buf )
        {
            write(fp, buf.ro(), buf.length() );
        }


    }
}
