#include "y/program.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ptr/arc.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/tokenizer.hpp"
#include "y/string/convert.hpp"
#include "y/sort/heap.hpp"

using namespace upsylon;

class swap
{
public:
	const unsigned I;
	const unsigned J;
	inline  swap(unsigned i, unsigned j) throw() : I(i), J(j) { assert(I != J); }
	inline ~swap() throw() {}
	inline  swap(const swap &other) throw() : I(other.I), J(other.J) {}

private:
	Y_DISABLE_ASSIGN(swap);
};


class swaps : public counted_object
{
public:
	unsigned     count;
	vector<swap> tests;
	template <char CH>
	static inline bool is_char(const int C) { return CH == C; }

	inline swaps(const string &id, const string &dd) :
		count(0),
		tests(256, as_capacity)
	{
		// get tableau count
		{
			tokenizer<char> tkn(id);
			if (!tkn.next(is_char<'-'>))
			{
				throw exception("Missing Count in swaps title");
			}
			const string cnt = tkn.to_string();
			count = unsigned(string_convert::to<size_t>(cnt, "count"));
			std::cerr << "count=" << count << std::endl;
		}

		{
			string       data = dd;
			data.trim(1).skip(1);
			const string &source = data;
			{
				string        target;
				for (size_t i = 0; i < source.size(); ++i)
				{
					if (',' == source[i] && ' ' == source[i + 1])
					{
						target << ':';
						++i;
					}
					else
					{
						target << source[i];
					}
				}
				data.swap_with(target);
			}
			tokenizer<char> tkn(data);
			while (tkn.next(is_char<':'>))
			{
				string swp = tkn.to_string();
				assert(swp.size() > 2);
				assert(swp.front() == '[');
				assert(swp.back() == ']');
				swp.trim(1).skip(1);
				tokenizer<char> sub(swp);
				if (!sub.next(is_char<','>))
				{
					throw exception("missing I");
				}
				const string I = sub.to_string();
				if (!sub.next(is_char<','>))
				{
					throw exception("missing J");
				}
				const string J = sub.to_string();
				const swap   s(unsigned(string_convert::to<size_t>(I, "I")),
					unsigned(string_convert::to<size_t>(J, "J")));

				tests.push_back(s);
			}
			//std::cerr << std::endl;
		}
		std::cerr << "#tests=" << tests.size() << std::endl;
	}

	virtual ~swaps() throw()
	{
	}

private:
	Y_DISABLE_COPY_AND_ASSIGN(swaps);
};

typedef arc_ptr<swaps> swaps_ptr;

static inline int __compare_swaps_ptr(const swaps_ptr &lhs, const swaps_ptr &rhs) throw()
{
	return comparison::increasing(lhs->count, rhs->count);
}

static inline
void write_indices( ios::ostream &fp, const array<swap> &tests )
{
    const size_t       nt    = tests.size();
    fp("\t\t\tstatic const unsigned I[%u] = {",unsigned(nt));
    for (size_t j = 1; j <= nt; ++j)
    {
        const swap &swp = tests[j];
        fp("0x%02x",swp.I);
        if(j<nt) fp << ',';
    }
    fp("};\n");
    fp("\t\t\tstatic const unsigned J[%u] = {",unsigned(nt));
    for (size_t j = 1; j <= nt; ++j)
    {
        const swap &swp = tests[j];
        fp("0x%02x",swp.J);
        if(j<nt) fp << ',';
    }
    fp("};\n");
}

