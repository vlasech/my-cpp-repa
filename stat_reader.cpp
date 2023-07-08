#include "stat_reader.h"

#include <iostream>
#include <algorithm>
#include <iomanip>

namespace transport
{
    namespace detail
    {
        const std::vector<Request> ReadUserRequests(std::istream& in)
        {
            return ReadRequests(in);
        }

        void ProcessUserRequests(TransportCatalogue &catalogue, const std::vector<Request> &requests, std::ostream& out)
        {
            for (const auto &request : requests)
            {
                if (request.type == RequestType::BUS)
                    UserBusRequest(catalogue, request, out);
                if (request.type == RequestType::STOP)
                    UserStopRequest(catalogue, request, out);
            }
        }

        void UserBusRequest(TransportCatalogue &catalogue, const Request &request, std::ostream& out)
        {
            out << "Bus " << request.data << ": ";
            if (!catalogue.GetBusByName(request.data))
            {
                out << "not found" << std::endl;
            }
            else
            {
                auto result = catalogue.GetBusInfo(request.data);
                out << std::setprecision(6)
                          << result.stops_count << " stops on route, "
                          << result.unique_stop_count << " unique stops, "
                          << result.real_route_length << " route length, "
                          << result.real_route_length / result.geo_route_length << " curvature"
                          << std::endl;
            }
        }

        void UserStopRequest(TransportCatalogue &catalogue, const Request &request, std::ostream& out)
        {
            out << "Stop " << request.data << ": ";
            if (!catalogue.GetStopByName(request.data))
            {
                out << "not found" << std::endl;
            }
            else
            {
                std::unordered_set<const Bus *> result = catalogue.GetStopInfo(request.data);
                if (result.empty())
                {
                    out << "no buses" << std::endl;
                }
                else
                {
                    out << "buses ";
                    std::vector<const Bus *> buses(result.begin(), result.end());
                    std::sort(buses.begin(), buses.end(), [](const Bus *lhs, const Bus *rhs)
                              { return lhs->name < rhs->name; });
                    for (auto bus : buses)
                    {
                        out << bus->name << " ";
                    }
                    out << std::endl;
                }
            }
        }
    }
}