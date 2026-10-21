#pragma once
#include <iosfwd>

class GeoCoordinate {
public:
    GeoCoordinate();
    GeoCoordinate(int latitude, int longitude);
    GeoCoordinate(const GeoCoordinate& other);
    GeoCoordinate(GeoCoordinate&& other) noexcept;
    GeoCoordinate& operator=(const GeoCoordinate& other);
    GeoCoordinate& operator=(GeoCoordinate&& other) noexcept;
    ~GeoCoordinate();

    bool is_valid() const;
    bool operator==(const GeoCoordinate& other) const;
    bool operator<(const GeoCoordinate& other) const;

    friend std::ostream& operator<<(std::ostream& out, const GeoCoordinate& geo_coordinate);

private:
    int day_;
    int month_;
    int year_;
};