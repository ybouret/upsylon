
#include "y/ios/irstream.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace ios
    {

        irstream:: ~irstream() throw() {}

#define Y_IRSTREAM(L)          \
cache(-1),                     \
count(0),                      \
length(L)

        irstream:: irstream(const string &filename,const offset_t shift, const offset_t len) :
        local_file(filename,readable),
        Y_IRSTREAM(len)
        {
            (void)seek(shift,from_set);
        }


        irstream:: irstream( const ios::cstdin_t &_) :
        local_file(_),
        Y_IRSTREAM(-1)
        {

        }

        irstream:: irstream(const char *filename,const offset_t shift, const offset_t len) :
        local_file(filename,readable),
        Y_IRSTREAM(len)
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
                if(length>=0&&count>=length)
                {
                    return false;
                }
                else
                {
                    size_t done = 0;
                    descriptor::get(handle, &C, 1, done);
                    count += done;
                    return 1==done;
                }
            }
        }

        void irstream:: store(char C)
        {
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
