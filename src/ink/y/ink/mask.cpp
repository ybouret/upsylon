#include "y/ink/mask.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Ink
    {
        Mask:: ~Mask() throw()
        {
        }

        Mask:: Mask() : container(), list(), dict(), pool()
        {

        }

        Mask:: Mask(const Mask &other) :  container(), list(), dict( other.size(), as_capacity ), pool()
        {
            match_capacities();
            for( const Vertex *v = other.head(); v; v=v->next )
            {
                if(!append(v->position))
                {
                    throw exception("Ink.Mask(COPY) unexpected failure");
                }
            }
        }

        bool Mask:: append( const coord q)
        {
            if( dict.search(q) )
            {
                return false;
            }
            else
            {
                // Phase 1/2 dual insertion
                {
                    Vertex *v = pool(q);
                    list.push_back( v );
                    try
                    {

                        const MetaVertex mv(v);
                        if(!dict.insert(mv))
                        {
                            throw exception("Ink.Mask.append(unexpected failure)");
                        }
                    }
                    catch(...)
                    {
                        pool.store( list.pop_back() );
                        throw;
                    }
                }

                // Phase 2/2 adjust capacity
                try
                {
                    match_capacities();
                }
                catch(...)
                {
                    pool.store( list.pop_back() );
                    dict.no(q);
                    throw;
                }

                return true;
            }
        }

        bool Mask:: remove(const coord q) throw()
        {
            MetaVertex *mvp = dict.search(q);
            if(mvp)
            {
                pool.store( list.unlink(mvp->vtx) );
                dict.no(q);
                return true;
            }
            else
            {
                return false;
            }
        }

        void  Mask:: reserve(const size_t n)
        {
            dict.reserve(n);
            match_capacities();
        }

        size_t Mask:: size() const throw() { assert( list.size == dict.size() ); return list.size; }

        void Mask:: free() throw()
        {
            pool.store_all(list);
            dict.free();
        }

        void  Mask:: match_capacities() const
        {
            const size_t n = dict.capacity();
            const coord  z;
            while( list.size+pool.size < n )
            {
                pool.store( new Vertex(z) );
            }
        }

        bool  Mask:: try_match_capacities() const throw()
        {
            try
            {
                match_capacities(); return true;
            }
            catch(...)
            {
                return false;
            }
        }

        void Mask:: release() throw()
        {
            dict.release();
            pool.release();
            list.release();
        }

        size_t Mask:: capacity() const throw()
        {
            return dict.capacity();
        }

        const Vertex * Mask:: head() const throw()
        {
            return list.head;
        }

        Mask & Mask:: operator=( const Mask &other )
        {
            if(this!=&other)
            {
                free();
                ensure(other.size());
                for(const Vertex *v=other.head();v;v=v->next)
                {
                    if(!append(v->position))
                    {
                        throw exception("Ink.Mask= failure");
                    }
                }
            }
            return *this;
        }


        Mask & Mask:: __or( const Mask &other )
        {
            if(this!=&other)
            {
                for( const Vertex *v = other.head(); v; v=v->next )
                {
                    (void) append(v->position);
                }
            }
            // else unchanged
            return *this;
        }

        Mask & Mask:: __and( const Mask &other )
        {
            if(this!=&other)
            {
                Vertex::List result;
                for(const Vertex *v = other.list.head; v; v=v->next )
                {
                    const coord q = v->position;
                    if( has(q) ) result.push_back( new Vertex(q) );
                }
                free();
                while(result.size)
                {
                    if(!append(result.tail->position))
                        throw exception("Ink.Mask.__and failure");
                    delete result.pop_back();
                }
            }
            // else unchanged
            return *this;
        }

        Mask & Mask:: __xor( const Mask &other )
        {
            if(this!=&other)
            {
                Vertex::List to_append; //!< separated
                Vertex::List to_remove; //!< common
                for(const Vertex *v = other.list.head; v; v=v->next )
                {
                    const coord q = v->position;
                    if( has(q) )
                    {
                        to_remove.push_back( new Vertex(q) );
                    }
                    else
                    {
                        to_append.push_back( new Vertex(q) );
                    }
                }
                
                while(to_remove.size)
                {
                    if( ! remove( to_remove.tail->position ) )
                    {
                        throw exception("Ink.Mask.__xor remove failure");
                    }
                    delete to_remove.pop_back();
                }

                while(to_append.size)
                {
                    if( ! append( to_append.tail->position ) )
                    {
                        throw exception("Ink.Mask.__xor append failure");
                    }
                    delete to_append.pop_back();
                }

            }
            else
            {
                free();
            }
            return *this;
        }

    }
}
