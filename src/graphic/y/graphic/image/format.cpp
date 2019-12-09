#include "y/graphic/image.hpp"

namespace upsylon {

    namespace Graphic {


        Image::Format:: ~Format() throw()
        {
        }

        Image:: Format:: Format(const char *id, const char **ext) :
        name(id),
        extensions(2,as_capacity)
        {
            //std::cerr << "[" << name << "]" << std::endl;
            if(ext)
            {
                while(*ext)
                {
                    const string str=*(ext++);
                    extensions.insert(str);
                }
            }
            //std::cerr << "extensions: " << extensions << std::endl;
        }

        const string & Image::Format:: key() const throw()
        {
            return name;
        }


    }

}
