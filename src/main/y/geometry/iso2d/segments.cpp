#include "y/geometry/iso2d/segments.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso2D {

            Segment:: Segment(const Point &h, const Point &t) throw() :
            head(h),
            tail(t),
            next(0),
            prev(0)
            {
                assert(head!=tail);
            }

            Segment::  Segment(const Segment &_ ) throw() :
            head(_.head),
            tail(_.tail),
            next(0),
            prev(0)
            {
                assert(head!=tail);
            }

            Segment:: ~Segment() throw()
            {

            }

            bool Segment:: AreTheSame(const Segment &lhs, const Segment &rhs) throw()
            {
                return (lhs.head==rhs.head) && (lhs.tail==rhs.tail);
            }

            bool Segment:: AreOpposite(const Segment &lhs, const Segment &rhs) throw()
            {
                return (lhs.head==rhs.tail) && (lhs.tail==rhs.head);
            }


            bool operator==( const  Segment &lhs, const  Segment &rhs) throw()
            {
                return Segment::AreTheSame(lhs,rhs) ||  Segment::AreOpposite(lhs,rhs);
            }

            bool operator!=( const  Segment &lhs, const Segment &rhs) throw()
            {
                return !Segment::AreTheSame(lhs,rhs) &&  !Segment::AreOpposite(lhs,rhs);
            }


        }
    }
}


namespace upsylon {

    namespace geometry {

        namespace Iso2D {


            Segments:: Segments() throw() : Segment::List()
            {
            }

            Segments:: ~Segments() throw()
            {
            }

            Segments:: Segments(const Segments &other) : Segment::List(other)
            {
            }

        }
    }
}

namespace upsylon {

    namespace geometry {

        namespace Iso2D {

            bool Segments::  grow( Lines &iso, const Segment *s )
            {
                assert(s);
                const Point &s_head = s->head;
                const Point &s_tail = s->tail;
                for(size_t i=iso.size();i>0;--i)
                {
                    Line_       &C      = *iso[i]; assert(C.size>=2);
                    const Point &c_head = *(C.head);
                    const Point &c_tail = *(C.tail);

                    if(c_head==s_head)
                    {
                        C.push_front( new Node(s_tail) );
                        return true;
                    }

                    if(c_head==s_tail)
                    {
                        C.push_front( new Node(s_head) );
                        return true;
                    }

                    if(c_tail==s_head)
                    {
                        C.push_back( new Node(s_tail) );
                        return true;
                    }

                    if(c_tail==s_tail)
                    {
                        C.push_back( new Node(s_head) );
                        return true;
                    }
                }
                return false;
            }


            void Segments:: buildLines( Lines &iso, const bool keep )
            {
                iso.free();
                goto TRY_GROW;

            KEEP:
                assert(0==size);
                for(size_t i=1;i<=iso.size();++i)
                {
                    const Line_    &l = *iso[i];
                    const Node     &p = *(l.head);
                    const Node     &q = *(l.tail);
                    (bool &)l.cyclic  = (p==q);
                }

                if(keep)
                {
                    for(size_t i=1;i<=iso.size();++i)
                    {
                        const Line_ &l = *iso[i];
                        const Node  *p = l.head;
                        while(p&&p->next)
                        {
                            push_back( new Segment( *p, *(p->next) ) );
                            p=p->next;
                        }
                        if(l.cyclic)
                        {
                            assert(l.size>=2);
                            push_back( new Segment( *l.tail, *(l.head->next) ) );
                        }
                    }

                }



                return;

            TRY_GROW:
                {
                    //--------------------------------------------------------------
                    //
                    // try to place all remaining segments
                    //
                    //--------------------------------------------------------------
                    Segments store;         // local store
                    bool     grown = false; // check if something was grown...
                    while(size>0)
                    {
                        if(grow(iso,head))
                        {
                            grown = true;
                            delete pop_front();
                        }
                        else
                        {
                            store.push_back( pop_front() );
                        }
                    }

                    //--------------------------------------------------------------
                    //
                    // check status
                    //
                    //--------------------------------------------------------------

                    if(store.size<=0)
                    {
                        //__________________________________________________________
                        //
                        // early return, nothing left
                        //__________________________________________________________
                        goto KEEP;
                    }
                    else
                    {
                        //__________________________________________________________
                        //
                        // still some segments to place
                        //__________________________________________________________
                        if(grown)
                        {
                            //______________________________________________________
                            //
                            // iso has changed, try again
                            //______________________________________________________
                            assert(store.size>0);
                            swap_with(store);
                            goto TRY_GROW;
                        }
                        else
                        {
                            //______________________________________________________
                            //
                            // nothing has changed, need a new curve
                            //______________________________________________________
                            assert(store.size>0);
                            Line           I = new Line_();
                            const Segment *s = store.head;
                            I->push_back( new Node(s->head) );
                            I->push_back( new Node(s->tail) );
                            iso.push_back(I);
                            delete store.pop_front();
                            if(store.size<=0)
                            {
                                goto KEEP;      // second early return
                            }
                            else
                            {
                                swap_with(store);
                                goto TRY_GROW; // try with new topology
                            }
                        }

                    }

                }
            }


        }

    }

}

