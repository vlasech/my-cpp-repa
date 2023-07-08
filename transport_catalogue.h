#pragma once

#include "geo.h"

#include <string>
#include <vector>
#include <deque>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <cstdint>

namespace transport
{
    struct Stop
    {
        std::string name;
        detail::Coordinates coordinates;
    };

    struct Bus
    {
        std::string name;
        std::vector<const Stop *> stops;
        bool is_circular;
    };

    struct BusInfo
    {
        size_t stops_count;        // количество остановок в маршруте автобуса от stop1 до stop1 включительно
        size_t unique_stop_count;  // количество уникальных остановок, на которых останавливается автобус
        double geo_route_length;   // длина маршрута в метрах (географическая)
        int32_t real_route_length; // длина маршрута в метрах (реальная)
    };

    struct Hasher
    {
        size_t operator()(const std::pair<const Stop *, const Stop *> &pair) const
        {
            std::hash<const void *> pointer_hash;
            return pointer_hash(pair.first) ^ (pointer_hash(pair.second) << 1);
        }
    };

    struct Distance
    {
        std::string stop1;
        std::string stop2;
        uint32_t distance;
    };

    class TransportCatalogue
    {
    private:
        std::deque<Stop> stops_;
        std::unordered_map<std::string_view, const Stop *> stopname_to_stop_;

        std::deque<Bus> buses_;
        std::unordered_map<std::string_view, const Bus *> busname_to_bus_;

        std::unordered_map<std::string_view, std::unordered_set<const Bus *>> stopname_to_buses_;

        std::unordered_map<std::pair<const Stop *, const Stop *>, int32_t, Hasher> distances_between_stops_;

        std::unordered_map<std::string_view, size_t> busname_to_unique_stops_;

        double ComputeGeoRouteLength(std::string_view name) const;

        uint32_t ComputeRealRouteLength(std::string_view name) const;

        size_t GetUniqueStopsCount(std::string_view name) const;

    public:
        TransportCatalogue() = default;
        ~TransportCatalogue() = default;

        void AddStop(const Stop &stop);

        void AddDistance(Distance distances_between_stops);

        void AddBus(const Bus &bus);

        const Stop *GetStopByName(std::string_view name) const;

        const Bus *GetBusByName(std::string_view name) const;

        BusInfo GetBusInfo(std::string_view name);

        std::unordered_set<const Bus *> GetStopInfo(std::string_view name) const;

        uint32_t GetDistance(const Stop *stop1, const Stop *stop2) const;
    };
}