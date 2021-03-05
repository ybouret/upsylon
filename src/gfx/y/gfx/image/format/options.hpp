//! \file

#ifndef Y_GFX_IMAGE_FORMAT_OPTIONS_INCLUDED
#define Y_GFX_IMAGE_FORMAT_OPTIONS_INCLUDED 1

#include "y/gfx/image/format/option.hpp"
#include "y/associative/hash/set.hpp"
#include "y/string/convert.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace image
        {
            //__________________________________________________________________
            //
            //! base class for options
            //__________________________________________________________________
            typedef hash_set<string,option::pointer> options_;

            //__________________________________________________________________
            //
            //! database of options
            //__________________________________________________________________
            class options : public options_
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit options();                //!< empty
                explicit options(const string &);  //!< parse string
                explicit options(const char   *);  //!< parse optional text
                virtual ~options() throw();        //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                void parse(const string &opts); //!< parse opts [name=value:...]
                void parse(const char   *opts); //!< parse wrapper

                //! get with default value:
                /**
                 - size_t, unit_t, float, double, bool
                 */
                template <typename T> inline
                T get(const string &name, const T default_value) const
                {
                    const option::pointer *ppOpt = search(name);
                    if(ppOpt)
                    {
                        return string_convert::to<T>( (**ppOpt).value, *name);
                    }
                    else
                    {
                        return default_value;
                    }
                }

                //! get with default value wrapper
                template <typename T> inline
                T get(const char *name, const T default_value) const
                {
                    const string _(name); return get<T>(_,default_value);
                }

                bool flag(const string &name) const; //!< default : false
                bool flag(const char   *name) const; //!< flag wrapper

            private:
                Y_DISABLE_COPY_AND_ASSIGN(options);
                void add(const string &field);
            };

        }

    }

}

#endif

