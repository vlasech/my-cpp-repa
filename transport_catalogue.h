#pragma once

#include "geo.h"

#include <string>
#include <vector>
#include <deque>
#include <set>
#include <unordered_set>
#include <unordered_map>

struct Stop
{
    std::string name;
    Coordinates coordinates;
//    std::unordered_map<std::string_view, int> distances;
};

struct Bus
{
    std::string name;
    std::vector<const Stop *> stops;
    bool is_circular;
};

struct BusInfo
{
    size_t stops_count;       // количество остановок в маршруте автобуса от stop1 до stop1 включительно
    size_t unique_stop_count; // количество уникальных остановок, на которых останавливается автобус
    double geo_route_length;  // длина маршрута в метрах (географическая)
};

class TransportCatalogue
{
private:
    std::deque<Stop> stops_;
    std::unordered_map<std::string_view, const Stop *> stopname_to_stop_;

    std::deque<Bus> buses_;
    std::unordered_map<std::string_view, const Bus *> busname_to_bus_;

    std::unordered_map<std::string_view, std::unordered_set<const Bus *>> stopname_to_buses_;

    //    std::unordered_map<std::string_view, std::unordered_set<std::string_view>> stopname_to_buses_;

    double ComputeGeoRouteLength(std::string_view name) const;

    size_t UniqueStopsCount(std::string_view name) const;

public:
    TransportCatalogue() = default;
    ~TransportCatalogue() = default;

    void AddStop(Stop stop);

    void AddBus(Bus bus);

    const Stop *GetStopByName(std::string_view name) const;

    const Bus *GetBusByName(std::string_view name) const;

    BusInfo GetBusInfo(std::string_view name);

    std::unordered_set<const Bus *> GetStopInfo(std::string_view name) const;

    void PrintStops() const;

    void PrintBuses() const;
};
