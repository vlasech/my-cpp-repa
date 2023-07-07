#include "input_reader.h"

#include <iostream>
#include <sstream>

const std::vector<Request> ReadingRequests()
{
    std::vector<Request> requests;
    std::string request;
    int n;

    std::cin >> n;
    std::cin.ignore();

    for (int i = 0; i < n; ++i)
    {
        std::getline(std::cin, request);
        RequestType type = (request.substr(0, 4) == "Stop") ? RequestType::STOP : RequestType::BUS;
        request.erase(0, request.find(' ') + 1);
        requests.push_back({type, request});
    }
    return requests;
}

void ProcessingRequests(TransportCatalogue &catalogue, const std::vector<Request> &requests)
{
    std::vector<Request> stop_requests;
    std::vector<Request> bus_requests;

    for (const auto &request : requests)
    {
        switch (request.type)
        {
        case RequestType::STOP:
            stop_requests.push_back(request);
            break;
        case RequestType::BUS:
            bus_requests.push_back(request);
            break;
        }
    }
    std::vector<Distance> distances_between_stops;
    for (const auto &request : stop_requests)
    {
        auto temp = ParseStopRequest(request.data);
        catalogue.AddStop(std::get<0>(temp));
        distances_between_stops.insert(distances_between_stops.end(),
                                       std::get<1>(temp).begin(), std::get<1>(temp).end());
    }

    for (const auto &request : distances_between_stops)
    {
        catalogue.AddDistance(request);
    }

    for (const auto &request : bus_requests)
    {
        catalogue.AddBus(ParseBusRequest(request.data, catalogue));
    }
}

std::tuple<Stop, std::vector<Distance>> ParseStopRequest(const std::string &request)
{
    Stop stop;
    size_t it1 = request.find(':');
    stop.name = request.substr(0, it1);
    size_t it2 = request.find(',');
    stop.coordinates.lat = std::stod(request.substr(it1 + 1, it2));
    size_t it3 = request.find(',', it2 + 1);
    if (it3 == std::string::npos)
    {
        stop.coordinates.lng = std::stod(request.substr(it2 + 1));
        std::vector<Distance> distances_between_stops;
        return std::make_tuple(stop, distances_between_stops);
    }
    else
    {
        stop.coordinates.lng = std::stod(request.substr(it2 + 1, it3 - 1 - it2));
        return ParseStopRequestWithDistence(request, stop, it3);
    }
}

std::tuple<Stop, std::vector<Distance>> ParseStopRequestWithDistence(const std::string &request, Stop stop, size_t it)
{
    std::vector<Distance> distances_between_stops;
    size_t start = it + 2;
    size_t end;
    while ((end = request.find('m', start)) != std::string::npos)
    {
        Distance temp;
        temp.distance = std::stoi(request.substr(start, end - start));
        temp.stop1 = stop.name;

        start = end + 5;
        size_t end2 = request.find(',', start);
        if (end2 == std::string::npos)
        {
            end2 = request.length();
        }
        temp.stop2 = request.substr(start, end2 - start);
        distances_between_stops.push_back(temp);
        start = end2 + 2;
    }
    return std::make_tuple(stop, distances_between_stops);
}

Bus ParseBusRequest(const std::string &request, TransportCatalogue &catalogue)
{
    Bus bus;
    size_t it = request.find(':');
    bus.name = request.substr(0, it);
    bus.is_circular = (request.find('>') != std::string::npos);

    size_t start = it + 2;
    size_t end = std::min(request.find('>', start), request.find('-', start));
    while (end != std::string::npos)
    {
        std::string stop_name = request.substr(start, end - 1 - start);
        bus.stops.push_back(catalogue.GetStopByName(stop_name));
        start = end + 2;
        end = std::min(request.find('>', start), request.find('-', start));
    }
    std::string stop_name = request.substr(start, request.size());
    bus.stops.push_back(catalogue.GetStopByName(stop_name));
    return bus;
}