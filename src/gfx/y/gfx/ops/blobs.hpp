//! \file

#ifndef Y_GFX_OPS_BLOBS_INCLUDED
#define Y_GFX_OPS_BLOBS_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/pixel.hpp"
#include "y/core/inode.hpp"

namespace upsylon
{
    namespace graphic
    {

        class bnode : public object, public inode<bnode>
        {
        public:
            explicit bnode() throw();
            virtual ~bnode() throw();

            coord    pos;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(bnode);
        };

        class blist : public core::list_of_cpp<bnode>
        {
        public:
            explicit blist() throw();
            virtual ~blist() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(blist);
        };

        class blob : public object, public blist, public inode<blob>
        {
        public:

            size_t label;
            explicit blob(size_t l=0) throw();
            virtual ~blob() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(blob);
        };

        typedef core::list_of_cpp<blob> blobs_list;

        class blobs : public pixmap<size_t>, public blobs_list
        {
        public:
            enum mode
            {
                connect4,
                connect8
            };

            explicit blobs(const unit_t W, const unit_t H);
            virtual ~blobs() throw();

            static size_t max_in(const area &, const mode) throw();



        private:
            Y_DISABLE_COPY_AND_ASSIGN(blobs);
        };



    }


}

#endif

