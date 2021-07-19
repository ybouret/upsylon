//! \file

#ifndef Y_GFX_OPS_BLOBS_INCLUDED
#define Y_GFX_OPS_BLOBS_INCLUDED 1

#include "y/gfx/ops/blob.hpp"
#include "y/gfx/pixel.hpp"


namespace upsylon
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! base class for blobs
        //
        //______________________________________________________________________
        typedef core::list_of_cpp<blob> blob_list;

        class blobs
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~blobs() throw(); //!< cleanup
            explicit blobs(const unit_t W, const unit_t H, const shared_knots &K); //!< setup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void     clear() throw(); //!< clear all

            //! generic build algorithm
            template <typename T> inline
            void build(const pixmap<T> &field,
                       const size_t     connectivity,
                       blob::proc       proc = NULL,
                       void            *args = NULL)
            {
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                assert(bmask.has_same_metrics_than(field));
                assert(connectivity==4||connectivity==8);
                clear();

                size_t       label = 0;       // initial label
                const unit_t w     = bmask.w; // full width
                const unit_t h     = bmask.h; // full height
                knots        stack;           // local search stack

                for(unit_t y=0;y<h;++y)
                {
                    const pixrow<T> &f_y  = field(y);
                    pixrow<size_t>  &m_y  = aliasing::_(bmask(y));

                    for(unit_t x=0;x<w;++x)
                    {
                        //------------------------------------------------------
                        //
                        // ENTER: inner loop
                        //
                        //------------------------------------------------------

                        if( pixel::is_zero(f_y(x)) ) continue;            // no data
                        size_t &m = m_y(x); if(m>0)  continue;            // already visited
                        blob   *b = produce(label);
                        {
                            //--------------------------------------------------
                            // initialize stack
                            //--------------------------------------------------
                            assert(0==stack.size);
                            m = label;
                            **stack.push_back( fetch_knot() ) = coord(x,y);

                            while(stack.size)
                            {
                                //----------------------------------------------
                                // register front node, take its coordinates
                                //----------------------------------------------
                                const coord  curr = **(b->push_back( stack.pop_front() ));
                                for(size_t i=0;i<connectivity;++i)
                                {
                                    //------------------------------------------
                                    // check if can be added
                                    //------------------------------------------
                                    const coord link = curr + area::delta[i];     // where to look at
                                    if( !bmask.owns(link)  ) continue;            // does not belong  -> continue
                                    size_t &l = aliasing::_(bmask(link));         // check label
                                    if( l > 0 ) { assert(label==l); continue; }   // already visited  -> continue
                                    if( pixel::is_zero( field(link) ) ) continue; // a zero pixel     -> continue

                                    //------------------------------------------
                                    // added to stack
                                    //------------------------------------------
                                    l = label;
                                    **stack.push_back( fetch_knot() )  = link;
                                }
                            }

                        }
                        //------------------------------------------------------
                        // a new blob was created!
                        //------------------------------------------------------
                        if( proc && ! (*proc)(*b,args) )
                        {
                            // clean zone
                            expunge(b);
                            --label;
                        }

                        //------------------------------------------------------
                        //
                        // LEAVE: inner loop
                        //
                        //------------------------------------------------------
                    }

                }
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const blob_list blist; //!< current list
            const blob_map  bmask; //!< current mask
            shared_knots    cache; //!< shared cache for knots I/O

        private:
            Y_DISABLE_ASSIGN(blobs);
            knot *fetch_knot();            //!< a new (cached) knot
            void  rewrite() throw();       //!< dispatch each label
            void  relabel() throw();       //!< label from 1 to blist.size, rewrite
            void  expunge(blob *) throw(); //!< expunge blob from list and mask, delete it
            blob *produce(size_t&);        //!< produce new blob with update label
        };


#if 0
        //______________________________________________________________________
        //
        //
        //! base class for blobs
        //
        //______________________________________________________________________
        typedef core::list_of_cpp<blob> blobs_;
        
        //______________________________________________________________________
        //
        //
        //! blobs for a pixmap
        //
        //______________________________________________________________________
        class blobs : public blobs_
        {
        public:
            explicit blobs() throw(); //!< setup empty
            virtual ~blobs() throw(); //!< cleanup
            
            void     sort(pixmap<size_t> &masks);                                  //!< sort by decreasing order, relabel and rewrite
            void     remove_if(blob::proc, void *, pixmap<size_t> &)      throw(); //!< remove if, relabel and rewrite
            void     remove_below(const size_t cutSize, pixmap<size_t> &) throw(); //!< remove if blob.size <= cutSize, rlabel and rewrite

            //! generic build algorithm
            template <typename T> inline
            void build(pixmap<size_t>  &masks,
                       const pixmap<T> &field,
                       shared_knots    &cache,
                       const size_t     connectivity,
                       blob::proc       proc = NULL,
                       void            *args = NULL)
            {
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                assert(masks.has_same_metrics_than(field));
                assert(connectivity==4||connectivity==8);
                masks.ldz();
                release();
                
                size_t       label = 0;       // initial label
                const unit_t w     = masks.w; // full width
                const unit_t h     = masks.h; // full height
                knots        stack;           // local search stack
                
               
                
                for(unit_t y=0;y<h;++y)
                {
                    const pixrow<T> &f_y  = field(y);
                    pixrow<size_t>  &m_y  = masks(y);
                    
                    for(unit_t x=0;x<w;++x)
                    {
                        //------------------------------------------------------
                        //
                        // ENTER: inner loop
                        //
                        //------------------------------------------------------
                        
                        if( pixel::is_zero(f_y(x)) ) continue;            // no data
                        size_t &m = m_y(x); if(m>0)  continue;            // already visited
                        blob   *b = push_back( new blob(++label,cache) ); // start new blob!
                        {
                            //--------------------------------------------------
                            // initialize stack
                            //--------------------------------------------------
                            assert(0==stack.size);
                            m = label;
                            **stack.push_back( fetch_knot(cache) )  = coord(x,y);
                            
                            while(stack.size)
                            {
                                //----------------------------------------------
                                // register front node, take its coordinates
                                //----------------------------------------------
                                const coord  curr = **(b->push_back( stack.pop_front() ));
                                for(size_t i=0;i<connectivity;++i)
                                {
                                    //------------------------------------------
                                    // check if can be added
                                    //------------------------------------------
                                    const coord link = curr + area::delta[i];     // where to look at
                                    if( !masks.owns(link)  ) continue;            // does not belong  -> continue
                                    size_t &l = masks(link);                      // check label
                                    if( l > 0 ) { assert(label==l); continue; }   // already visited  -> continue
                                    if( pixel::is_zero( field(link) ) ) continue; // a zero pixel     -> continue
                                   
                                    //------------------------------------------
                                    // added to stack
                                    //------------------------------------------
                                    l = label;
                                    **stack.push_back( fetch_knot(cache) )  = link;
                                }
                            }
                            
                        }
                        //------------------------------------------------------
                        // a new blob was created!
                        //------------------------------------------------------
                        if( proc && ! (*proc)(*b,args) )
                        {
                            // clean zone
                            delete unlink(b->remove_from(masks));
                            --label;
                        }
                        
                        //------------------------------------------------------
                        //
                        // LEAVE: inner loop
                        //
                        //------------------------------------------------------
                    }
                    
                }
                
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(blobs);
            static knot *fetch_knot(shared_knots &cache);
            void         relabel() throw();
            void         rewrite(pixmap<size_t> &) const throw();
        };
#endif
        
 
    }


}

#endif

