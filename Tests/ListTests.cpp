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
	auto l = Conj(45, tail);

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
	auto l2 = Conj(1984, l);

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
	for (int i = 1; i < 10000; i++)
		l = Conj(i, l);

	REQUIRE(l->Head() == 9999);
	REQUIRE(l->Tail()->Count() == 9999);
	REQUIRE(l->Count() == 10000);
}
