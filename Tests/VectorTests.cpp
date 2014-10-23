#include <Endure.h>
#include "Catch/single_include/catch.hpp"

using namespace Endure;

TEST_CASE("Create empty int vector", "[Vector]")
{
	auto v = CreateVector<int>();

	REQUIRE(!v->Count);
}