Y_PROGRAM_START()
{
	if (argc > 1)
	{
		const string filename = argv[1];
		const string output   = vfs::with_new_extension(filename, "hpp");
		std::cerr << "input : " << filename << std::endl;
		std::cerr << "output: " << output << std::endl;

		vector<swaps_ptr> Swaps(128, as_capacity);
		{
			ios::icstream fp(argv[1]);
			string line;
			while (fp.gets(line))
			{
				string title = line;
				if (!fp.gets(line))
				{
					throw exception("Missing Data for '%s'", *title);
				}
				string data = line;
				if (!fp.gets(line))
				{
					throw exception("Cannot skip line after '%s'", *title);
				}
				std::cerr << title << std::endl;
				const swaps_ptr p = new swaps(title, data);
				if (Swaps.size() <= 0 || p->count > Swaps.back()->count)
				{
					Swaps.push_back(p);
				}
			}
		}
		std::cerr << "Found " << Swaps.size() << " swaps" << std::endl;
		hsort(Swaps, __compare_swaps_ptr);
		const unsigned count_max = Swaps.back()->count;
		const unsigned ns = unsigned(Swaps.size());
		{
			ios::ocstream fp(output);

			//__________________________________________________________________
			//
			// file prolog
			//__________________________________________________________________
			fp << "//! \\file\n";
			fp << "#ifndef Y_NWSRT_INCLUDED\n";
			fp << "#define Y_NWSRT_INCLUDED 1\n";
			fp << "#include \"y/type/bswap.hpp\"\n";

            
            //__________________________________________________________________
            //
            // macros
            //__________________________________________________________________
            fp << "//! swapping the pair I,J\n";
            fp << "#define Y_NWSRT_SWAP(I,J) { T &aI = a[I]; T &aJ = a[J]; if(aJ<aI) core::bswap< sizeof(T) >( &aI, &aJ ); }\n";
            fp << "//! swapping two pairs I,J\n";
            fp << "#define Y_NWSRT_SWP2(I,J) { const unsigned II = I; const unsigned JJ = J; T &aI = a[II]; T &aJ = a[JJ]; if(aJ<aI) { core::bswap< sizeof(T) >( &aI, &aJ ); core::bswap< sizeof(U) >( &b[II],&b[JJ]); } }\n";

            
			fp << "namespace upsylon {\n";

			{
				//______________________________________________________________
				//
				// nwsrt
				//______________________________________________________________
				fp << "\t//! network sorting\n";
				fp << "\tstruct nwsrt {\n";
				fp("\t\tstatic const size_t max_size=%u; //!< max handled case\n", count_max);

				// one array version
				{
					for (unsigned i = 1; i <= ns; ++i)
					{
						const unsigned count = Swaps[i]->count;
						fp("\t\t//!built-in version on %u items\n", count);
						fp("\t\ttemplate <typename T> static inline void on%u(T *a) {\n", count);
						fp << "\t\t\tassert(a);\n";
						const array<swap> &tests = Swaps[i]->tests;
                        write_indices(fp,tests);
                        fp("\t\t\tfor(size_t k=0;k<%u;++k) { Y_NWSRT_SWAP(I[k],J[k]); }\n", unsigned(tests.size()) );
						fp << "\t\t}\n";
					}

					//__________________________________________________________
					//
					// gather
					//__________________________________________________________
					fp("\t\t//!built-in version on 0-%u items\n", count_max);
					fp << "\t\ttemplate <typename T> static inline void on(T *a, const size_t n) {\n";

					fp << "\t\t\tswitch(n) {\n";
					for (size_t i = 1; i <= ns; ++i)
					{
						const unsigned count = Swaps[i]->count;
						fp("\t\t\t\tcase %2u: on%u(a); %sbreak;\n", count, count, (count < 10 ? " " : ""));
					}
					fp << "\t\t\t\tdefault: break;\n";
					fp << "\t\t\t}\n";

					fp << "\t\t}\n";
				}

				// two arrays version
				{
					for (unsigned i = 1; i <= ns; ++i)
					{
						const unsigned count = Swaps[i]->count;
						fp("\t\t//!built-in version on %u items\n", count);
						fp("\t\ttemplate <typename T,typename U> static inline void on%u(T *a, U *b) {\n", count);
						fp << "\t\t\tassert(a); assert(b);\n";
						const array<swap> &tests = Swaps[i]->tests;
                        write_indices(fp,tests);
                        fp("\t\t\tfor(size_t k=0;k<%u;++k) { Y_NWSRT_SWP2(I[k],J[k]); }\n", unsigned(tests.size()) );
                        fp << "\t\t}\n";
					}
                    
					//__________________________________________________________
					//
					// gather
					//__________________________________________________________
					fp("\t\t//!built-in version on 0-%u items\n", count_max);
					fp << "\t\ttemplate <typename T, typename U> static inline void on(T *a, U *b, const size_t n) {\n";

					fp << "\t\t\tswitch(n) {\n";
					for (size_t i = 1; i <= ns; ++i)
					{
						const unsigned count = Swaps[i]->count;
						fp("\t\t\t\tcase %2u: on%u(a,b); %sbreak;\n", count, count, (count < 10 ? " " : ""));
					}
					fp << "\t\t\t\tdefault: break;\n";
					fp << "\t\t\t}\n";

					fp << "\t\t}\n";
				}

				//______________________________________________________________
				//
				// end of nwsrt
				//______________________________________________________________
				fp << "\t};\n";
			}

			//__________________________________________________________________
			//
			// file epilog
			//__________________________________________________________________
			fp << "}\n";
			fp << "#endif\n";
		}


	}
}
Y_PROGRAM_END()

