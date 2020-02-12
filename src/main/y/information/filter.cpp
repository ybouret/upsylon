
#include "y/information/filter.hpp"

namespace upsylon {

    namespace information {

        filter:: filter() throw() {}

        filter:: ~filter() throw() {}

        void filter:: process( ios::ostream &target, ios::istream &source )
        {
            filter &self = *this;
            char C = 0;
            while( source.query(C) )
            {
                self.write(C);
                while( self.query(C) )
                {
                    target.write(C);
                }
            }
            self.flush();
            while( self.query(C) )
            {
                target.write(C);
            }
            target.flush();
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

