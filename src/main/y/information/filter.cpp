
#include "y/information/filter.hpp"

namespace upsylon {

    namespace information {

        filter:: filter() throw() {}

        filter:: ~filter() throw() {}

        size_t filter:: process( ios::ostream &target, ios::istream &source, size_t *nread )
        {
            size_t icount = 0;
            size_t ocount = 0;
            filter &self = *this;
            char C = 0;
            while( source.query(C) )
            {
                ++icount;
                self.write(C);
                while( self.query(C) )
                {
                    target.write(C);
                    ++ocount;
                }
            }
            self.flush();
            while( self.query(C) )
            {
                target.write(C);
                ++ocount;
            }
            target.flush();
            if(nread) *nread = icount;
            return ocount;
        }



    }

}

#include "y/ios/osstream.hpp"
#include "y/ios/imstream.hpp"

namespace upsylon {

    namespace information {

        string filter:: to_string( ios::istream &source )
        {
            string ans;
            {
                ios::osstream target(ans);
                process(target,source);
            }
            return ans;
        }

        string filter:: to_string(const char *text)
        {
            ios::imstream source(text, length_of(text) );
            return to_string(source);
        }

        string filter:: to_string(const memory::ro_buffer &buff)
        {
            ios::imstream source(buff);
            return to_string(source);
        }

    }
}

