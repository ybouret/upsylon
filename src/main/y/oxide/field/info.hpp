//! \file

#ifndef Y_OXIDE_FIELD_INFO_INCLUDED
#define Y_OXIDE_FIELD_INFO_INCLUDED 1

#include "y/oxide/layout.hpp"
#include "y/string.hpp"

namespace upsylon
{
    namespace Oxide
    {

        //! base type for field, common part and abstract API
        class FieldInfo : public counted_object
        {
        public:
            const string           name;         //!< identifiers
            const size_t           ownedObjects; //!< owned built objects
            const size_t           linearExtent; //!< EXPECTED linear extent in bytes
            const size_t           sizeOfObject; //!< sizeof(T)

            virtual ~FieldInfo() throw();            //!< cleanup and remove privateData
            string subName( const Coord1D n ) const; //!< create 'name[n]'

        protected:
            //! setup name and number of linear byte
            explicit FieldInfo(const string &id, const LayoutInfo &L, const size_t szObj); //!< setup
            explicit FieldInfo(const char   *id, const LayoutInfo &L, const size_t szObj); //!< setup

            void    *privateData; //!< private data if dynamic
            size_t   privateSize; //!< private size if dynamic

            void *acquirePrivate();          //!< allocate privateData
            void  releasePrivate() throw();  //!< release privateData


        private:
            Y_DISABLE_COPY_AND_ASSIGN(FieldInfo);
        };

       
    }
}

#endif

