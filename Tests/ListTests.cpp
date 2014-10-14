#include <Endure.h>
#include "Catch/single_include/catch.hpp"

#include <memory>

using namespace Endure;
using namespace std;

namespace Endure
{
	template <typename T> class _List;
	template <typename T> class _List
	{
	public:
		static shared_ptr<_List<T>> Cons(T item, shared_ptr<_List<T>> list)
		{
			return shared_ptr<_List<T>>(new _List<T>(item, list));
		}

		const T Head;
		const shared_ptr<_List<T>> Tail;
		const int Count;

		_List(T head)
			: Head(head), Tail(nullptr), Count(1)
		{
		}

		_List(T head, shared_ptr<_List<T>> tail)
			: Head(head), Tail(tail), Count(tail.get()->Count + 1)
		{
		}
	};

	// TODO: Upgrade VS and use this everywhere
	//template <typename T> using List = shared_ptr<_List<T>>;
}

TEST_CASE("Create single element int list", "[List]")
{
	auto l = shared_ptr<_List<int>>(new _List<int>(6));

	REQUIRE(l->Head == 6);
	REQUIRE(!l->Tail);
	REQUIRE(l->Count == 1);
}

TEST_CASE("Create two element int list", "[List]")
{
	auto tail = shared_ptr<_List<int>>(new _List<int>(29));
	auto l = shared_ptr<_List<int>>(new _List<int>(45, tail));

	REQUIRE(l->Head == 45);
	REQUIRE(l->Tail == tail);
	REQUIRE(l->Count == 2);

	shared_ptr<_List<int>> t = l->Tail;
	REQUIRE(t->Head == 29);
	REQUIRE(!t->Tail);
	REQUIRE(t->Count == 1);
}

TEST_CASE("Cons to single element int list", "[List]")
{
	auto l = shared_ptr<_List<int>>(new _List<int>(2000));
	auto l2 = _List<int>::Cons(1984, l);

	REQUIRE(l2->Head == 1984);
	REQUIRE(l2->Tail == l);
	REQUIRE(l2->Count == 2);

	REQUIRE(l2->Tail->Head == 2000);
	REQUIRE(!l2->Tail->Tail);
	REQUIRE(l2->Tail->Count == 1);
}

TEST_CASE("Simple int list cons stress test", "[List")
{
	auto l = shared_ptr<_List<int>>(new _List<int>(0));
	for (int i = 1; i < 10000; i++)
		l = _List<int>::Cons(i, l);

	REQUIRE(l->Head == 9999);
	REQUIRE(l->Tail->Count == 9999);
	REQUIRE(l->Count == 10000);
}
