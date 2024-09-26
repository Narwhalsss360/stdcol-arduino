#include <stdcol.h>
#include "TestPlatform.h"
#include "TestFunctionWrapper.h"
#include "TestObjects.h"
#include "TestUtilities.h"

//stl hash<>
#if defined(plat_windows)
#include <xhash>
#elif defined(plat_unix)
#include <system_error>
#endif

constexpr char flog_name[] = "Test Results.log";

#ifndef plat_arduino
std::ofstream flog = std::ofstream(flog_name);
#endif
Platform::stringstream tlog;

TesterFunction tests[] = {
	{
		"[Runtime] TesterFunction Test",
		[](TesterFunction& this_test) {
			tlog << this_test << " Ran test for TesterFunction!" << '\n';
			return test_pass;
		}
	},
	{
		"[Runtime] TesterFunction exception catcher test",
		[](TesterFunction& this_test) {
			this_test.result = true;
#ifndef plat_arduino
			throw test_exception();
#endif
			return test_pass;
		}
	},
	{
		"[Compile] index",
		[](TesterFunction& this_test) {
			using stdcol::index;
			using stdcol::findex;

			findex fi = static_cast<findex::int_type>(index(32));
			for (index i = 0; i < 10; i++, fi = i * 2 + 1);

			fi = findex();
			fail_if(fi.found());

			return test_pass;
		}
	},
	{
		"[Compile | Runtime] HEADER: iterables",
		[](TesterFunction& this_test) {
			using stdcol::general_iterator;
			using stdcol::iterable;
			using stdcol::iterate;

			{
				tlog << "Using general_iterator... ";
				for (
					auto fname_begin = general_iterator<const char[sizeof(flog_name)], const char&>(::flog_name, 0),
					fname_end = general_iterator<const char[sizeof(flog_name)], const char&>(flog_name, sizeof(flog_name));
					fname_begin != fname_end;
					++fname_begin) {
						tlog << *fname_begin;
					}
					tlog << '\n';
			}

			{
				tlog << "Using iterable... ";
				auto fname_iter = iterable<const char*>(static_cast<const char*>(flog_name), flog_name + sizeof(flog_name));
				for (const char& c : fname_iter) {
					tlog << c;
				}
				tlog << '\n';
			}

			{
				tlog << "Using iterate(T (&)[size])... ";
				for (const char& c : iterate(flog_name)) {
					tlog << c;
				}
				tlog << '\n';
			}

			return test_pass;
		}
	},
	{
		"[Compile | Runtime] Header: enumerables",
		[](TesterFunction& this_test) {
			using stdcol::general_iterator;
			using stdcol::iterate;
			using stdcol::enumerator;
			using stdcol::enumerable;
			using stdcol::enumerate;

			{
				tlog << "Using enumerator... ";
				for (
					auto fname_begin = enumerator<general_iterator<const char[sizeof(flog_name)], const char&>, const char&>(general_iterator<const char[sizeof(flog_name)], const char&>(flog_name, 0), 0),
					fname_end = enumerator<general_iterator<const char[sizeof(flog_name)], const char&>, const char&>(general_iterator<const char[sizeof(flog_name)], const char&>(flog_name, sizeof(flog_name)), sizeof(flog_name));
					fname_begin != fname_end;
					++fname_begin) {
						tlog << '(' << (*fname_begin).index << ", " << *fname_begin << ") ";
					}
					tlog << '\n';
			}

			{
				tlog << "Using enumerable... ";
				auto fname_enum = iterate(enumerable<const char*, const char&>(static_cast<const char*>(flog_name), flog_name + sizeof(flog_name)));
				for (auto e : fname_enum) {
					tlog << '(' << e.index << ", " << e << ") ";
				}
				tlog << '\n';
			}

			{
				tlog << "Using enumerate(T (&)[size])... ";
				for (auto e : iterate(enumerate(flog_name))) {
					tlog << '(' << e.index << ", " << e << ") ";
				}
				tlog << '\n';
			}
			return test_pass;
		}
	},
	{
		"[Compile | Runtime] array & collection",
		[](TesterFunction& this_test) {
			using stdcol::collection;
			using stdcol::array;
			using stdcol::enumerate;

			array<float, 10> numArray;
			collection<float>& nums = numArray;
			const array<float, 10>& cnumArray = numArray;
			auto& c_array = numArray.c_array();
			const auto& cc_carray = cnumArray.c_array();

			for (float& n : nums) {
				n = 1.0/2.0;
			}

			tlog << "Initial: ";
			for (auto e : enumerate(nums)) {
				tlog << '(' << e.index << ", " << e << ") ";
			}
			tlog << '\n';

			for (auto e : enumerate(nums)) {
				static_cast<float&>(e) = e.index / 2.0f;
			}

			tlog << "Index-based: ";
			for (auto e : enumerate(nums)) {
				tlog << '(' << e.index << ", " << e << ") ";
			}
			tlog << '\n';

			return test_pass;
		}
	},
	{
		"[Compile | Runtime] dynamic_array & dynamic_collection",
		[](TesterFunction& this_test) {
			using stdcol::array;
			using stdcol::collection;
			using stdcol::dynamic_collection;
			using stdcol::dynamic_array;
			using stdcol::enumerate;
			using namespace stdcol::operators;

			dynamic_array<array<double, 2>> pairsArray = dynamic_array<array<double, 2>>(5);
			const dynamic_array<array<double, 2>>& cpairsArray = pairsArray;
			dynamic_collection<array<double, 2>>& pairs = pairsArray;
			auto& c_array = pairsArray.c_array<5>();
			const auto& cc_array = pairsArray.c_array<5>();

			auto f = [](double x) { return (x * x * x) - 2.5 * x; };

			constexpr double
				x_start = -3,
				x_step = 1,
				x_stop = 3;

			for (double x = x_start; x <= x_stop; x += x_step) {
				array<double, 2> pair;
				pair[0] = x;
				pair[1] = f(x);
				pairs +=  pair;
			}

			tlog << '[';
			for (auto e : enumerate(pairs)) {
				tlog << '[' << e.index << "]: (" << e.value[0] << ", " << e.value[1] << ')';
				if (e.index != pairs.size() - 1) {
					tlog << ", ";
				}
			}
			tlog << "]\n";

			dynamic_array<array<double, 2>> pairsCopy = pairsArray;

			pairs.resize(0);
			tlog << "Capacity: " << pairs.capacity() << '\n';
			return test_pass;
		}
	},
	{
		"[Complile | Runtime] wrapper",
		[](TesterFunction& this_test)
		{
			using stdcol::wrapper;
			using stdcol::wrap;
			using stdcol::size;
			char local_flog_name[size(flog_name)];
			for (int i = 0; i < size(flog_name); i++) {
				local_flog_name[i] = flog_name[i];
			}

			auto flog_name_wrapper = wrap(flog_name);
			auto local_flog_name_wrapper = wrap(local_flog_name);
			char* local_flog_name_ptr = local_flog_name;
			int ssize = size(flog_name) - 4;
			auto resize_local_flog_name_wrapper = wrap(local_flog_name_ptr, ssize);

			tlog << "flog_name_wrapper" << flog_name_wrapper << '\n';
			tlog << "local_flog_name_wrapper" << local_flog_name_wrapper << '\n';
			tlog << "resize_local_flog_name_wrapper" << resize_local_flog_name_wrapper << '\n';

			return test_pass;
		}
	},
	{
		"[Compile] init_list",
		[](TesterFunction& this_test) {
			using stdcol::enumerate;
			using stdcol::initializer_list;
			using stdcol::array;
			using stdcol::dynamic_array;
			using namespace stdcol::operators;

			initializer_list<int> il = { 1, 2, 3 };

			tlog << "Init list: ";
			for (auto e : enumerate(il)) {
				tlog << '[' << e.index << "]:" << e << ' ';
			}
			tlog << '\n';

			array<int, 3> ints =  { 4, 5, 6 };

			ints = { 7, 8, 9 };

			auto f = [](double x) { return (x * x * x) - 2.5 * x; };

			dynamic_array<array<double, 2>> pairs = {
				{ 0, f(0) },
				{ 1, f(1) },
				{ 2, f(2) },
				{ 3, f(3) },
				{ 4, f(4) }
			};

			constexpr double extra = 4.3;
			constexpr double step = 0.5;

			const double end = pairs[pairs.size() - 1][0] + extra;
			for (double x = pairs[pairs.size() - 1][0]; x <= end; x += step) {
				pairs +=  { x, f(x) };
			}
			return test_pass;
		}
	},
	{
		"[Compile | Runtime] linked_node",
		[](TesterFunction& this_test) {
			using stdcol::linked_node;
			auto n = linked_node<NonDefaultConstructable>(nullptr, nullptr, 32, 1);
			return test_pass;
		}
	},
	{
		"linked_list",
		[](TesterFunction& this_test) {
			using stdcol::index;
			using stdcol::linked;
			linked<int> ints;

			for (int n = 1, i = 0; n <= 45; n += 2, i++) {
				ints.emplace(i, n);
			}

			ints.emplace(2, 6);
			ints.emplace(5, 10);

			linked<int> copy = ints;

			return test_pass;
		}
	},
	{
		"linked_iterator",
		[](TesterFunction& this_test) {
			using stdcol::index;
			using stdcol::array;
			using stdcol::linked;
			using stdcol::linked_node;
			using stdcol::linked_iterator;
			using stdcol::initializer_list;
			using stdcol::iterate;
			using stdcol::enumerate;

			linked<NonDefaultConstructable> items;

			index i = 0;
			for (int n = 2; n <= 1000; n += n, i++) {
				items.emplace(i, static_cast<unsigned int>(2 * i), n);
			}

			for (auto& n : iterate(items)) {
				tlog << n.a << ':' << n.b << ", ";
			}

			tlog << '\n';

			index size = items.size();
			for (auto e : enumerate(items)) {
				tlog << '[' << e.index << "]: (" << e.value.a << ", " << e.value.b << ')';
				if (e.index != size - 1) {
					tlog << ", ";
				}
			}

			tlog << '\n';
			return test_pass;
		}
	},
	{
		"set",
		[](TesterFunction& this_test)
		{
			using stdcol::set;
			using namespace stdcol::operators;

			set<unsigned int> some_naturals;

			some_naturals.reserve(120);
			for (int i = 0; i <= 100; i++)
				some_naturals += (unsigned int)i;
            some_naturals.reserve(1);
			return test_pass;
		}
	},
	{
		"queue",
		[](TesterFunction& this_test)
		{
			using stdcol::queue;

			const int nums[] = { 2, 4, 6, 8, 10 };

			queue<int> line;

			for (int i = 0; i < sizeof(nums) / sizeof(int); i++)
			{
				if (!(line += nums[i]))
					return test_fail;
			}

			//return test_fail; //infinite-loop here.
			while (line.size())
			{
				int num;
				if (line -= num)
				{
					tlog << num << '\n';
					continue;
				}
				return test_fail;
			}

			return test_pass;
		}
	},
	{
		"hash_table",
		[](TesterFunction& this_test)
		{
			using stdcol::dictionary;
			using stdcol::hash_table;

			//return test_fail; //dynamic_array<linked<kvp_t>> destructor does a double free
			auto hashtable = hash_table<int, int, stdcol::hasher<int>>(4);
			dictionary<int, int>& fsamples = hashtable;

			auto f = [](int x) { return ((x * x * x) / 6) - (2 * x); };

			for (int x = -5; x <= 5; x++) {
				fsamples.add(x, f(x));
			}

			if (!fsamples.contains(5)) {
				return test_fail;
			}

			for (int x = 10; x <= 20; x++) {
				fsamples[x] = f(x);
			}

			dictionary<int, int>::buckets_t buckets = fsamples.buckets();

			for (dictionary<int, int>::bucket_t& bucket : buckets) {
				tlog << "---\n";
				for (dictionary<int, int>::kvp_t& kvp : bucket) {
					tlog << "    " << kvp.key << ',' << kvp.value << "\n";
				}
			}

			return test_pass;
		}
	},
	{
		"stack",
		[](TesterFunction& this_test)
		{
			using stdcol::stack;

			stack<int> intStack;

			for (int i = 5; i <= 50; i += 5) {
				if (!(intStack += i)) {
					tlog << "Failed to push\n";
					return test_fail;
				}
			}

			while (intStack.size()) {
				int out;
				if (!(intStack -= out)) {
					tlog << "Failed to pop\n";
					return test_fail;
				}
				tlog << out << ' ';
			}
			tlog << '\n';

			return test_pass;
		}
	},
	{
		"binary_tree",
		[](TesterFunction& this_test)
		{
			using stdcol::binary_tree;
			using stdcol::binary_tree_node;
			using stdcol::dynamic_array;
			using stdcol::rotations;

			binary_tree<int> tree;

			tree.set_root(tree.emplace(1)->balance_ancestors());
			tree.set_root(tree.emplace(2)->balance_ancestors());
			tree.set_root(tree.emplace(3)->balance_ancestors());
			tree.set_root(tree.emplace(4)->balance_ancestors());
			tree.set_root(tree.emplace(5)->balance_ancestors());
			tree.set_root(tree.emplace(6)->balance_ancestors());
			tree.set_root(tree.emplace(7)->balance_ancestors());

			binary_tree_node<int>* root = tree.root();
			tlog << "The height of the tree is " << root->height() << '\n';

			binary_tree_node<int>* node_5 = tree.find(5);
			return test_pass;
		}
	},
	{
		"binary_tree traversals",
		[](TesterFunction& this_test)
		{
			using stdcol::stack;
			using stdcol::set;
			using stdcol::binary_tree;
			using stdcol::binary_tree_node;
			using namespace stdcol::tree_traversals;

			binary_tree<int> tree;

			tree.emplace(4);
			tree.emplace(2);
			tree.emplace(6);
			tree.emplace(1);
			tree.emplace(3);
			tree.emplace(5);
			tree.emplace(7);

			for (auto i : tree) {
				tlog << i;
			}
			tlog << '\n';

			for (auto i : tree.pre_order()) {
				tlog << i;
			}
			tlog << '\n';

			for (auto i : tree.post_order()) {
				tlog << i;
			}
			tlog << '\n';

			return test_pass;
		}
	},
	{
		"priority_queue",
		[](TesterFunction& this_test)
		{
			using stdcol::priority_queue;

			priority_queue<int> queue;
			queue += { 0, 1 };
			queue += { 0, 2 };
			queue += { 0, 3 };
			queue += { 0, 4 };
			queue += { 0, 5 };
			queue += { 1, 6 };
			queue += { 1, 7 };
			queue += { 1, 8 };
			queue += { 1, 9 };
			queue += { 1, 10 };

			while (queue.size()) {
				int item;
				queue -= item;
				tlog << item << ' ';
			}
			tlog << '\n';

			return test_pass;
		}
	},
	{
		"binary_tree_rotations",
		[](TesterFunction& this_test)
		{
			using stdcol::binary_tree;
			using stdcol::rotations;

			binary_tree<int> tree;

			tree.emplace(4);
			tree.emplace(3);
			tree.emplace(2);
			tree.rotate(tree.root(), rotations::left_right);

			tree.remove(tree.root());
			tree.emplace(4);
			tree.emplace(6);
			tree.emplace(5);
			tree.rotate(tree.root(), rotations::right_left);

			return test_pass;
		}
	},
	{
		"tree",
		[](TesterFunction& this_test)
		{
			using stdcol::tree;
			using stdcol::tree_node;
			using namespace stdcol::operators;
			tree<int> int_tree;

			/*
					1
				2		3
			  4   5
			*/

			tree_node<int>* one = new tree_node<int>(nullptr, 1);

			tree_node<int>* two = new tree_node<int>(one, 2);
			one->links() += two;

			tree_node<int>* three = new tree_node<int>(one, 3);
			one->links() += three;

			tree_node<int>* four = new tree_node<int>(two, 4);
			two->links() += four;

			tree_node<int>* five = new tree_node<int>(two, 5);
			two->links()  += five;

			int_tree.set_root(one);

			return test_pass;
		}
	},
    {
        "directed_graph",
        [](TesterFunction& this_test)
        {
            using stdcol::directed_graph;
            using stdcol::graph_node;

            directed_graph<int> graph;

            graph_node<int>* n1 = graph.emplace(1);
            if (n1 == nullptr) {
                return test_fail;
            }

            graph_node<int>* n2 = graph.emplace(2);
            if (n2 == nullptr) {
                return test_fail;
            }

            if (!n1->add_edge(n2)) {
                return test_fail;
            }

            if (!graph.add_edge(2, 1)) {
                return test_fail;
            }

            tlog << n1->get_edges() << '\n';

            return graph.get(1) == n1 && graph.get(2) == n2;
        }
    },
	{
		"avl_tree",
		[](TesterFunction& this_test)
		{
			using stdcol::avl_tree;

			avl_tree<int> tree;
			for (int i = 1; i <= 7; i++) {
				tree.emplace(i);
			}

			return test_pass;
		}
	},
	{
		"collection_operators",
		[](TesterFunction& this_test)
		{
			using stdcol::dynamic_array;
            using stdcol::at;
			using namespace stdcol::operators;

			dynamic_array<int> ints;

			for (int i = 0; i < 5; i++) {
				ints += i;
			}

			tlog << "Loaded ints:";
			for (int i = 0; i < ints.size(); i++) {
				tlog << *at(ints, i) << ' ';
			}
			tlog << '\n';

			ints -= 2;
			for (int i = 0; i < ints.size(); i++) {
				tlog << ints[i] << ' ';
			}
			tlog << '\n';

			return test_pass;
		}
	},
    {
        "Begin and end functions",
        [](TesterFunction& this_test)
        {
            using stdcol::begin;
            using stdcol::end;
            using stdcol::dynamic_array;
            using stdcol::linked;
            using namespace stdcol::operators;

            dynamic_array<int> ints;
            for (int i = 0; i < 5; i++) {
                ints += i * 2;
            }

            tlog << "2 * index: ";
            for (auto __begin = begin(ints), __end = end(ints);
                __begin != __end;
                ++__begin) {
                tlog << *__begin << ' ';
            }

            linked<int> linked_ints;
            for (const int& n : ints) {
                linked_ints += n;
            }

            tlog << "\n2 * index: ";
            for (auto __begin = begin(linked_ints), __end = end(linked_ints);
                __begin != __end;
                ++__begin) {
                tlog << *__begin << ' ';
            }

            tlog << '\n';
            return test_pass;
        }
    }
};

entry_symbol {
    Platform::setup();
    tlog << "Running UnitTest on platform " << plat << '\n';
    tlog << "stdcol_platform_name: " << stdcol_platform_name << '\n';

	for (TesterFunction& test : tests) {
#ifdef plat_arduino
		tlog << "Running test " << test.name << ":\n";
		tlog << test << ": " << resulString[test()] << '\n';
#else
		try {
			tlog << "Running test " << test.name << ":\n";
			tlog << test << ": " << resulString[test()] << '\n';
		} catch (const std::exception& e) {
			tlog << "An exception was thrown in test " << test.name << ", " << e.what() << '\n';
		}
		flog << tlog.str();
#endif
		Platform::out << tlog.str();
		tlog.str("");
	}

	Platform::out << "Press enter to exit...\n";
	Platform::keywait();
    entry_return;
}

#ifdef plat_arduino
void loop() {}
#endif
