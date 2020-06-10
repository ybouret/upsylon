//! \file
#ifndef Y_SPADE_VTK_FORMAT_INCLUDED
#define Y_SPADE_VTK_FORMAT_INCLUDED 1

#include "y/spade/types.hpp"
#include "y/string.hpp"
#include "y/associative/suffix-tree.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {

    namespace Spade {

        namespace VTK {

            class Format : public Object
            {
            public:
                typedef arc_ptr<Format> Handle;
                virtual ~Format() throw();

                const unsigned components;
                virtual void   write( ios::ostream &, const void * ) const = 0;

            protected:
                explicit Format(const unsigned) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Format);
            };

            class PrimaryFormat : public Format
            {
            public:
                explicit PrimaryFormat(const string &);
                virtual ~PrimaryFormat() throw();

                const string format;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(PrimaryFormat);
            };

            class Formats : public singleton<Formats>
            {
            public:

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Formats);
            };

        }

    }

}

#endif
