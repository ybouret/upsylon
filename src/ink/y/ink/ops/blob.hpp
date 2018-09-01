//! \file
#ifndef Y_INK_BLOB_INCLUDED
#define Y_INK_BLOB_INCLUDED 1

#include "y/ink/pixmaps.hpp"
#include "y/ink/vertex.hpp"

namespace upsylon
{
    namespace Ink
    {

        enum Connectivity
        {
            Connect4,
            Connect8
        };

        typedef Pixmap<size_t> __Blobs; //!< blob base type

        class Blobs : public __Blobs
        {
        public:
            virtual ~Blobs() throw(); //!< destructor

            explicit Blobs(const size_t W, size_t H);

            size_t count; //!< current number of blobs

            //! build the blobs
            template <typename T>
            void build(const Pixmap<T>    &pxm,
                       const Connectivity  cnx)
            {
                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________

                Blobs &self = *this;
                size_t neighbors=0;
                switch(cnx)
                {
                    case Connect4: neighbors=4; break;
                    case Connect8: neighbors=8; break;
                }
                assert(neighbors>0);
                count = 0;
                this->ld(0);

                //______________________________________________________________
                //
                // loop
                //______________________________________________________________
                Y_INK_AREA_LIMITS(*this);
                size_t max_stack = 0;
                for(unit_t y=ymax;y>=ymin;--y)
                {
                    const typename Pixmap<T>::Row &P = pxm[y];
                    __Blobs::Row                  &B = self[y];
                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        //______________________________________________________
                        //
                        // skip if pixel is zero
                        //______________________________________________________
                        if(Pixel<T>::IsZero(P[x]))
                        {
                            assert(0==B[x]);
                            continue;
                        }

                        //______________________________________________________
                        //
                        // skip if other blob...
                        //______________________________________________________
                        if(B[x]>0)
                        {
                            continue;
                        }

                        //______________________________________________________
                        //
                        // start a new blob!
                        //______________________________________________________
                        ++count;
                        Vertex::PoolType vstack;
                        vstack.store( vpool( coord(x,y) ) );
                        while( vstack.size > 0 )
                        {
                            // get where we are
                            const coord   pos  = vstack.top->position;
                            vpool.store( vstack.query() );
                            if(count==self[pos]) continue; //!< already tagged


                            assert(0==self[pos]);
                            assert(!Pixel<T>::IsZero(pxm[pos]));

                            // register this pixel
                            self[pos] = count;

                            // and probe neighbors
                            for(size_t k=0;k<neighbors;++k)
                            {
                                const coord q = pos + Crux::Delta[k];
                                if(!self.has(q))             continue; // out
                                if(Pixel<T>::IsZero(pxm[q])) continue; // zero
                                if(self[q]>0)                continue; // already in blob
                                vstack.store( vpool(q) );
                            }
                            max_stack = max_of(max_stack,vstack.size);
                        }
                        
                    }
                }
                std::cerr << "blobs.count=" << count << std::endl;
                std::cerr << "vpool.size =" << vpool.size << std::endl;
                std::cerr << "max_stack  =" << max_stack << std::endl;
            }


            Vertex::Pool vpool;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blobs);
        };

    };
}

#endif

