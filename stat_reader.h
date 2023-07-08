#pragma once

#include "transport_catalogue.h"
#include "input_reader.h"

#include <string>
#include <vector>

namespace transport
{
    namespace detail
    {
        const std::vector<Request> ReadUserRequests(std::istream& in);

        void ProcessUserRequests(TransportCatalogue &catalogue, const std::vector<Request> &requests, std::ostream& out);

        void UserBusRequest(TransportCatalogue &catalogue, const Request &request, std::ostream& out);

        void UserStopRequest(TransportCatalogue &catalogue, const Request &request, std::ostream& out);
    }
}