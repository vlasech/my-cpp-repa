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

    for (const auto &request : stop_requests)
    {
        catalogue.AddStop(ParseStopRequest(request.data));
    }

    for (const auto &request : bus_requests)
    {
        catalogue.AddBus(ParseBusRequest(request.data, catalogue));
    }
}

Stop ParseStopRequest(const std::string &request)
{
    Stop stop;
    size_t it1 = request.find(':');
    stop.name = request.substr(0, it1);
    size_t it2 = request.find(',');
    stop.coordinates.lat = std::stod(request.substr(it1 + 1, it2));
    stop.coordinates.lng = std::stod(request.substr(it2 + 1));
    return stop;
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