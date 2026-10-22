#include <gtest/gtest.h>
#include <sstream>
#include <utility>
#include "geo_coordinate.h"
#include "geo_set.h"

TEST(GeoCoordinateTest, ValidBoundaries) {
    EXPECT_TRUE((GeoCoordinate(90.0, 180.0)).is_valid());
    EXPECT_TRUE((GeoCoordinate(-90.0, -180.0)).is_valid());
    EXPECT_TRUE((GeoCoordinate(0.0, 0.0)).is_valid());
}

TEST(GeoCoordinateTest, InvalidLatitude) {
    EXPECT_FALSE((GeoCoordinate(90.0001, 0.0)).is_valid());
    EXPECT_FALSE((GeoCoordinate(-91.0, 0.0)).is_valid());
}

TEST(GeoCoordinateTest, InvalidLongitude) {
    EXPECT_FALSE((GeoCoordinate(0.0, 180.0001)).is_valid());
    EXPECT_FALSE((GeoCoordinate(0.0, -200.0)).is_valid());
}

TEST(GeoCoordinateTest, EqualityAndOrdering) {
    GeoCoordinate a(10.0, 20.0);
    GeoCoordinate b(10.0, 20.0);
    GeoCoordinate c(10.0, 30.0);
    GeoCoordinate d(15.0, 0.0);

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_FALSE(a < b);
    EXPECT_TRUE(a < c);
    EXPECT_TRUE(a < d);
    EXPECT_FALSE(d < a);
}

TEST(GeoCoordinateTest, StreamOutput) {
    GeoCoordinate c(12.5, -34.25);
    std::ostringstream os;
    os << c;
    EXPECT_EQ(os.str(), "(12.5, -34.25)");
}

TEST(GeoCoordinateSetTest, EmptySet) {
    GeoCoordinateSet s;
    EXPECT_EQ(s.size(), 0u);
    EXPECT_TRUE(s.empty());
    EXPECT_FALSE(s.contains(GeoCoordinate(0.0, 0.0)));
    EXPECT_FALSE(s(GeoCoordinate(1.0, 1.0)));
}

TEST(GeoCoordinateSetTest, AddNewAndDuplicate) {
    GeoCoordinateSet s;
    EXPECT_TRUE(s.add(GeoCoordinate(10.0, 20.0)));
    EXPECT_TRUE(s.add(GeoCoordinate(0.0, 0.0)));
    EXPECT_FALSE(s.add(GeoCoordinate(10.0, 20.0)));
    EXPECT_EQ(s.size(), 2u);
}

TEST(GeoCoordinateSetTest, AddInvalidRejected) {
    GeoCoordinateSet s;
    EXPECT_FALSE(s.add(GeoCoordinate(100.0, 0.0)));
    EXPECT_FALSE(s.add(GeoCoordinate(0.0, 200.0)));
    EXPECT_EQ(s.size(), 0u);
    EXPECT_TRUE(s.empty());
}

TEST(GeoCoordinateSetTest, ContainsAndCallOperatorAgree) {
    GeoCoordinateSet s;
    s.add(GeoCoordinate(10.0, 20.0));
    s.add(GeoCoordinate(-5.0, 30.0));

    EXPECT_TRUE(s.contains(GeoCoordinate(10.0, 20.0)));
    EXPECT_TRUE(s(GeoCoordinate(10.0, 20.0)));
    EXPECT_FALSE(s.contains(GeoCoordinate(0.0, 0.0)));
    EXPECT_FALSE(s(GeoCoordinate(0.0, 0.0)));
}

TEST(GeoCoordinateSetTest, SortedOrderAfterAdds) {
    GeoCoordinateSet s;
    s.add(GeoCoordinate(10.0, 20.0));
    s.add(GeoCoordinate(0.0, 0.0));
    s.add(GeoCoordinate(10.0, 5.0));
    s.add(GeoCoordinate(-5.0, 10.0));

    std::ostringstream os;
    os << s;
    EXPECT_EQ(os.str(), "{ (-5, 10), (0, 0), (10, 5), (10, 20) }");
}

TEST(GeoCoordinateSetTest, RemoveExistingAndMissing) {
    GeoCoordinateSet s;
    s.add(GeoCoordinate(10.0, 20.0));
    s.add(GeoCoordinate(0.0, 0.0));

    EXPECT_TRUE(s.remove(GeoCoordinate(10.0, 20.0)));
    EXPECT_EQ(s.size(), 1u);
    EXPECT_FALSE(s.remove(GeoCoordinate(10.0, 20.0)));
    EXPECT_FALSE(s.remove(GeoCoordinate(99.0, 99.0)));
}

TEST(GeoCoordinateSetTest, RemoveLastElement) {
    GeoCoordinateSet s;
    s.add(GeoCoordinate(7.0, 7.0));
    EXPECT_TRUE(s.remove(GeoCoordinate(7.0, 7.0)));
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0u);
}

TEST(GeoCoordinateSetTest, CopyIsIndependent) {
    GeoCoordinateSet a;
    a.add(GeoCoordinate(1.0, 1.0));
    a.add(GeoCoordinate(2.0, 2.0));

    GeoCoordinateSet b(a);
    EXPECT_EQ(b.size(), 2u);
    EXPECT_TRUE(b.contains(GeoCoordinate(1.0, 1.0)));

    b.add(GeoCoordinate(3.0, 3.0));
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(a.size(), 2u);
    EXPECT_FALSE(a.contains(GeoCoordinate(3.0, 3.0)));
}

