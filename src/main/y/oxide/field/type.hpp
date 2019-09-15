//! \file

#ifndef Y_OXIDE_FIELD_TYPE_INCLUDED
#define Y_OXIDE_FIELD_TYPE_INCLUDED 1

#include "y/oxide/layout.hpp"
#include "y/string.hpp"
#include "y/ios/plugin.hpp"
#include <typeinfo>

namespace upsylon
{
    namespace Oxide
    {


        //----------------------------------------------------------------------
        //
        //
        //! base type for field, common part and abstract API
        //
        //
        //----------------------------------------------------------------------
        class Field : public counted_object
        {
        public:
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const string               name;         //!< identifiers
            const size_t               localObjects; //!< available local linear Objects
            const size_t               ownedObjects; //!< owned built objects
            const size_t               linearExtent; //!< EXPECTED linear extent in bytes
            const size_t               sizeOfObject; //!< sizeof(T)
            const std::type_info      &typeOfObject; //!< system wide type info name
            const ios::plugin::pointer transfer;     //!< type related load/save method for one object


            //------------------------------------------------------------------
            //
            // virtual interface
            //
            //------------------------------------------------------------------
            //! cleanup and remove privateData
            virtual ~Field() throw();

            //! get address of an object by its index (for asynchronous copy)
            virtual const void *getObjectAddr( const Coord1D index ) const throw() = 0;

            //! internal copy of object between two indices
            virtual void copyInternalObject(const Coord1D target, const Coord1D source)    = 0;

            //! external copy of data between two different fiedls
            virtual void copyExternalObject(const Coord1D target,
                                            const Field   &other,
                                            const Coord1D  source) = 0;

            //------------------------------------------------------------------
            //
            // non-virtual interface
            //
            //------------------------------------------------------------------
            size_t save( ios::ostream &fp, const Coord1D index) const;  //!< save one object from index
            size_t load( ios::istream &fp, const Coord1D index);        //!< load one object into index

            //! save from a sublayout
            template <typename LAYOUT>
            size_t save( ios::ostream &fp, const LAYOUT &outer, const LAYOUT &inner  ) const
            {
                assert(outer.contains(inner));
                typename LAYOUT::Loop loop(inner.lower,inner.upper);
                size_t total = 0;
                for(loop.start();loop.valid();loop.next())
                {
                    total += save(fp, outer.indexOf(loop.value) );
                }
                return total;
            }

            //! load from a sublayout
            template <typename LAYOUT>
            size_t load( ios::istream &fp, const LAYOUT &outer, const LAYOUT &inner )
            {
                assert(outer.contains(inner));
                typename LAYOUT::Loop loop(inner.lower,inner.upper);
                size_t total = 0;
                for(loop.start();loop.valid();loop.next())
                {
                    total += load(fp, outer.indexOf(loop.value)  );
                }
                return total;
            }

            //! saving objects from a sequence of indices
            template <typename SEQUENCE> inline
            size_t save(const SEQUENCE &indices, ios::ostream &fp  ) const
            {
                size_t total = 0;
                size_t n     = indices.size();
                for( typename SEQUENCE::const_iterator i=indices.begin(); n>0; --n, ++i)
                {
                    total += save(fp,*i);
                }
                return total;
            }

            //! reload objects from a sequence of indices
            template <typename SEQUENCE> inline
            size_t load(const SEQUENCE &indices, ios::istream &fp   )
            {
                size_t total = 0;
                {
                    size_t n     = indices.size();
                    for( typename SEQUENCE::const_iterator i=indices.begin(); n>0; --n, ++i)
                    {
                        total += load(fp,*i);
                    }
                }
                return total;
            }

            //! scatter to peer a common sub layout, with different outer layouts!
            template <typename LAYOUT>
            void  scatter(const LAYOUT &sub,
                          const LAYOUT &self_outer,
                          Field        &peer,
                          const LAYOUT &peer_outer) const
            {
                assert(self_outer.contains(sub));
                assert(peer_outer.contains(sub));
                typename LAYOUT::Loop loop(sub.lower,sub.upper);
                for(loop.start();loop.valid();loop.next())
                {
                    peer.copyExternalObject( peer_outer.indexOf(loop.value) , *this, self_outer.indexOf(loop.value) );
                }
            }

            //! gather from peer a common sub layout, with different outer layouts
            template <typename LAYOUT>
            void  gather(const LAYOUT &sub,
                          const LAYOUT &self_outer,
                          const Field        &peer,
                          const LAYOUT &peer_outer)
            {
                assert(self_outer.contains(sub));
                assert(peer_outer.contains(sub));
                typename LAYOUT::Loop loop(sub.lower,sub.upper);
                for(loop.start();loop.valid();loop.next())
                {
                    copyExternalObject( self_outer.indexOf(loop.value), peer, peer_outer.indexOf(loop.value) );
                }
            }





            //! forward to the name comparison
            friend bool operator<(const Field &lhs, const Field &rhs) throw();
          
            //! create "name[n]"
            string subName( const Coord1D n ) const;


        protected:
            explicit Field(const string &id, const LayoutInfo &L, const size_t szObj, const std::type_info &); //!< setup
            explicit Field(const char   *id, const LayoutInfo &L, const size_t szObj, const std::type_info &); //!< setup

            void    *privateData; //!< private data if dynamic
            size_t   privateSize; //!< private size if dynamic

            void *acquirePrivate();          //!< allocate privateData with privateSize
            void  releasePrivate() throw();  //!< release privateData


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Field);
        };

       
    }
}

#endif

