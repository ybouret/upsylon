
#include "y/ios/irstream.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace ios
    {

        irstream:: ~irstream() throw() {}

#define Y_IRSTREAM()           \
cache(-1),                     \
iobuf(),                       \
curr(iobuf.data),              \
last(curr)

        irstream:: irstream(const string &filename,const offset_t shift) :
        local_file(filename,readable),
        Y_IRSTREAM()
        {
            (void)seek(shift,from_set);
        }


        irstream:: irstream( const ios::cstdin_t &_) :
        local_file(_),
        Y_IRSTREAM()
        {

        }

        irstream:: irstream(const char *filename,const offset_t shift) :
        local_file(filename,readable),
        Y_IRSTREAM()
        {
            (void)seek(shift,from_set);
        }

        bool irstream:: query(char &C)
        {
            if(cache>=0)
            {
                //std::cerr << "-- cached" << std::endl;
                assert(cache<=0xff);
                C     =  char(uint8_t(cache));
                cache = -1;
                return true;
            }
            else
            {
                if(curr>=last)
                {
                    // try to reload
                    //std::cerr << "-- reloading" << std::endl;
                    last=curr=iobuf.data;
                    size_t done=0;
                    descriptor::get( handle, iobuf.data, iobuf.size, done);
                    //std::cerr << "-- reloaded " << done << std::endl;
                    if(!done)
                    {
                        return false;
                    }
                    last = curr+done;
                }
                assert(curr<last);
                C = *(curr++);
                return true;
            }
        }

        void irstream:: store(char C)
        {
            //seek(-1,from_cur);
            if(cache>0)
            {
                throw exception("unsupported multiple irstream.store");
            }
            else
            {
                cache = uint8_t(C);
            }
        }



    }
}
