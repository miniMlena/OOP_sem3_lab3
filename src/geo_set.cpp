#include "geo_set.h"
#include <ostream>
#include <utility>

GeoCoordinateSet::GeoCoordinateSet()
    : data_(new GeoCoordinate[0]), size_(0), capacity_(0) {}

GeoCoordinateSet::~GeoCoordinateSet() {
    delete[] data_;
}

GeoCoordinateSet::GeoCoordinateSet(const GeoCoordinateSet& other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    data_ = new GeoCoordinate[capacity_];

    for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
}

GeoCoordinateSet::GeoCoordinateSet(GeoCoordinateSet&& other) noexcept {
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;

    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

GeoCoordinateSet& GeoCoordinateSet::operator=(const GeoCoordinateSet& other) {
    if (this != &other) {
        GeoCoordinateSet tmp(other);
        std::swap(data_, tmp.data_);
        std::swap(size_, tmp.size_);
        std::swap(capacity_, tmp.capacity_);
    }
    return *this;
}

GeoCoordinateSet& GeoCoordinateSet::operator=(GeoCoordinateSet&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

std::size_t GeoCoordinateSet::size() const { return size_; }
bool GeoCoordinateSet::empty() const { return size_ == 0; }

bool GeoCoordinateSet::contains(const GeoCoordinate& coord) const {
    std::size_t left = 0;
    std::size_t right = size_;

    while (left < right) {
        const std::size_t mid = left + (right - left) / 2;
        if (data_[mid] == coord) return true;
        if (data_[mid] < coord) left = mid + 1;
        else right = mid;
    }
    return false;
}

bool GeoCoordinateSet::operator()(const GeoCoordinate& coord) const {
    return contains(coord);
}

bool GeoCoordinateSet::add(const GeoCoordinate& coord) {
    if (!coord.is_valid()) return false;
    if (contains(coord)) return false;

    if (size_ == capacity_) {
        const std::size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
        GeoCoordinate* new_data = new GeoCoordinate[new_capacity];

        for (std::size_t i = 0; i < size_; ++i)
            new_data[i] = data_[i];

        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

    std::size_t pos = size_;
    while (pos > 0 && coord < data_[pos - 1]) {
        data_[pos] = data_[pos - 1];
        --pos;
    }
    data_[pos] = coord;
    ++size_;
    return true;
}

bool GeoCoordinateSet::remove(const GeoCoordinate& coord) {
    std::size_t left = 0;
    std::size_t right = size_;

    while (left < right) {
        const std::size_t mid = left + (right - left) / 2;
        if (data_[mid] == coord) {
            for (std::size_t i = mid + 1; i < size_; ++i)
                data_[i - 1] = data_[i];
            --size_;
            return true;
        }
        if (data_[mid] < coord) left = mid + 1;
        else right = mid;
    }
    return false;
}

GeoCoordinateSet& GeoCoordinateSet::operator+=(const GeoCoordinate& coord) {
    add(coord);
    return *this;
}

GeoCoordinateSet& GeoCoordinateSet::operator-=(const GeoCoordinate& coord) {
    remove(coord);
    return *this;
}

GeoCoordinateSet GeoCoordinateSet::operator|(const GeoCoordinateSet& other) const {
    GeoCoordinateSet result;
    const std::size_t max_capacity = size_ + other.size_;
    if (max_capacity == 0) return result;

    result.data_ = new GeoCoordinate[max_capacity];
    result.capacity_ = max_capacity;

    std::size_t i = 0, j = 0;
    while (i < size_ && j < other.size_) {
        if (data_[i] < other.data_[j])
            result.data_[result.size_++] = data_[i++];
        else if (other.data_[j] < data_[i])
            result.data_[result.size_++] = other.data_[j++];
        else {
            result.data_[result.size_++] = data_[i++];
            ++j;
        }
    }
    while (i < size_) result.data_[result.size_++] = data_[i++];
    while (j < other.size_) result.data_[result.size_++] = other.data_[j++];

    return result;
}

GeoCoordinateSet GeoCoordinateSet::operator&(const GeoCoordinateSet& other) const {
    GeoCoordinateSet result;
    const std::size_t max_capacity = size_ < other.size_ ? size_ : other.size_;
    if (max_capacity == 0) return result;

    result.data_ = new GeoCoordinate[max_capacity];
    result.capacity_ = max_capacity;

    std::size_t i = 0, j = 0;
    while (i < size_ && j < other.size_) {
        if (data_[i] < other.data_[j]) ++i;
        else if (other.data_[j] < data_[i]) ++j;
        else {
            result.data_[result.size_++] = data_[i++];
            ++j;
        }
    }
    return result;
}

GeoCoordinateSet GeoCoordinateSet::operator-(const GeoCoordinateSet& other) const {
    GeoCoordinateSet result;
    if (size_ == 0) return result;

    result.data_ = new GeoCoordinate[size_];
    result.capacity_ = size_;

    std::size_t i = 0, j = 0;
    while (i < size_ && j < other.size_) {
        if (data_[i] < other.data_[j])
            result.data_[result.size_++] = data_[i++];
        else if (other.data_[j] < data_[i])
            ++j;
        else {
            ++i;
            ++j;
        }
    }
    while (i < size_) result.data_[result.size_++] = data_[i++];

    return result;
}

bool GeoCoordinateSet::operator<=(const GeoCoordinateSet& other) const {
    std::size_t i = 0, j = 0;
    while (i < size_ && j < other.size_) {
        if (data_[i] < other.data_[j])
            return false;
        else if (other.data_[j] < data_[i])
            ++j;
        else {
            ++i;
            ++j;
        }
    }
    return i == size_;
}

bool GeoCoordinateSet::operator==(const GeoCoordinateSet& other) const {
    if (size_ != other.size_) return false;
    for (std::size_t i = 0; i < size_; ++i)
        if (!(data_[i] == other.data_[i]))
            return false;
    return true;
}

bool GeoCoordinateSet::operator!=(const GeoCoordinateSet& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& out, const GeoCoordinateSet& set) {
    out << '{';
    for (std::size_t i = 0; i < set.size_; ++i) {
        if (i > 0) out << ',';
        out << ' ' << set.data_[i];
    }
    out << " }";
    return out;
}