TEST(GeoCoordinateSetTest, SelfAssignmentSafe) {
    GeoCoordinateSet a;
    a.add(GeoCoordinate(1.0, 2.0));
    GeoCoordinateSet& ref = a;
    a = ref;
    EXPECT_EQ(a.size(), 1u);
    EXPECT_TRUE(a.contains(GeoCoordinate(1.0, 2.0)));
}

TEST(GeoCoordinateSetTest, MoveLeavesSourceEmpty) {
    GeoCoordinateSet a;
    a.add(GeoCoordinate(1.0, 1.0));
    a.add(GeoCoordinate(2.0, 2.0));

    GeoCoordinateSet b(std::move(a));
    EXPECT_EQ(b.size(), 2u);
    EXPECT_TRUE(b.contains(GeoCoordinate(1.0, 1.0)));

    EXPECT_EQ(a.size(), 0u);
    EXPECT_TRUE(a.empty());
    EXPECT_FALSE(a.contains(GeoCoordinate(1.0, 1.0)));
}

TEST(GeoCoordinateSetTest, Union) {
    GeoCoordinateSet a;
    a.add(GeoCoordinate(1.0, 1.0));
    a.add(GeoCoordinate(2.0, 2.0));
    GeoCoordinateSet b;
    b.add(GeoCoordinate(2.0, 2.0));
    b.add(GeoCoordinate(3.0, 3.0));

    GeoCoordinateSet u = a | b;
    EXPECT_EQ(u.size(), 3u);
    EXPECT_TRUE(u.contains(GeoCoordinate(1.0, 1.0)));
    EXPECT_TRUE(u.contains(GeoCoordinate(2.0, 2.0)));
    EXPECT_TRUE(u.contains(GeoCoordinate(3.0, 3.0)));
    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(b.size(), 2u);
}

TEST(GeoCoordinateSetTest, Intersection) {
    GeoCoordinateSet a;
    a.add(GeoCoordinate(1.0, 1.0));
    a.add(GeoCoordinate(2.0, 2.0));
    a.add(GeoCoordinate(3.0, 3.0));
    GeoCoordinateSet b;
    b.add(GeoCoordinate(2.0, 2.0));
    b.add(GeoCoordinate(3.0, 3.0));
    b.add(GeoCoordinate(4.0, 4.0));

    GeoCoordinateSet r = a & b;
    EXPECT_EQ(r.size(), 2u);
    EXPECT_TRUE(r.contains(GeoCoordinate(2.0, 2.0)));
    EXPECT_TRUE(r.contains(GeoCoordinate(3.0, 3.0)));
    EXPECT_FALSE(r.contains(GeoCoordinate(1.0, 1.0)));
}

TEST(GeoCoordinateSetTest, Difference) {
    GeoCoordinateSet a;
    a.add(GeoCoordinate(1.0, 1.0));
    a.add(GeoCoordinate(2.0, 2.0));
    a.add(GeoCoordinate(3.0, 3.0));
    GeoCoordinateSet b;
    b.add(GeoCoordinate(2.0, 2.0));
    b.add(GeoCoordinate(4.0, 4.0));

    GeoCoordinateSet r = a - b;
    EXPECT_EQ(r.size(), 2u);
    EXPECT_TRUE(r.contains(GeoCoordinate(1.0, 1.0)));
    EXPECT_TRUE(r.contains(GeoCoordinate(3.0, 3.0)));
    EXPECT_FALSE(r.contains(GeoCoordinate(2.0, 2.0)));
}

TEST(GeoCoordinateSetTest, SubsetOperator) {
    GeoCoordinateSet a;
    a.add(GeoCoordinate(1.0, 1.0));
    a.add(GeoCoordinate(2.0, 2.0));
    GeoCoordinateSet b;
    b.add(GeoCoordinate(1.0, 1.0));
    b.add(GeoCoordinate(2.0, 2.0));
    b.add(GeoCoordinate(3.0, 3.0));
    GeoCoordinateSet c;
    c.add(GeoCoordinate(1.0, 1.0));
    c.add(GeoCoordinate(5.0, 5.0));
    GeoCoordinateSet e;

    EXPECT_TRUE(a <= b);
    EXPECT_FALSE(b <= a);
    EXPECT_TRUE(a <= a);
    EXPECT_FALSE(a <= c);
    EXPECT_TRUE(e <= a);
    EXPECT_TRUE(e <= e);
    EXPECT_FALSE(a <= e);
}

TEST(GeoCoordinateSetTest, EqualityAndInequality) {
    GeoCoordinateSet a;
    a.add(GeoCoordinate(1.0, 1.0));
    a.add(GeoCoordinate(2.0, 2.0));
    GeoCoordinateSet b;
    b.add(GeoCoordinate(2.0, 2.0));
    b.add(GeoCoordinate(1.0, 1.0));
    GeoCoordinateSet c;
    c.add(GeoCoordinate(1.0, 1.0));

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
}

TEST(GeoCoordinateSetTest, PlusEqualMinusEqual) {
    GeoCoordinateSet s;
    s += GeoCoordinate(1.0, 1.0);
    s += GeoCoordinate(2.0, 2.0);
    EXPECT_EQ(s.size(), 2u);

    s -= GeoCoordinate(1.0, 1.0);
    EXPECT_EQ(s.size(), 1u);
    EXPECT_FALSE(s.contains(GeoCoordinate(1.0, 1.0)));
    EXPECT_TRUE(s.contains(GeoCoordinate(2.0, 2.0)));

    s += GeoCoordinate(3.0, 3.0) += GeoCoordinate(4.0, 4.0);
    EXPECT_EQ(s.size(), 3u);
}

TEST(GeoCoordinateSetTest, OutputFormatEmpty) {
    GeoCoordinateSet s;
    std::ostringstream os;
    os << s;
    EXPECT_EQ(os.str(), "{ }");
}