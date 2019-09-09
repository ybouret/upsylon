//! \file

#ifndef Y_OXIDE_FIELD_TYPE_INCLUDED
#define Y_OXIDE_FIELD_TYPE_INCLUDED 1

#include "y/oxide/layout.hpp"
#include "y/string.hpp"
#include "y/ios/plugin.hpp"
#include "y/ios/imstream.hpp"

namespace upsylon
{
    namespace Oxide
    {

        //! base type for field, common part and abstract API
        class FieldType : public counted_object
        {
        public:
            const string           name;         //!< identifiers
            const size_t           localObjects; //!< available local linear Objects
            const size_t           ownedObjects; //!< owned built objects
            const size_t           linearExtent; //!< EXPECTED linear extent in bytes
            const size_t           sizeOfObject; //!< sizeof(T)

            virtual ~FieldType() throw();            //!< cleanup and remove privateData
            string subName( const Coord1D n ) const; //!< create 'name[n]'

            //------------------------------------------------------------------
            // virtual interface
            //------------------------------------------------------------------

            //! get address of an object by its index
            virtual const void *getObjectAddr( const Coord1D index ) const throw() = 0;

            //! copy data between two indices
            virtual void copyObject(const Coord1D target, const Coord1D source)    = 0;


            //------------------------------------------------------------------
            // non-virtual interface
            //------------------------------------------------------------------
            typedef ios::plugin::save_proc SaveProc; //!< using ios::plugin::save_proc to save data to ostream
            typedef ios::plugin::load_proc LoadProc; //!< using ios::plugin::load_proc to load data from istream

            size_t    save( ios::ostream &fp, const Coord1D index, SaveProc proc ) const; //!< save one object
            size_t    save( ios::ostream &fp, SaveProc proc) const;                       //!< save all objects
            size_t    load( ios::istream &fp, const Coord1D index, LoadProc proc);        //!< load one object
            size_t    load( ios::istream &fp, LoadProc proc);                             //!< load all objects

            //! saving objects from a sequence of indices
            template <typename SEQUENCE> inline
            size_t save_only(const SEQUENCE &indices, ios::ostream &fp, SaveProc proc) const
            {
                size_t total = 0;
                size_t n = indices.size();
                for( typename SEQUENCE::const_iterator i=indices.begin(); n>0; --n, ++i)
                {
                    total += save(fp,*i,proc);
                }
                return total;
            }

            //! reload objects from a sequence of indices
            template <typename SEQUENCE> inline
            size_t load_only(const SEQUENCE &indices, ios::istream &fp, LoadProc proc )
            {
                size_t total = 0;
                size_t n = indices.size();
                for( typename SEQUENCE::const_iterator i=indices.begin(); n>0; --n, ++i)
                {
                    total += load(fp,*i,proc);
                }
                return total;
            }

            //! load from read-only buffer
            size_t load(const memory::ro_buffer &buff, const Coord1D index, LoadProc proc);
            
            //! load all  from read-only buffer
            size_t load(const memory::ro_buffer &buff, LoadProc proc);

            //! reload objects from a sequence of indices and a read-only buffer
            template <typename SEQUENCE> inline
            size_t load_only(const SEQUENCE &indices, const memory::ro_buffer &buff, LoadProc proc )
            {
                ios::imstream fp(buff);
                return load_only<SEQUENCE>(indices,fp,proc);
            }


        protected:
            //! setup name and number of linear byte
            explicit FieldType(const string &id, const LayoutInfo &L, const size_t szObj); //!< setup
            explicit FieldType(const char   *id, const LayoutInfo &L, const size_t szObj); //!< setup

            void    *privateData; //!< private data if dynamic
            size_t   privateSize; //!< private size if dynamic

            void *acquirePrivate();          //!< allocate privateData with privateSize
            void  releasePrivate() throw();  //!< release privateData


        private:
            Y_DISABLE_COPY_AND_ASSIGN(FieldType);
        };

       
    }
}

#endif

