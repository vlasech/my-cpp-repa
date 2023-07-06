#include "stat_reader.h"

#include <iostream>

std::vector<std::string> ReadingUserRequests()
{
    std::vector<std::string> requests;
    std::string request;
    int n;

    std::cin >> n;
    std::cin.ignore();

    for (int i = 0; i < n; ++i)
    {
        std::getline(std::cin, request);
        request.erase(0, request.find(' ') + 1);
        requests.push_back(request);
    }
    return requests;
}

void ProcessingUserRequests(TransportCatalogue &catalogue, const std::vector<std::string> &requests)
{

    for (const auto &request : requests)
    {
        if (!catalogue.GetBusByName(request))
        {
            std::cout << "Bus " << request << ":"
                      << " not found" << std::endl;
        }
        else
        {
            auto result = catalogue.GetBusInfo(request);
            std::cout << "Bus " << request << ": "
                      << result.stops_count << " stops on route, "
                      << result.unique_stop_count << " unique stops, "
                      << result.geo_route_length << " route length"
                      << std::endl;
        }
    }
}