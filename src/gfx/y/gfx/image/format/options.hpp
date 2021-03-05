//! \file

#ifndef Y_GFX_IMAGE_FORMAT_OPTIONS_INCLUDED
#define Y_GFX_IMAGE_FORMAT_OPTIONS_INCLUDED 1

#include "y/gfx/image/format/option.hpp"
#include "y/associative/hash/set.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace image
        {

            typedef hash_set<string,option::pointer> options_;

            //! database of options
            class options : public options_
            {
            public:
                explicit options();
                
                template <typename DATA> inline
                explicit options( const DATA &data) : options_()
                {
                    parse(data);
                }

                virtual ~options() throw();

                void parse(const string &opts);
                void parse(const char   *opts);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(options);
                void add(const string &field);
            };

        }

    }

}

#endif

