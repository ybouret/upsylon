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
                T fetch(const string &name, const T default_value) const
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

                bool fetch(const string &name) const; //!< default : false

                template <typename T>
                static T get(const options *opts, const string &name, const T default_value)
                {
                    return opts ? opts->fetch(name,default_value) : default_value;
                }

                template <typename T>
                static T get(const options *opts, const char *name, const T default_value)
                {
                    const string _(name);
                    return get<T>(opts,_,default_value);
                }

                static bool flag(const options *opts, const string &name)
                {
                    return opts ? opts->fetch(name) : false;
                }

                static bool flag(const options *opts, const char *name)
                {
                    const string _(name);
                    return flag(opts,_);
                }



            private:
                Y_DISABLE_COPY_AND_ASSIGN(options);
                void add(const string &field);
            };

        }

    }

}

#endif

