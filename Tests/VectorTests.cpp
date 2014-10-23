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
