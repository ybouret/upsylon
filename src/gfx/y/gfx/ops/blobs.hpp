//! \file

#ifndef Y_GFX_OPS_BLOBS_INCLUDED
#define Y_GFX_OPS_BLOBS_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/pixel.hpp"
#include "y/core/knode.hpp"
#include "y/core/inode.hpp"

namespace upsylon
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        // types and definition
        //
        //______________________________________________________________________
        typedef core::knode<coord>    knot;   //!< dynamic coordinate
        typedef knot::list_type       knots_; //!< base class for list of knots

        
        //______________________________________________________________________
        //
        //
        //! list of knots
        //
        //______________________________________________________________________
        class knots : public entity, public knots_
        {
        public:
            virtual ~knots() throw();        //!< cleanup
            explicit knots() throw();        //!< setup
            void     reserve(size_t n);      //!< push_back n nodes
            void     ensure(const size_t n); //!< ensure n nodes
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(knots);
        };

        //______________________________________________________________________
        //
        //
        //! shared list of knots
        //
        //______________________________________________________________________
        typedef arc_ptr<knots> shared_knots;

        //______________________________________________________________________
        //
        //
        //! a blob is a list of nodes, itself a node for blobs
        //
        //______________________________________________________________________
        class blob : public knots, public inode<blob>
        {
        public:
            
            //! prototype to validate a newlyformed blob
            typedef bool (*proc)(blob &, void *);
            
            explicit blob(const size_t, const shared_knots &) throw(); //!< setup
            virtual ~blob() throw();                                   //!< cleanup

            const size_t label; //!< identifier
            shared_knots cache; //!< pool to return knots to
            
            //! change label, propagating
            blob  *change_label(const size_t    value,
                                pixmap<size_t> &masks) throw();
            
            //! change label to 0
            blob  *remove_from(pixmap<size_t> &masks) throw();
            
            //! set value on knots
            template <typename T> inline
            void dispatch(typename type_traits<T>::parameter_type args, pixmap<T> &pxm ) const
            {
                for(const knot *node=head;node;node=node->next)
                {
                    pxm(**node) = args;
                }
            }
            
            //! compare blobs by increasing size
            static int increasing_size(const blob *, const blob *, void*) throw();
            
            //! comapre blobs by decreasing size
            static int decreasing_size(const blob *, const blob *, void*) throw();
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(blob);
        };
        
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
            
            void     sort_decreasing(); //!< sort blobs bigs first
            void     rewrite(pixmap<size_t> &masks) throw(); //!< rewrite labels
            
            
            //! generic build algorithm
            template <typename T> inline
            void build(pixmap<size_t>  &masks,
                       const pixmap<T> &field,
                       shared_knots    &cache,
                       const size_t     n,
                       blob::proc       proc = NULL,
                       void            *args = NULL)
            {
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                assert(masks.has_same_metrics_than(field));
                assert(n==4||n==8);
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
                                for(size_t i=0;i<n;++i)
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
            void relabel() throw();
        };
        
 
    }


}

#endif

