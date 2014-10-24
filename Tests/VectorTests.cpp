#include <Endure.h>
#include "Catch/single_include/catch.hpp"

using namespace Endure;

TEST_CASE("Create empty int vector", "[Vector]")
{
	auto v = CreateVector<int>();

	REQUIRE(!v->Count());
}

TEST_CASE("Append single item to empty int vector", "[Vector]")
{
	auto v = CreateVector<int>();
	auto v2 = Conj(v, 25);

	REQUIRE(!v->Count());
	REQUIRE(v2->Count() == 1);
	REQUIRE(Get(v2, 0) == 25);
}

TEST_CASE("Append single item to single int vector", "[Vector]")
{
	auto v = CreateVector<int>();
	auto v2 = Conj(v, 26);
	auto v3 = Conj(v2, -28);

	REQUIRE(!v->Count());
	REQUIRE(v2->Count() == 1);
	REQUIRE(Get(v2, 0) == 26);
	REQUIRE(v3->Count() == 2);
	REQUIRE(Get(v3, 0) == 26);
	REQUIRE(Get(v3, 1) == -28);
}

TEST_CASE("Append 32 items to empty int vector", "[Vector]")
{
	auto v = CreateVector<int>();
	for (int i = 0; i < 32; i++)
		v = v->Conj(i);

	REQUIRE(v->Count() == 32);
	for (int i = 0; i < 32; i++)
		REQUIRE(v->Get(i) == i);
}
