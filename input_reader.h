#pragma once

#include "transport_catalogue.h"

#include <string>
#include <tuple>

enum class RequestType
{
    STOP,
    BUS
};

struct Request
{
    RequestType type;
    std::string data;
};

const std::vector<Request> ReadingRequests();

void ProcessingRequests(TransportCatalogue &catalogue, const std::vector<Request> &requests);

std::tuple<Stop, std::vector<Distance>> ParseStopRequest(const std::string &request);

std::tuple<Stop, std::vector<Distance>> ParseStopRequestWithDistence(const std::string &request, Stop stop, size_t it);

Bus ParseBusRequest(const std::string &request, TransportCatalogue &catalogue);