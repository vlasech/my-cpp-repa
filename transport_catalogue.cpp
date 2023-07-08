#include "transport_catalogue.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

namespace transport
{
    using namespace detail;
    
    void TransportCatalogue::AddStop(const Stop &stop)
    {
        stops_.push_back(stop);
        stopname_to_stop_.insert({stops_.back().name, &stops_.back()});
    }

    void TransportCatalogue::AddDistance(Distance distances_between_stops)
    {
        const Stop *stop1_ptr = GetStopByName(distances_between_stops.stop1);
        const Stop *stop2_ptr = GetStopByName(distances_between_stops.stop2);

        distances_between_stops_.insert({std::make_pair(stop1_ptr, stop2_ptr), distances_between_stops.distance});
    }

    void TransportCatalogue::AddBus(const Bus &bus)
    {
        buses_.push_back(bus);
        const Bus *bus_ptr = &buses_.back();
        busname_to_bus_.insert({buses_.back().name, bus_ptr});

        for (const Stop *stop : bus_ptr->stops)
        {
            stopname_to_buses_[stop->name].insert(bus_ptr);
        }

        std::unordered_set<std::string_view> unique_stops;
        for (const auto &stop : bus.stops) {
            unique_stops.insert(stop->name);
        }
        busname_to_unique_stops_[buses_.back().name] = unique_stops.size();
    }

    const Stop *TransportCatalogue::GetStopByName(std::string_view name) const
    {
        auto it = stopname_to_stop_.find(name);
        return it == stopname_to_stop_.end() ? nullptr : it->second;
    }

    const Bus *TransportCatalogue::GetBusByName(std::string_view name) const
    {
        auto it = busname_to_bus_.find(name);
        return it == busname_to_bus_.end() ? nullptr : it->second;
    }

    BusInfo TransportCatalogue::GetBusInfo(std::string_view name)
    {
        BusInfo bus_info;
        const Bus *bus = GetBusByName(name);

        if (bus->is_circular)
            bus_info.stops_count = bus->stops.size();
        else
            bus_info.stops_count = bus->stops.size() * 2 - 1;

        bus_info.unique_stop_count = GetUniqueStopsCount(name);
        bus_info.real_route_length = ComputeRealRouteLength(name);
        bus_info.geo_route_length = ComputeGeoRouteLength(name);

        return bus_info;
    }

    std::unordered_set<const Bus *> TransportCatalogue::GetStopInfo(std::string_view name) const
    {
        if (stopname_to_buses_.count(name) > 0)
        {
            return stopname_to_buses_.at(name);
        }
        return {};
    }

    uint32_t TransportCatalogue::GetDistance(const Stop *stop1, const Stop *stop2) const
    {
        auto it = distances_between_stops_.find({stop1, stop2});

        if (it != distances_between_stops_.end())
        {
            return it->second;
        }
        else
        {
            it = distances_between_stops_.find({stop2, stop1});
            return it->second;
        }
    }

    size_t TransportCatalogue::GetUniqueStopsCount(std::string_view name) const
    {
        return busname_to_unique_stops_.at(name);
    }

    double TransportCatalogue::ComputeGeoRouteLength(std::string_view name) const
    {
        const Bus *bus = GetBusByName(name);
        double geo_route_length = 0;
        for (auto it = bus->stops.begin(); it + 1 != bus->stops.end(); ++it)
        {
            if (bus->is_circular)
            {
                geo_route_length += ComputeDistance((*it)->transport::Stop::coordinates, (*(it + 1))->transport::Stop::coordinates);
            }
            else
            {
                geo_route_length += ComputeDistance((*it)->transport::Stop::coordinates, (*(it + 1))->transport::Stop::coordinates) * 2;
            }
        }

        if (bus->is_circular)
        {
            geo_route_length += ComputeDistance((*(bus->stops.end() - 1))->coordinates, (*bus->stops.begin())->coordinates);
        }
        return geo_route_length;
    }

    uint32_t TransportCatalogue::ComputeRealRouteLength(std::string_view name) const
    {
        const Bus *bus = GetBusByName(name);

        uint32_t real_length = 0;
        for (size_t i = 0; i < bus->stops.size() - 1; ++i)
        {
            real_length += GetDistance(bus->stops[i], bus->stops[i + 1]);
        }

        if (!bus->is_circular)
        {
            for (size_t i = bus->stops.size() - 1; i > 0; --i)
            {
                real_length += GetDistance(bus->stops[i], bus->stops[i - 1]);
            }
        }

        return real_length;
    }
}
