#pragma once

#include "transport_catalogue.h"

#include <string>

enum class RequestType {
    STOP,
    BUS
};

struct Request {
    RequestType type;
    std::string data;
};

std::vector<Request> ReadingRequests();

void ProcessingRequests(TransportCatalogue &catalogue, const std::vector<Request> &requests);

Stop ParseStopRequest(const std::string &request);

Bus ParseBusRequest(const std::string &request, TransportCatalogue &catalogue);

std::string Trim(const std::string &str);

std::vector<std::string_view> SplitIntoWords(std::string_view text);
