#include "stat_reader.h"

#include <iostream>
#include <algorithm>
#include <iomanip>

const std::vector<Request> ReadingUserRequests()
{
    return ReadingRequests();
}

void ProcessingUserRequests(TransportCatalogue &catalogue, const std::vector<Request> &requests)
{
    for (const auto &request : requests)
    {
        if (request.type == RequestType::BUS)
            UserBusRequest(catalogue, request);
        if (request.type == RequestType::STOP)
            UserStopRequest(catalogue, request);
    }
}

void UserBusRequest(TransportCatalogue &catalogue, const Request &request)
{
    std::cout << "Bus " << request.data << ": ";
    if (!catalogue.GetBusByName(request.data))
    {
        std::cout << "not found" << std::endl;
    }
    else
    {
        auto result = catalogue.GetBusInfo(request.data);
        std::cout << std::setprecision(6)
                  << result.stops_count << " stops on route, "
                  << result.unique_stop_count << " unique stops, "
                  << result.real_route_length << " route length, "
                  << result.real_route_length / result.geo_route_length << " curvature"
                  << std::endl;
    }
}

void UserStopRequest(TransportCatalogue &catalogue, const Request &request)
{
    std::cout << "Stop " << request.data << ": ";
    if (!catalogue.GetStopByName(request.data))
    {
        std::cout << "not found" << std::endl;
    }
    else
    {
        std::unordered_set<const Bus *> result = catalogue.GetStopInfo(request.data);
        if (result.empty())
        {
            std::cout << "no buses" << std::endl;
        }
        else
        {
            std::cout << "buses ";
            std::vector<const Bus *> buses(result.begin(), result.end());
            std::sort(buses.begin(), buses.end(), [](const Bus *lhs, const Bus *rhs)
                      { return lhs->name < rhs->name; });
            for (auto bus : buses)
            {
                std::cout << bus->name << " ";
            }
            std::cout << std::endl;
        }
    }
}