#include "geo_coordinate.h"
#include <ostream>

GeoCoordinate::GeoCoordinate()
    : latitude_(0.0), longitude_(0.0) {}

GeoCoordinate::GeoCoordinate(double latitude, double longitude)
    : latitude_(latitude), longitude_(longitude) {}

GeoCoordinate::GeoCoordinate(const GeoCoordinate& other)
    : latitude_(other.latitude_), longitude_(other.longitude_) {}

GeoCoordinate::GeoCoordinate(GeoCoordinate&& other) noexcept
    : latitude_(other.latitude_), longitude_(other.longitude_) {}

GeoCoordinate& GeoCoordinate::operator=(const GeoCoordinate& other) {
    if (this != &other)
    {
        latitude_ = other.latitude_;
        longitude_ = other.longitude_;
    }
    return *this;
}

GeoCoordinate& GeoCoordinate::operator=(GeoCoordinate&& other) noexcept {
    if (this != &other)
    {
        latitude_ = other.latitude_;
        longitude_ = other.longitude_;
    }
    return *this;
}

GeoCoordinate::~GeoCoordinate() = default;

bool GeoCoordinate::is_valid() const {
    if (latitude_ < -90.0 || latitude_ > 90.0) return false;
    if (longitude_ < -180.0 || longitude_ > 180.0) return false;
    return true;
}

bool GeoCoordinate::operator==(const GeoCoordinate& other) const {
    return latitude_ == other.latitude_ && longitude_ == other.longitude_;
}

bool GeoCoordinate::operator<(const GeoCoordinate& other) const {
    if (latitude_ < other.latitude_) return true;
    if (latitude_ > other.latitude_) return false;
    return longitude_ < other.longitude_;
}

std::ostream& operator<<(std::ostream& out, const GeoCoordinate& coord) {
    return out << '(' << coord.latitude_ << ", " << coord.longitude_ << ')';
}