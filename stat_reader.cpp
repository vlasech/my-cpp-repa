#include "stat_reader.h"

#include <iostream>
#include <algorithm>

const std::vector<Request> ReadingUserRequests()
{
    return ReadingRequests();
}

void ProcessingUserRequests(TransportCatalogue &catalogue, const std::vector<Request> &requests)
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

    for (const auto &request : requests)
    {
        if (request.type == RequestType::BUS)
        {

            std::cout << "Bus " << request.data << ": ";
            if (!catalogue.GetBusByName(request.data))
            {
                std::cout << "not found" << std::endl;
            }
            else
            {
                auto result = catalogue.GetBusInfo(request.data);
                std::cout << result.stops_count << " stops on route, "
                          << result.unique_stop_count << " unique stops, "
                          << result.geo_route_length << " route length"
                          << std::endl;
            }
        }
        else
        {
            std::cout << "Stop " << request.data << ": ";
            if (!catalogue.GetStopByName(request.data))
            {
                std::cout << "not found" << std::endl;
            }
            else
            {
                auto result = catalogue.GetStopInfo(request.data);
                if (result.empty())
                {
                    std::cout << "no buses" << std::endl;
                }
                else
                {
                    std::cout << "buses ";
                    std::vector<std::string_view> buses(result.begin(), result.end());
                    std::sort(buses.begin(), buses.end());
                    for (auto bus : buses)
                    {
                        std::cout << bus << " ";
                    }
                    std::cout << std::endl;
                }
            }
        }
    }
}