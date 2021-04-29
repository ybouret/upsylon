
#include "y/ios/carrier/db.hpp"
#include "y/type/complex.hpp"
#include "y/type/point3d.hpp"
#include "y/yap.hpp"

#include "y/exception.hpp"

namespace upsylon
{
    Y_SINGLETON_IMPL(ios::carriers);
    
    namespace ios
    {
        carriers:: ~carriers() throw()
        {
        }
        
        carriers:: carriers() :
        homogeneous(),
        distributed()
        {
            // simple types
            record<uint8_t>();
            record<uint16_t>();
            record<uint32_t>();
            record<uint64_t>();

            record<int8_t>();
            record<int16_t>();
            record<int32_t>();
            record<int64_t>();
            
            record<float>();
            record<double>();

            record<char>();
            record<short>();
            record<int>();
            record<long>();
            record<long long>();
         
            record<unsigned char>();
            record<unsigned short>();
            record<unsigned int>();
            record<unsigned long>();
            record<unsigned long long>();

            // tuples
            record_tuple<complex,float>();
            record_tuple<complex,double>();
            
            record_tuple<point2d,float>();
            record_tuple<point2d,double>();
            
            record_tuple<point3d,float>();
            record_tuple<point3d,double>();
            
            // serializable
            use<string>();
            use<apn>();
            use<apq>();
            use<apz>();
            
        }
        
        const carrier * carriers:: search(const std::type_info       &tid,
                                          const comms::infrastructure infra) const throw()
        {
 

            switch(infra)
            {
                case comms::homogeneous:
                {
                    const carrier::pointer *ppC = homogeneous.search(tid);
                    if(ppC) return & (**ppC);
                } /* FALLTHRU */
                case comms::distributed:
                {
                    const carrier::pointer *ppC = distributed.search(tid);
                    if(ppC) return & (**ppC);
                }
            }
            return NULL;
        }
        
        const carrier & carriers:: query(const std::type_info       &tid,
                                         const comms::infrastructure infra,
                                         const char                 *where) const
        {
            
            const carrier *p = search(tid,infra);
            if(!p)
            {
                if(!where) where="program";
                throw exception("ios::carriers<%s>: unhandled (in %s)", tid.name(), where);
            }
            return *p;
        }
        
        const carrier *carrier_search(const std::type_info       &tid,
                                      const comms::infrastructure infra)
        {
            static const carriers &db = carriers::instance();
            return db.search(tid,infra);
        }
        
        
    }
    
}


        
