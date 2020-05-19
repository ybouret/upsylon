
#include "y/ios/conveyors.hpp"
#include "y/type/complex.hpp"
#include "y/type/point3d.hpp"

namespace upsylon {
    
    namespace ios
    {
        static inline void __ld( conveyors &self, const comms::topology topo )
        {
            (void) self.query<uint8_t>(topo);
            (void) self.query<uint16_t>(topo);
            (void) self.query<uint32_t>(topo);
            (void) self.query<uint64_t>(topo);
            
            (void) self.query<int8_t>(topo);
            (void) self.query<int16_t>(topo);
            (void) self.query<int32_t>(topo);
            (void) self.query<int64_t>(topo);
            
            (void) self.query<char>(topo);
            (void) self.query<short>(topo);
            (void) self.query<int>(topo);
            (void) self.query<long>(topo);
            (void) self.query<long long>(topo);

            (void) self.query<unsigned char>(topo);
            (void) self.query<unsigned short>(topo);
            (void) self.query<unsigned int>(topo);
            (void) self.query<unsigned long>(topo);
            (void) self.query<unsigned long long>(topo);

            (void) self.query<float>(topo);
            (void) self.query<double>(topo);


            
            (void) self.query<complex,float>(topo);
            (void) self.query<complex,double>(topo);
            
            (void) self.query<point2d,float>(topo);
            (void) self.query<point2d,double>(topo);
            
            (void) self.query<point3d,float>(topo);
            (void) self.query<point3d,double>(topo);
            
            (void) self.query<point2d,unit_t>(topo);
            (void) self.query<point3d,unit_t>(topo);
            

        }
        
        void  conveyors:: import(const comms::topology topo)
        {
            __ld(*this,topo);
            //__ld(*this,comms::distributed);
            sort();
        }
        
    }
    
}

