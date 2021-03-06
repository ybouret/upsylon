//! \file
#ifndef Y_TIFFPP_INCLUDED
#define Y_TIFFPP_INCLUDED 1

#include "y/string.hpp"
#include "y/memory/zblock.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace upsylon
{
    namespace graphic
    {
        //! low level TIFF functions
        class _TIFF
        {
        public:
            //! desctructor
            virtual ~ _TIFF() throw();

            //! internal raster base type
            typedef memory::dyadic           Memory;
            typedef zblock<uint32_t,Memory> _Raster;
            
            //! internal memory raster
            class Raster : public _Raster
            {
            public:
                virtual ~Raster() throw();    //!< cleanp
                explicit Raster();            //!< setup
                void startup(const size_t n); //!< ensure enough dwords

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Raster);
            };
            
        protected:
            explicit _TIFF() throw(); //!< constructor
            void     *handle;         //!< TIFF object
        private:
            Y_DISABLE_COPY_AND_ASSIGN(_TIFF);
        };

        //! input TIFF file
        class I_TIFF : public _TIFF
        {
        public:
          
            //! open the file
            explicit I_TIFF(const string &filename);
            //! destructor
            virtual ~I_TIFF() throw();

            //! test if possible to read directpry
            bool   ReadDirectory();
            
            //! return the number of contained directories
            size_t CountDirectories();
            
            //! set current directory
            void   SetDirectory(const size_t n);
            
            //! get current width
            int    GetWidth();
            
            //! get current height
            int    GetHeight();

            //! read a raster
            void ReadRBGAImage(Raster &raster);

            //! directly count the number of directories
            static size_t CountDirectoriesOf( const string &filename );

        private:            
            Y_DISABLE_COPY_AND_ASSIGN(I_TIFF);
        };

        //! output TIFF file
        class O_TIFF : public _TIFF
        {
        public:
            static const size_t samples_per_pixel = 4; //!< used RGBA by default
            virtual ~O_TIFF() throw();                 //!< destructor
            explicit O_TIFF(const string         &filename,
                            const bool            append);   //!< open the file to write



            //! function that should set compression
            void SetCompression(const char *);
            
            //! wrapper for compression type
            struct CompressionType
            {
                const char *name; //!< the name
                unsigned    ttag; //!< the tiff tag
            };

            //! table of registered compressions
            static const CompressionType NamedCompression[];


            //! write an RGBA raster
            void WriteRGBAImage(const Raster &raster,
                                const int     w,
                                const int     h,
                                const int     iDrectory);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(O_TIFF);
            
        };
    }
}

#endif
