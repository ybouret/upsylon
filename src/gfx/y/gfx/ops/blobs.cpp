
#include "y/gfx/ops/blobs.hpp"

namespace upsylon
{

    namespace graphic
    {

        bnode:: bnode() throw() : object(), inode<bnode>(), pos()
        {
        }

        bnode:: ~bnode() throw()
        {

        }

    }

}

namespace upsylon
{

    namespace graphic
    {
        blist:: blist() throw() : core::list_of_cpp<bnode>()
        {
        }

        blist:: ~blist() throw()
        {


        }


    }

}

namespace upsylon
{

    namespace graphic
    {

        blob:: ~blob() throw()
        {
        }

        blob:: blob(const size_t l) throw() :
        object(),
        blist(),
        inode<blob>(),
        label(l)
        {
        }
    }

}


namespace upsylon
{

    namespace graphic
    {

        blobs:: ~blobs() throw()
        {
        }

        blobs:: blobs(const unit_t W, const unit_t H) :
        pixmap<size_t>(W,H),
        blobs_list()
        {
        }

        size_t blobs:: max_in(const area &a, const mode m) throw()
        {
            size_t res = 0;
            switch(m)
            {
                case connect8:
                {
                    for(unit_t j=0;j<a.h; j+= 2)
                    {
                        for(unit_t i=0;i<a.w;i+=2)
                        {
                            ++res;
                        }
                    }
                } break;

                case connect4:
                {
                    for(unit_t j=0;j<a.h; ++j)
                    {
                        for(unit_t i=(j&1);i<a.w;i+=2)
                        {
                            ++res;
                        }
                    }
                }   break;

            }
            return res;
        }

    }

}
