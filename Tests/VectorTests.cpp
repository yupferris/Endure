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

TEST_CASE("Create empty int vector range checks", "[Vector]")
{
	auto v = CreateVector<int>();

	REQUIRE_THROWS_AS(v->Get(-1), std::out_of_range);
	REQUIRE_THROWS_AS(v->Get(0), std::out_of_range);
	REQUIRE_THROWS_AS(v->Get(1), std::out_of_range);
	REQUIRE_THROWS_AS(v->Get(1220), std::out_of_range);
}

TEST_CASE("Single item int vector range checks", "[Vector]")
{
	auto v = CreateVector<int>();
	auto v2 = Conj(v, 25);

	REQUIRE_THROWS_AS(v->Get(-1), std::out_of_range);
	REQUIRE_THROWS_AS(v->Get(0), std::out_of_range);
	REQUIRE_THROWS_AS(v->Get(1), std::out_of_range);
	REQUIRE_THROWS_AS(v->Get(1220), std::out_of_range);

	REQUIRE_THROWS_AS(v2->Get(-1), std::out_of_range);
	REQUIRE_THROWS_AS(v2->Get(1), std::out_of_range);
	REQUIRE_THROWS_AS(v2->Get(2), std::out_of_range);
	REQUIRE_THROWS_AS(v2->Get(1220), std::out_of_range);
}

TEST_CASE("32 item int vector range checks", "[Vector]")
{
	auto v = CreateVector<int>();
	for (int i = 0; i < 32; i++)
		v = v->Conj(i);

	REQUIRE_THROWS_AS(v->Get(-1233), std::out_of_range);
	REQUIRE_THROWS_AS(v->Get(-1), std::out_of_range);
	REQUIRE_THROWS_AS(v->Get(32), std::out_of_range);
	REQUIRE_THROWS_AS(v->Get(333), std::out_of_range);
}

TEST_CASE("Append 64 items to empty int vector", "[Vector]")
{
	auto v = CreateVector<int>();
	for (int i = 0; i < 64; i++)
		v = v->Conj(i);

	REQUIRE(v->Count() == 64);
	for (int i = 0; i < 64; i++)
		REQUIRE(v->Get(i) == i);
}

TEST_CASE("Append 50000 items to empty int vector", "[Vector]")
{
	auto v = CreateVector<int>();
	for (int i = 0; i < 50000; i++)
		v = v->Conj(i);

	REQUIRE(v->Count() == 50000);
	for (int i = 0; i < 50000; i++)
		REQUIRE(v->Get(i) == i);
}

TEST_CASE("Change 50000 items in int vector", "[Vector]")
{
	auto v = CreateVector<int>();
	for (int i = 0; i < 50000; i++)
		v = Conj(v, i);

	for (int i = 0; i < 50000; i++)
		v = v->Assoc(i, 50000 - i);

	REQUIRE(v->Count() == 50000);
	for (int i = 0; i < 50000; i++)
		REQUIRE(v->Get(i) == 50000 - i);
}

TEST_CASE("Change 16 items in small (tail-only) int vector", "[Vector]")
{
	auto v = CreateVector<int>();
	for (int i = 0; i < 32; i++)
		v = Conj(v, i);

	for (int i = 0; i < 16; i++)
		v = Assoc(v, i, 16 - i);

	REQUIRE(v->Count() == 32);
	for (int i = 0; i < 32; i++)
		REQUIRE(v->Get(i) == (i < 16 ? 16 - i : i));
}

TEST_CASE("Assoc to empty int vector", "[Vector]")
{
	auto v = CreateVector<int>();
	v = Assoc(v, 0, 26);

	REQUIRE(v->Count() == 1);
	REQUIRE(v->Get(0) == 26);
}

TEST_CASE("Assoc int vector range checks", "[Vector]")
{
	auto v = CreateVector<int>();
	for (int i = 0; i < 10; i++)
		v = v->Conj(10000 + i);

	REQUIRE_THROWS_AS(v->Assoc(-1, 1337), std::out_of_range);
	REQUIRE_THROWS_AS(v->Assoc(11, 1337), std::out_of_range);
	REQUIRE_THROWS_AS(v->Assoc(12, 1337), std::out_of_range);
	REQUIRE_THROWS_AS(v->Assoc(1220, 1337), std::out_of_range);
}

TEST_CASE("Pop empty int vector", "[Vector]")
{
	REQUIRE_THROWS_AS(CreateVector<int>()->Pop(), std::logic_error);
}

TEST_CASE("Pop single item int vector", "[Vector]")
{
	auto v = CreateVector<int>();
	v = Conj(v, 10);
	v = Pop(v);

	REQUIRE(!v->Count());
}

TEST_CASE("Pop small int vector", "[Vector]")
{
	auto v = CreateVector<int>();
	for (int i = 0; i < 32; i++)
		v = Conj(v, i);

	for (int i = 0; i < 16; i++)
		v = Pop(v);

	REQUIRE(v->Count() == 16);
	for (int i = 0; i < 16; i++)
		REQUIRE(v->Get(i) == i);
}
