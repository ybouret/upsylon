
#include "y/graphic/stack.hpp"
#include "y/graphic/color/rgb.hpp"

namespace upsylon {

    namespace Graphic {


        template <>
        void Stack<float>:: average(Pixmap<float> &target, const size_t ini, const size_t end) const
        {
            average_<float,1>(target, ini, end);
        }

        template <>
        void Stack<uint8_t>:: average(Pixmap<uint8_t> &target, const size_t ini, const size_t end) const
        {
            average_<uint8_t,1>(target, ini, end);
        }

        template <>
        void Stack<rgb>:: average(Pixmap<rgb> &target, const size_t ini, const size_t end) const
        {
            average_<uint8_t,3>(target, ini, end);
        }

        template <>
        void Stack<rgba>:: average(Pixmap<rgba> &target, const size_t ini, const size_t end) const
        {
            average_<uint8_t,3>(target, ini, end);
        }

    }

}

#include "y/graphic/image.hpp"
#include "y/fs/vfs.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Graphic {

        void Stack_ :: CheckTIFF( const string &fileName )
        {
            if( "TIFF" != Image::instance().FormatFor(fileName).name )
            {
                throw exception("Graphic::Stack.save(invalid '%s')", vfs::get_base_name( fileName ) );
            }
        }

        size_t Stack_:: NumDirectories( const string &fileName, const size_t maxDirectories)
        {
            size_t numDirectories = I_TIFF::CountDirectoriesOf(fileName);
            if(maxDirectories>0)
            {
                numDirectories = min_of(maxDirectories,numDirectories);
            }
            return numDirectories;
        }

    }
}

