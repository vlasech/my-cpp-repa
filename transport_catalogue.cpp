#include "transport_catalogue.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

void TransportCatalogue::AddStop(Stop stop)
{
    stops_.push_back(stop);
    stopname_to_stop_.insert({stops_.back().name, &stops_.back()});
}

void TransportCatalogue::AddBus(Bus bus)
{
    buses_.push_back(bus);
    const Bus *bus_ptr = &buses_.back();
    busname_to_bus_.insert({buses_.back().name, bus_ptr});

    for (const Stop* stop : bus_ptr->stops)
    {
        stopname_to_buses_[stop->name].insert(bus_ptr);
    }
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
    const Bus *bus = GetBusByName(name);
    if (bus == nullptr)
    {
        throw std::invalid_argument("bus is not found");
    }
    if (bus->is_circular)
        return {bus->stops.size(), UniqueStopsCount(name), ComputeGeoRouteLength(name)};
    else
        return {bus->stops.size() * 2 - 1, UniqueStopsCount(name), ComputeGeoRouteLength(name)};
}

std::unordered_set<const Bus*> TransportCatalogue::GetStopInfo(std::string_view name) const
{
    if (stopname_to_buses_.count(name) > 0)
    {
        return stopname_to_buses_.at(name);
    }
    return {};
}

size_t TransportCatalogue::UniqueStopsCount(std::string_view name) const
{
    std::unordered_set<std::string_view> unique_stops;
    for (const auto &stop : busname_to_bus_.at(name)->stops)
    {
        unique_stops.insert(stop->name);
    }
    return unique_stops.size();
}

double TransportCatalogue::ComputeGeoRouteLength(std::string_view name) const
{
    const Bus *bus = GetBusByName(name);
    double geo_route_length = 0;
    for (auto it = bus->stops.begin(); it + 1 != bus->stops.end(); ++it)
    {

        if (bus->is_circular)
        {
            geo_route_length += ComputeDistance((*it)->coordinates, (*(it + 1))->coordinates);
        }
        else
        {
            geo_route_length += ComputeDistance((*it)->coordinates, (*(it + 1))->coordinates) * 2;
        }
    }

    if (bus->is_circular)
    {
        geo_route_length += ComputeDistance((*(bus->stops.end() - 1))->coordinates, (*bus->stops.begin())->coordinates);
    }
    return geo_route_length;
}

void TransportCatalogue::PrintStops() const
{
    for (const auto &stop : stops_)
    {
        std::cout << stop.name << std::endl;
    }
}

void TransportCatalogue::PrintBuses() const
{
    for (const auto &bus : buses_)
    {
        std::cout << "Bus name: " << bus.name << ", stops: ";
        for (const auto &stop : bus.stops)
        {
            std::cout << stop->name << " ";
        }
        std::cout << "\n";
    }
}
