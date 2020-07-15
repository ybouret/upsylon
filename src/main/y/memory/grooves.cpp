
#include "y/memory/grooves.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon {

    namespace memory {

        grooves:: ~grooves() throw()
        {
        }

        static inline
        storage::model __set_default_model(const storage::model m) throw()
        {
            switch(m)
            {
                case storage::unused: return storage::shared;
                default:
                    break;
            }
            return m;
        }

        grooves:: grooves(const storage::model m) throw() :
        shift(0),
        front(),
        model(__set_default_model(m))
        {

        }


        const groove & grooves:: handler() const throw()
        {
            return front;
        }

        groove & grooves:: create1()
        {
            make(1);
            return front.as<groove>();
        }


        size_t grooves:: size() const throw()
        {
            return front.count;
        }

        groove & grooves:: operator[](const size_t indx) throw()
        {
            assert(shift);
            assert(indx>=1);
            assert(indx<=size());
            return shift[indx];
        }

        const groove & grooves:: operator[](const size_t indx) const throw()
        {
            assert(shift);
            assert(indx>=1);
            assert(indx<=size());
            return shift[indx];
        }

        void grooves:: upgrade(const storage::model m)
        {
            static const char fn[] = "memory::grooves.upgrade";

            if( size()>0 )            throw exception("%s(in use)",fn);
            if( storage::unused == m) throw libc::exception(EINVAL,"%s('%s')",fn,storage::text(m));

            aliasing::_(model) = m;

        }

        void grooves:: make(const size_t n)
        {
            if(n==size())
            {
                free();
            }
            else
            {
                release();
                front.make<groove>(model,n);
                shift = (& front.as<groove>() ) - 1;
            }
        }


        void grooves:: free() throw()
        {
            for(size_t i=front.count;i>0;--i)
            {
                assert(shift);
                shift[i].free();
            }
        }

        void grooves:: release() throw()
        {
            front.release();
            shift = 0;
        }

        groove       & grooves:: operator*()       throw()
        {
            assert(front.count>0);
            return front.get<groove>();
        }

        const groove & grooves:: operator*() const throw()
        {
            assert(front.count>0);
            return front.get<groove>();
        }

        void grooves:: ready_(const storage::model which, const size_t sz[], const size_t ns)
        {
            assert(sz);
            assert(ns);
            make(ns);
            for(size_t i=0;i<ns;++i)
            {
                front.as<groove>(i).acquire(which,sz[i]);
            }
        }


    }

}
