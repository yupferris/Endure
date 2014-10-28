#include <Endure.h>
#include "Catch/single_include/catch.hpp"

using namespace Endure;

TEST_CASE("Create empty float list", "[List]")
{
	auto l = CreateList<float>();

	REQUIRE_THROWS_AS(l->Head(), std::logic_error);
	REQUIRE_THROWS_AS(l->Tail(), std::logic_error);
	REQUIRE(!l->Count());
}

TEST_CASE("Create single element int list", "[List]")
{
	auto l = CreateList(6);

	REQUIRE(l->Head() == 6);
	REQUIRE(!l->Tail()->Count());
	REQUIRE(l->Count() == 1);
}

TEST_CASE("Create two element int list", "[List]")
{
	auto tail = CreateList(29);
	auto l = Conj(tail, 45);

	REQUIRE(l->Head() == 45);
	REQUIRE(l->Tail() == tail);
	REQUIRE(l->Count() == 2);

	List<int> t = l->Tail();
	REQUIRE(t->Head() == 29);
	REQUIRE(!t->Tail()->Count());
	REQUIRE(t->Count() == 1);
}

TEST_CASE("Cons to single element int list", "[List]")
{
	auto l = CreateList(2000);
	auto l2 = Conj(l, 1984);

	REQUIRE(l2->Head() == 1984);
	REQUIRE(l2->Tail() == l);
	REQUIRE(l2->Count() == 2);

	REQUIRE(l2->Tail()->Head() == 2000);
	REQUIRE(!l2->Tail()->Tail()->Count());
	REQUIRE(l2->Tail()->Count() == 1);
}

TEST_CASE("Simple int list cons stress test", "[List]")
{
	auto l = CreateList(0);
	for (int i = 1; i < 100; i++)
		l = Conj(l, i);

	REQUIRE(l->Head() == 99);
	REQUIRE(l->Tail()->Count() == 99);
	REQUIRE(l->Count() == 100);
}
