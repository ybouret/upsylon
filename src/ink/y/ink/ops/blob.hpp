//! \file
#ifndef Y_INK_BLOB_INCLUDED
#define Y_INK_BLOB_INCLUDED 1

#include "y/ink/pixmaps.hpp"
#include "y/ink/ops/particles.hpp"

namespace upsylon
{
    namespace Ink
    {

        //! Probing level
        enum Connectivity
        {
            Connect4, //!< four  neighbors
            Connect8  //!< eight neighbors
        };

        typedef Pixmap<size_t>   __Blobs; //!< blob base type
        typedef Particle<size_t> Blob;    //!< Particle with an index tag

        //! Build blobs map and particle
        class Blobs : public __Blobs
        {
        public:

            virtual ~Blobs() throw(); //!< destructor
            explicit Blobs(const size_t W, size_t H);  //!< constructor


            size_t       count; //!< current number of blobs
            Vertex::Pool vpool; //!< pool of reusable vertices

            //! build the blobs
            template <typename T>
            void build(Blob::List         &particles,
                       const Pixmap<T>    &pxm,
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
                particles.clear();
                count = 0;
                this->ld(0);

                //______________________________________________________________
                //
                // loop
                //______________________________________________________________
                Y_INK_AREA_LIMITS(*this);
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
                        auto_ptr<Blob>   particle = new Blob(count);
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
                            particle->push_back( vpool(pos) );
                            // and probe neighbors
                            for(size_t k=0;k<neighbors;++k)
                            {
                                const coord q = pos + Crux::Delta[k];
                                if(!self.has(q))             continue; // out
                                if(Pixel<T>::IsZero(pxm[q])) continue; // zero
                                if(self[q]>0)                continue; // already in blob
                                vstack.store( vpool(q) );
                            }
                        }
                        particles.push_back( particle.yield() );
                    }
                }

                //______________________________________________________________
                //
                // rewrite by decreasing particle size
                //______________________________________________________________
                particles.sort();
                rewrite(particles,false);
            }

            //! tranform particles into blob map, with optional reset of map
            inline void rewrite( const Blob::List &particles, const bool reset=true ) throw()
            {
                Blobs &self = *this;
                if(reset) { self.ld(0); }
                size_t i=0;
                for(Blob *p=particles.head;p;p=p->next)
                {
                    (size_t&)(p->tag) = ++i;
                    for(const Vertex *vtx=p->head;vtx;vtx=vtx->next)
                    {
                        self[vtx->position] = i;
                    }
                }
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blobs);
        };

    };
}

#endif

