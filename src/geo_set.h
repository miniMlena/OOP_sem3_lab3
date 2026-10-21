#include "geo_coordinate.h"

class GeoCoordinateSet {
public:
    GeoCoordinateSet();
    GeoCoordinateSet(const GeoCoordinateSet&);
    GeoCoordinateSet(GeoCoordinateSet&&) noexcept;
    GeoCoordinateSet& operator=(const GeoCoordinateSet&);
    GeoCoordinateSet& operator=(GeoCoordinateSet&&) noexcept;
    ~GeoCoordinateSet();
    bool add(const GeoCoordinate&); // true — множество изменилось
    bool remove(const GeoCoordinate&); // true — элемент был и удалён
    bool contains(const GeoCoordinate&) const;
    bool operator()(const GeoCoordinate&) const; // принадлежность
    std::size_t size() const; // количество элементов
    bool empty() const; // size() == 0
    GeoCoordinateSet& operator+=(const GeoCoordinate&);
    GeoCoordinateSet& operator-=(const GeoCoordinate&);
    GeoCoordinateSet operator|(const GeoCoordinateSet&) const; // объединение
    GeoCoordinateSet operator&(const GeoCoordinateSet&) const; // пересечение
    GeoCoordinateSet operator-(const GeoCoordinateSet&) const; // разность
    bool operator==(const GeoCoordinateSet&) const;
    bool operator!=(const GeoCoordinateSet&) const;
    friend std::ostream& operator<<(std::ostream&, const GeoCoordinateSet&);
};