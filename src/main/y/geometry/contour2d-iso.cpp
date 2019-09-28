#include "y/geometry/contour2d.hpp"


namespace upsylon {

    namespace geometry {

        contour2d:: isopoint:: isopoint( const point &p ) throw() :
        point(p), next(0), prev(0)
        {
        }

        contour2d:: isopoint:: ~isopoint() throw()
        {
        }

    }
}

namespace upsylon {

    namespace geometry {

        contour2d:: isoline_:: isoline_() throw() :
        cyclic(false)
        {
        }

        contour2d:: isoline_:: ~isoline_() throw()
        {
        }



    }
}


namespace upsylon {

    namespace geometry {

        bool contour2d:: segments::  grow( isolines &iso, const segment *s )
        {
            assert(s);
            const point &s_head = s->head;
            const point &s_tail = s->tail;
            for(size_t i=iso.size();i>0;--i)
            {
                isoline_    &C      = *iso[i]; assert(C.size>=2);
                const point &c_head = *(C.head);
                const point &c_tail = *(C.tail);

                if(c_head==s_head)
                {
                    C.push_front( new isopoint(s_tail) );
                    return true;
                }

                if(c_head==s_tail)
                {
                    C.push_front( new isopoint(s_head) );
                    return true;
                }

                if(c_tail==s_head)
                {
                    C.push_back( new isopoint(s_tail) );
                    return true;
                }

                if(c_tail==s_tail)
                {
                    C.push_back( new isopoint(s_head) );
                    return true;
                }
            }
            return false;
        }


        void contour2d:: segments:: build( isolines &iso, const bool keep )
        {
            iso.free();
            goto TRY_GROW;

        KEEP:
            assert(0==size);
            if(keep)
            {
                for(size_t i=1;i<=iso.size();++i)
                {
                    const isopoint *p = iso[i]->head;
                    while(p&&p->next)
                    {
                        push_back( new segment( *p, *(p->next) ) );
                        p=p->next;
                    }
                }

            }

            for(size_t i=1;i<=iso.size();++i)
            {
                const isoline_ &l = *iso[i];
                const isopoint &p = *(l.head);
                const isopoint &q = *(l.tail);
                (bool &)l.cyclic = (p==q);
            }

            return;

        TRY_GROW:
            {
                //--------------------------------------------------------------
                //
                // try to place all remaining segments
                //
                //--------------------------------------------------------------
                segments store;         // local store
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
                        isoline        I = new isoline_();
                        const segment *s = store.head;
                        I->push_back( new isopoint(s->head) );
                        I->push_back( new isopoint(s->tail) );
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
