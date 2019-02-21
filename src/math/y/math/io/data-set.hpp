//! \file
#ifndef Y_MATH_DATA_SET_INCLUDED
#define Y_MATH_DATA_SET_INCLUDED 1

#include "y/associative/map.hpp"
#include "y/container/sequence.hpp"
#include "y/ios/icstream.hpp"
#include "y/exception.hpp"
#include "y/string/tokenizer.hpp"
#include "y/string/convert.hpp"
#include "y/comparison.hpp"

namespace upsylon
{
    namespace math
    {
        namespace kernel
        {
            //! separators for columns
            inline bool data_set_separator(const int C) { return ' '==C || '\t' == C; }

        }

        //! temporaty object to load data files
        template <typename T>
        class data_set
        {
        public:
            Y_DECL_ARGS(T,type);                //!< type alias
            typedef sequence<type>     *column; //!< simple pointer to a sequence
            typedef map<size_t,column>  colmap; //!< [index:column] map

            //! constructor, reserve a little memory
            inline explicit data_set(size_t n=2) : columns(n,as_capacity) {}
            //! destructor
            inline virtual ~data_set() throw() {}
            //! link a column index to a columnx
            inline void use( const size_t column_index, sequence<type> &seq )
            {
                if(column_index<=0) throw exception("data_set: column index <= 0");
                if(!columns.insert(column_index,&seq))
                {
                    throw exception("data_set: multiple column #%u", unsigned(column_index));
                }
            }

            //! free all register columns
            inline void free_all() throw()
            {
                for(typename colmap::iterator i=columns.begin();i!=columns.end();++i)
                {
                    (**i).free();
                }
            }

            //! load data from input stream,
            inline void load( ios::istream &fp, const size_t skip=0, const size_t nmax=0)
            {
                // preparing columns, order by increasing index
                columns.sort_keys( comparison::increasing<size_t> );

                unsigned iline = 1;
                string   line;
                for(;iline<=skip;++iline)
                {
                    if(!fp.gets(line)) throw exception("data_set: EOF while skipping line %u/%u", iline, unsigned(skip));
                }
                size_t loaded = 0;
                while(fp.gets(line))
                {
                    // check content
                    if(line.size()<=0) continue;
                    if(line[0] == '#') continue;


                    // now parse!
                    tokenizer<char> tkn(line);
                    for(typename colmap::iterator i=columns.begin();i!=columns.end();++i)
                    {
                        const size_t idx = i.key();
                        while( tkn.count() != idx )
                        {
                            if(!tkn.next(kernel::data_set_separator))
                            {
                                throw exception("%u:data_set missing column #%u",iline,unsigned(idx));
                            }
                        }
                        const string word  = tkn.to_string();
                        const T      value = string_convert::to<T>(word); // todo: add field?
                        (**i).push_back(value);
                    }
                    if(nmax>0&&++loaded>=nmax)
                    {
                        break;
                    }
                    ++iline;
                }
            }

            //! load using file name
            inline void load( const string &filename,const size_t skip=0, const size_t nmax=0)
            {
                ios::icstream fp(filename);
                load(fp,skip,nmax);
            }

            //! load using file name
            inline void load( const char *filename,const size_t skip=0, const size_t nmax=0)
            {
                const string _(filename);
                load(_,skip,nmax);
            }

            //! wrapper to load X/Y quickly
            static inline
            void loadXY(const string &filename,
                        const size_t  ix,
                        sequence<T>  &X,
                        const size_t  iy,
                        sequence<T>  &Y,
                        const size_t skip=0, const size_t nmax=0)
            {
                data_set<T> ds(2);
                ds.use(ix,X);
                ds.use(iy,Y);
                ds.load(filename,skip,nmax);
            }

            //! wrapper to load X/Y quickly
            static inline
            void loadXY(const char   *filename,
                        const size_t  ix,
                        sequence<T>  &X,
                        const size_t  iy,
                        sequence<T>  &Y,
                        const size_t skip=0, const size_t nmax=0)
            {
                const string _(filename);
                loadXY(_,ix,X,iy,Y,skip,nmax);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(data_set);
            colmap columns;
        };
    }
}

#endif

