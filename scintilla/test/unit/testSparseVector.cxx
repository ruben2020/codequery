// Unit Tests for Scintilla internal data structures

#include <string.h>

#include <cassert>

#include <stdexcept>
#include <algorithm>

#include "Platform.h"

#include "Position.h"
#include "SplitVector.h"
#include "Partitioning.h"
#include "SparseVector.h"

#include "catch.hpp"

// Test SparseVector.

// Helper to produce a string representation of a SparseVector<const char *>
// to simplify checks.
static std::string Representation(const SparseVector<const char *> &st) {
	std::string ret;
	for (int i = 0;i < st.Length();i++) {
		const char *value = st.ValueAt(i);
		if (value)
			ret += value;
		else
			ret += "-";
	}
	return ret;
}

TEST_CASE("SparseVector") {

	SparseVector<const char *> st;

	SECTION("IsEmptyInitially") {
		REQUIRE(1 == st.Elements());
		REQUIRE(0 == st.Length());
		st.Check();
	}

	SECTION("InsertSpace") {
		st.InsertSpace(0, 5);
		REQUIRE(1 == st.Elements());
		REQUIRE(static_cast<const char *>(NULL) == st.ValueAt(0));
		REQUIRE(static_cast<const char *>(NULL) == st.ValueAt(1));
		REQUIRE(static_cast<const char *>(NULL) == st.ValueAt(4));
		st.Check();
	}

	SECTION("InsertValue") {
		st.InsertSpace(0, 5);
		st.SetValueAt(3, "3");
		REQUIRE(2 == st.Elements());
		REQUIRE("---3-" == Representation(st));
		st.Check();
	}

	SECTION("InsertAndDeleteValue") {
		st.InsertSpace(0, 5);
		REQUIRE(5 == st.Length());
		st.SetValueAt(3, "3");
		REQUIRE(2 == st.Elements());
		st.DeletePosition(3);
		REQUIRE(1 == st.Elements());
		REQUIRE(4 == st.Length());
		REQUIRE("----" == Representation(st));
		st.Check();
	}

	SECTION("InsertAndDeleteAtStart") {
		REQUIRE(1 == st.Elements());
		st.InsertSpace(0, 5);
		st.SetValueAt(0, "3");
		REQUIRE(1 == st.Elements());
		REQUIRE("3----" == Representation(st));
		st.DeletePosition(0);
		REQUIRE(1 == st.Elements());
		REQUIRE("----" == Representation(st));
		st.SetValueAt(0, "4");
		REQUIRE(1 == st.Elements());
		REQUIRE("4---" == Representation(st));
		st.DeletePosition(0);
		REQUIRE(1 == st.Elements());
		REQUIRE("---" == Representation(st));
		st.Check();
	}

	SECTION("InsertStringAtStartThenInsertSpaceAtStart") {
		REQUIRE(1 == st.Elements());
		st.InsertSpace(0, 5);
		st.SetValueAt(0, "3");
		REQUIRE(1 == st.Elements());
		REQUIRE("3----" == Representation(st));
		st.InsertSpace(0, 1);
		REQUIRE(2 == st.Elements());
		REQUIRE("-3----" == Representation(st));
		st.Check();
	}

	SECTION("InsertAndDeleteAtEnd") {
		REQUIRE(1 == st.Elements());
		st.InsertSpace(0, 5);
		st.SetValueAt(4, "5");
		REQUIRE(2 == st.Elements());
		REQUIRE("----5" == Representation(st));
		st.DeletePosition(4);
		REQUIRE(1 == st.Elements());
		REQUIRE("----" == Representation(st));
		st.Check();
	}

	SECTION("SetNULL") {
		REQUIRE(1 == st.Elements());
		st.InsertSpace(0, 5);
		st.SetValueAt(4, "5");
		REQUIRE(2 == st.Elements());
		REQUIRE("----5" == Representation(st));
		st.SetValueAt(4, NULL);
		REQUIRE(1 == st.Elements());
		REQUIRE("-----" == Representation(st));
		st.Check();
	}

	SECTION("DeleteAll") {
		REQUIRE(1 == st.Elements());
		st.InsertSpace(0, 10);
		st.SetValueAt(9, "9");
		st.SetValueAt(7, "7");
		st.SetValueAt(4, "4");
		st.SetValueAt(3, "3");
		REQUIRE(5 == st.Elements());
		REQUIRE("---34--7-9" == Representation(st));
		st.Check();
	}

}

TEST_CASE("SparseTextInt") {

	SparseVector<int> st;

	SECTION("InsertAndDeleteValue") {
		st.InsertSpace(0, 5);
		st.SetValueAt(3, 3);
		REQUIRE(2 == st.Elements());
		REQUIRE(0 == st.ValueAt(0));
		REQUIRE(0 == st.ValueAt(1));
		REQUIRE(0 == st.ValueAt(2));
		REQUIRE(3 == st.ValueAt(3));
		REQUIRE(0 == st.ValueAt(4));
		st.SetValueAt(3, -3);
		REQUIRE(2 == st.Elements());
		REQUIRE(0 == st.ValueAt(0));
		REQUIRE(0 == st.ValueAt(1));
		REQUIRE(0 == st.ValueAt(2));
		REQUIRE(-3 == st.ValueAt(3));
		REQUIRE(0 == st.ValueAt(4));
		st.SetValueAt(3, 0);
		REQUIRE(1 == st.Elements());
		REQUIRE(0 == st.ValueAt(0));
		REQUIRE(0 == st.ValueAt(1));
		REQUIRE(0 == st.ValueAt(2));
		REQUIRE(0 == st.ValueAt(3));
		REQUIRE(0 == st.ValueAt(4));
		st.Check();
	}
}

TEST_CASE("SparseTextString") {

	SparseVector<std::string> st;

	SECTION("InsertAndDeleteValue") {
		st.InsertSpace(0, 5);
		REQUIRE(5 == st.Length());
		st.SetValueAt(3, "3");
		REQUIRE(2 == st.Elements());
		REQUIRE("" == st.ValueAt(0));
		REQUIRE("" == st.ValueAt(2));
		REQUIRE("3" == st.ValueAt(3));
		REQUIRE("" == st.ValueAt(4));
		st.DeletePosition(0);
		REQUIRE(4 == st.Length());
		REQUIRE("3" == st.ValueAt(2));
		st.DeletePosition(2);
		REQUIRE(1 == st.Elements());
		REQUIRE(3 == st.Length());
		REQUIRE("" == st.ValueAt(0));
		REQUIRE("" == st.ValueAt(1));
		REQUIRE("" == st.ValueAt(2));
		st.Check();
	}
}
