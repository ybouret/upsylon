
//! \file

#ifndef Y_GRAPHIC_OPS_BLOBS_INCLUDED
#define Y_GRAPHIC_OPS_BLOBS_INCLUDED 1

#include "y/graphic/pixmaps.hpp"
#include "y/graphic/pixel.hpp"
#include "y/core/node.hpp"
#include "y/core/list.hpp"
#include "y/core/pool.hpp"

namespace upsylon {

    namespace Graphic {

        typedef core::node_of<const Point>     PNode;
        typedef core::pool_of_cpp<PNode>       PPool;
        typedef core::list_of_cpp<PNode>       PList;

        class Blob : public Object, public PList, public core::inode<Blob>
        {
        public:
            typedef core::list_of_cpp<Blob> List;

            explicit Blob( const size_t blobLabel ) throw() : label(blobLabel) {}
            virtual ~Blob() throw() {}

            const size_t label;



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blob);
        };


        class Blobs : public Pixmap<size_t>, public Blob::List
        {
        public:
            static const Point delta[8];

            enum Connectivity
            {
                Connect4,
                Connect8
            };

            explicit Blobs( const size_t W, const size_t  H) :
            Pixmap<size_t>(W,H),
            Blob::List()
            {
            }

            virtual ~Blobs() throw() {}

            template <typename T>
            void build(const Pixmap<T> &S, const Connectivity conn)
            {
                Blob::List     &L = *this;
                Pixmap<size_t> &B = *this;

                L.release();
                B->clear();
                const unit_t h = B->h;
                const unit_t w = B->w;
                size_t       D = 0;
                switch(conn)
                {
                    case Connect4: D=4; break;
                    case Connect8: D=8; break;
                }

                size_t label = 0;
                for(unit_t y=0;y<h;++y)
                {
                    Pixmap<size_t>::RowType           &b = B[y];
                    const typename Pixmap<T>::RowType &s = S[y];
                    for(unit_t x=0;x<w;++x)
                    {
                        if(Pixel::IsZero(s[x]) || (b[x]>0) )
                        {
                            continue; // no pixel or already marked
                        }
                        else
                        {
                            // initialize blob
                            ++label;
                            Blob *blob = L.push_back( new Blob(label) );

                            // initialize stack
                            PPool stack;
                            stack.store( new PNode( Point(x,y) ) );

                            // probe from scan
                            while( stack.size > 0 )
                            {
                                const Point p = **(stack.top);
                                assert( B(p) <= 0 || label==B(p) );
                                assert( !Pixel::IsZero(S(p)) );
                                blob->push_back( stack.query() );
                                B(p) = label;
                                for(size_t d=0;d<D;++d)
                                {
                                    const Point probe = p + delta[d];
                                    if(   B->contains(probe)
                                       && (B(probe)<=0)
                                       && !Pixel::IsZero(S(probe)) )
                                    {
                                        stack.store( new PNode(probe) );
                                    }
                                }
                            }
                        }
                    }
                }
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blobs);
        };

    }
}

#endif



