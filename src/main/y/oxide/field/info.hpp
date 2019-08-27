//! \file

#ifndef Y_OXIDE_FIELD_INFO_INCLUDED
#define Y_OXIDE_FIELD_INFO_INCLUDED 1

#include "y/oxide/layout.hpp"
#include "y/string.hpp"
#include "y/ios/imstream.hpp"

namespace upsylon
{
    namespace Oxide
    {

        //! base type for field, common part and abstract API
        class FieldInfo : public counted_object
        {
        public:
            const string           name;         //!< identifiers
            const size_t           localObjects; //!< available local linear Objects
            const size_t           ownedObjects; //!< owned built objects
            const size_t           linearExtent; //!< EXPECTED linear extent in bytes
            const size_t           sizeOfObject; //!< sizeof(T)

            virtual ~FieldInfo() throw();            //!< cleanup and remove privateData
            string subName( const Coord1D n ) const; //!< create 'name[n]'

            //------------------------------------------------------------------
            // virtual interface
            //------------------------------------------------------------------
            //! get address of an object by its index
            virtual const void *getObjectAddr( const Coord1D index ) const throw() = 0;

            //------------------------------------------------------------------
            // non-virtual interface
            //------------------------------------------------------------------
            typedef void (*SaveProc)( ios::ostream &, const void *); //!< saving  prototype
            typedef void (*LoadProc)( ios::istream &, void *);       //!< loading prototype

            void    save( ios::ostream &fp, const Coord1D index, SaveProc proc ) const; //!< save one object
            void    save( ios::ostream &fp, SaveProc proc) const;                       //!< save all objects
            void    load( ios::istream &fp, const Coord1D index, LoadProc proc);        //!< load one object
            void    load( ios::istream &fp, LoadProc proc);                             //!< load all objects

            //! saving objects from a sequence of indices
            template <typename SEQUENCE> inline
            void save_only(const SEQUENCE &indices, ios::ostream &fp, SaveProc proc) const
            {
                size_t n = indices.size();
                for( typename SEQUENCE::const_iterator i=indices.begin(); n>0; --n, ++i)
                {
                    save(fp,*i,proc);
                }
            }

            //! reload objects from a sequence of indices
            template <typename SEQUENCE> inline
            void load_only(const SEQUENCE &indices, ios::istream &fp, LoadProc proc )
            {
                size_t n = indices.size();
                for( typename SEQUENCE::const_iterator i=indices.begin(); n>0; --n, ++i)
                {
                    load(fp,*i,proc);
                }
            }

            //! load from read-only buffer
            void load(const memory::ro_buffer &buff, const Coord1D index, LoadProc proc);
            
            //! load all from read-only buffer
            void load(const memory::ro_buffer &buff, LoadProc proc);

            //! reload objects from a sequence of indices and a read-only buffer
            template <typename SEQUENCE> inline
            void load_only(const SEQUENCE &indices, const memory::ro_buffer &buff, LoadProc proc )
            {
                ios::imstream fp(buff);
                load_only<SEQUENCE>(indices,fp,proc);
            }


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

