#ifndef Y_INK_IMAGE_INCLUDED
#define Y_INK_IMAGE_INCLUDED 1

#include "y/ink/color/rgba2data.hpp"
#include "y/ink/color/data2rgba.hpp"
#include "y/ink/pixmaps.hpp"
#include "y/string.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{
    namespace ink
    {

        //! Interface for Image Input/Output
        class imageIO : public counted_object
        {
        public:
            const string name;
            inline virtual ~imageIO() throw() {}
            inline const string & key() const throw() { return name; }

            //==============================================================
            //
            // virtual interface
            //
            //==============================================================
            virtual bitmap  *load(const string          &filename,
                                  unit_t                 depth,
                                  rgba2data             &proc,
                                  const void            *options) const = 0;

            virtual void     save(const string        &filename,
                                  const bitmap        &bmp,
                                  data2rgba           &proc,
                                  const void          *options) const = 0;

        protected:
            inline explicit imageIO(const char *id) : name(id) {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(imageIO);
        };

        //! handling different imageIO
        class image : public imageIO
        {
        public:
            class format : public imageIO
            {
            public:
                typedef intr_ptr<string,format> pointer;
                virtual bool         lossless()   const throw() = 0;
                virtual const char **extensions() const throw() = 0;

                inline virtual ~format() throw() {}

            protected:
                inline explicit format(const char *id) : imageIO(id) {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(format);
            };

        private:

        };
    }
}

#endif

