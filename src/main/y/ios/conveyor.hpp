//! \file

#ifndef Y_IOS_CONVEYOR_INCLUDED
#define Y_IOS_CONVEYOR_INCLUDED 1

#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"
#include "y/ios/comms.hpp"
#include <typeinfo>
#include <iosfwd>

namespace upsylon {
 
    namespace ios
    {
        
        //! abstract function to convey data
        class conveyor : public counted_object
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            
            //! cleanup
            virtual ~conveyor() throw();
           
            //! display information
            friend std::ostream & operator<<(std::ostream &, const conveyor &);
            
            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            
            virtual size_t copy(void *target, const void *source)         const = 0; //!< direct and local copy
            virtual size_t save(ios::ostream &target, const void *source) const = 0; //!< save for later
            virtual size_t load(void *target, ios::istream &source)       const = 0; //!< reload
                                                                              
            //__________________________________________________________________
            //
            // non-virtual interface
            //__________________________________________________________________
           
            //! copy a range of iterators
            template <typename TARGET_ITERATOR,typename SOURCE_ITERATOR> inline
            size_t copy_range( TARGET_ITERATOR target, SOURCE_ITERATOR source, size_t count) const
            {
                size_t num = 0;
                while(count-- > 0 )
                {
                    num += copy( & (*target), & (*source) );
                    ++target;
                    ++source;
                }
                return num;
            }
            
            //! copy matching sequences
            template <typename TARGET, typename SOURCE> inline
            size_t copy_block( TARGET &target, const SOURCE &source ) const
            {
                assert( typeid(typename TARGET::mutable_type) == uuid );
                assert( typeid(typename SOURCE::mutable_type) == uuid );
                assert( target.size() == source.size() );
                return copy_range(target.begin(),source.begin(),target.size());
            }
            
            //! save a range of iterators
            template <typename SOURCE_ITERATOR> inline
            size_t save_range(ios::ostream &target, SOURCE_ITERATOR source, size_t count) const
            {
                size_t num = 0;
                while(count-- > 0)
                {
                    num += save(target, & (*source) );
                    ++source;
                }
                return num;
            }
            
            //! save a sequence
            template <typename SOURCE> inline
            size_t save_block(ios::ostream &target, const SOURCE &source ) const
            {
                assert( typeid(typename SOURCE::mutable_type) == uuid );
                return save_range(target,source.begin(),source.size());
            }
            
            //! load a range of iterators
            template <typename TARGET_ITERATOR> inline
            size_t load_range(TARGET_ITERATOR target, ios::istream &source, size_t count) const
            {
                size_t num = 0;
                while(count-- > 0)
                {
                    num += load( & (*target), source);
                    ++target;
                }
                return num;
            }
            
            //! load a sequence
            template <typename TARGET> inline
            size_t load_block( TARGET &target, ios::istream &source ) const
            {
                assert( typeid(typename TARGET::mutable_type) == uuid );
                return load_range(target.begin(),source,target.size());
            }
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const comms::shipping_style    style; //!< [computed|flexible]_block_size
            const comms::infrastructure    infra; //!< [homogeneous|distributed]
            const std::type_info          &uuid; //!< system type info
            const size_t                   move; //!< sizeof(T)
            
           
        protected:
            explicit conveyor(const comms::shipping_style,
                              const comms::infrastructure,
                              const std::type_info &,
                              const size_t) throw(); //!< filled by derived class
            void     missing_bytes() const; //!< raise exception
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(conveyor);
        };
        
    }
    
}

#endif

