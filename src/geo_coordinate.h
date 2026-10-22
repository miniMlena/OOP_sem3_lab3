#pragma once
#include <iosfwd>

class GeoCoordinate {
public:
    GeoCoordinate();
    GeoCoordinate(double latitude, double longitude);
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
    double latitude_; // [-90, 90]
    double longitude_; // [-180, 180]
};