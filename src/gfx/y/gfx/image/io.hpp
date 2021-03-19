//! \file

#ifndef Y_GFX_IMAGE_IO_INCLUDED
#define Y_GFX_IMAGE_IO_INCLUDED 1

#include "y/gfx/image/named-format.hpp"
#include "y/associative/hash/set.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace image
        {

            //__________________________________________________________________
            //
            //
            //! compound object to handle multiple named formats
            //
            //__________________________________________________________________
            class io :
            public singleton<io>,
            public format,
            public named_format::database
            {
            public:
                
                void  define( named_format *fmt ); //!< insert, taken care of
                io   &standard();                  //!< insert undefined standard

                virtual bool   matches(const string &ext) const;

                //! look for a given named format
                template <typename ID>
                named_format * operator()(const ID &id ) throw()
                {
                    named_format::pointer *ptr = search(id);
                    return ptr ? & (**ptr) : NULL;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(io);
                explicit io();
                virtual ~io() throw();
                Y_SINGLETON_DECL_WITH(0,io);


                virtual bitmap load_(const string       &file,
                                     const options      *opts,
                                     const rgba_to_type &conv) const;


                virtual void   save_(const bitmap       &bmp,
                                     const string       &file,
                                     const options      *opts,
                                     const type_to_rgba &conv) const;
            };

        }


    }
}

//! call singleton and load with standard formats
#define Y_IMAGE()   upsylon::graphic::image::io::instance().standard()

//! define the local IMG
#define Y_USE_IMG() upsylon::graphic::image::io &IMG = Y_IMAGE()

#endif
