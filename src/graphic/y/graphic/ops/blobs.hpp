
//! \file

#ifndef Y_GRAPHIC_OPS_BLOBS_INCLUDED
#define Y_GRAPHIC_OPS_BLOBS_INCLUDED 1

#include "y/graphic/pixmaps.hpp"
#include "y/graphic/pixel.hpp"
#include "y/graphic/linked.hpp"


namespace upsylon {

    namespace Graphic {

        
        //! a blob is a list of points with a unique label
        class Blob : public Object, public PList, public core::inode<Blob>
        {
        public:
            typedef core::list_of_cpp<Blob> List; //!< alias

            explicit Blob( const size_t blobLabel ) throw(); //!< setup
            virtual ~Blob() throw();                         //!< cleanup

            const size_t label; //!< label or index of this blob

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blob);
        };


        //! manage Blobs building algorithm
        class Blobs : public Indices, public Blob::List
        {
        public:

            //! how to explore current point
            enum Connectivity
            {
                Connect4, //!< first four neighbors
                Connect8  //!< eight four neighbors
            };

            explicit Blobs( const size_t W, const size_t  H); //!< setup
            virtual ~Blobs() throw();                         //!< cleanup

            //! build by exploring
            template <typename T> inline
            void build(const Pixmap<T>   &S,
                       const Connectivity conn)
            {
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                Blob::List  &L = *this;
                Indices     &B = *this;

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

                //--------------------------------------------------------------
                //
                // prepare current state
                //
                //--------------------------------------------------------------
                size_t label = 0; // mark blob
                PPool  stack;     // use to explore
                for(unit_t y=0;y<h;++y)
                {
                    Indices::RowType                  &b = B[y];
                    const typename Pixmap<T>::RowType &s = S[y];
                    for(unit_t x=0;x<w;++x)
                    {
                        if( (b[x]>0) || Pixel::IsZero(s[x]) )
                        {
                            //__________________________________________________
                            //
                            // discontinuity or visited
                            //__________________________________________________
                            continue;
                        }
                        else
                        {
                            //__________________________________________________
                            //
                            // start a new blob!
                            //__________________________________________________
                            Blob *blob = L.push_back( new Blob(++label) );
                            stack.store( new PNode( Point(x,y) ) );

                            //__________________________________________________
                            //
                            // grow blob
                            //__________________________________________________
                            while( stack.size )
                            {
                                const Point init = **(stack.top);
                                size_t     &mark = B[init];
                                if( mark <= 0 )
                                {
                                    //__________________________________________
                                    //
                                    // not visited
                                    //__________________________________________
                                    if( Pixel::IsZero(S[init]) )
                                    {
                                        // discontinuity
                                        stack.pop();
                                        continue;
                                    }
                                    else
                                    {
                                        // keep and explore
                                        blob->push_back( stack.query() );
                                        mark = label;
                                        for(size_t d=0;d<D;++d)
                                        {
                                            const Point probe = init + Pixel::delta[d];
                                            if( B->contains(probe)
                                               && (B[probe]<=0)
                                               && !Pixel::IsZero( S[probe] ) )
                                            {
                                                stack.store( new PNode(probe) );
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    //__________________________________________
                                    //
                                    // visited
                                    //__________________________________________
                                    assert( label == mark );
                                    stack.pop();
                                    continue;
                                }
                            }

                        }
                    }
                }
            }

            //! sort by decreasing size and rewrite indices
            void rewrite();

            //! keep above a minimial size
            void keepAbove( const size_t minSize ) throw();

            //! build, rewrite, and remove below minSize
            template <typename T> inline
            void build(const Pixmap<T>   &data,
                       const Connectivity conn,
                       const size_t       minSize)
            {
                build<T>(data,conn);
                rewrite();
                keepAbove(minSize);
            }

            //! transfer blob location
            template <typename T> inline
            void transfer(Pixmap<T>       &target,
                          const Pixmap<T> &source,
                          const Blob      *blob) const throw()
            {
                assert(blob);
                for(const PNode *node = blob->head; node; node=node->next )
                {
                    const Point p = **node;
                    target[p] = source[p];
                }
            }

            //! transfer all blobs
            template <typename T> inline
            void transfer(Pixmap<T>       &target,
                          const Pixmap<T> &source)
            {
                for(const Blob *blob = this->head; blob; blob=blob->next )
                {
                    transfer(target,source,blob);
                }
            }

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blobs);
        };

    }
}

#endif



