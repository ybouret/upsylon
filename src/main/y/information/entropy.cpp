#include "y/information/entropy.hpp"

#include <cstring>
#include <cmath>
#include <cstdlib>


namespace upsylon {

    namespace Information {

        Entropy:: Entropy() throw() :
        frequency()
        {
            reset();
        }

        Entropy:: ~Entropy() throw()
        {
            reset();
        }

        void Entropy:: reset() throw()
        {
            memset(frequency,0,sizeof(frequency));
        }

        Entropy & Entropy:: update( const void *data, const size_t size ) throw()
        {
            assert( !(0==data&&size>0) );
            const uint8_t *u = static_cast<const uint8_t *>(data);
            for(size_t i=0;i<size;++i)
            {
                ++frequency[ u[i] ];
            }
            return *this;
        }

        Entropy & Entropy:: operator<<( const char *text ) throw()
        {
            return update( text, text ? strlen(text) : 0 );
        }

        Entropy & Entropy:: operator<<( const memory::ro_buffer &buff ) throw()
        {
            return update( buff.ro(), buff.length() );
         }

        static inline int compareProba(const void *lhs, const void *rhs) throw()
        {
            const double L = *static_cast<const double *>(lhs);
            const double R = *static_cast<const double *>(rhs);
            return (L<R) ? -1 : ( (R<L) ? 1 : 0);
        }

        double Entropy:: operator*() const throw()
        {
            double proba[256] = {0};
            size_t count      = 0;
            {
                size_t sum = 0;
                for(size_t i=0;i<256;++i)
                {
                    sum += frequency[i];
                }
                for(size_t i=0;i<256;++i)
                {
                    const size_t freq = frequency[i];
                    if(freq>0)
                    {
                        const double p = double(freq)/sum;
                        proba[count++] = -p * log( p );
                    }
                }
            }
            if(count>0)
            {
                qsort(proba, count, sizeof(double), compareProba );
                double ans = 0;
                for(size_t i=0;i<count;++i)
                {
                     ans += proba[i];
                }
                return ans;
            }
            else
            {
                return 0;
            }
        }


        double Entropy:: of( const void *data, const size_t size ) throw()
        {
            reset(); return *update(data,size);
        }

        double Entropy:: of( const char *text ) throw()
        {
            reset(); return *( *this << text);
        }

        double Entropy:: of( const memory::ro_buffer &buff  ) throw()
        {
            reset(); return *( *this << buff );
        }

        size_t & Entropy:: operator[](const uint8_t u) throw()
        {
            return frequency[u];
        }

        const size_t & Entropy:: operator[](const uint8_t u) const throw()
        {
            return frequency[u];
        }

    }

}

#include "y/ios/serializable.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {
    
    namespace Information {
        
        namespace {
            
            class estream : public ios::ostream
            {
            public:
                inline explicit estream() throw() : entropy()
                {
                }
                
                inline virtual ~estream() throw()
                {
                }
                
                inline virtual void write( char C )
                {
                    entropy.update(&C,1);
                }
                
                inline virtual void flush() throw()
                {
                    
                }
                
                
                Entropy entropy;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(estream);
            };
        
            }
        
        double Entropy:: Of( const ios::serializable &args )
        {
            estream fp;
            args.serialize(fp);
            return *fp.entropy;
        }

    }
}

