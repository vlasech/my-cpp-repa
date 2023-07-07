#pragma once

#include "transport_catalogue.h"
#include "input_reader.h"

#include <string>
#include <vector>

const std::vector<Request> ReadingUserRequests();

void ProcessingUserRequests(TransportCatalogue &catalogue, const std::vector<Request> &requests);

void UserBusRequest(TransportCatalogue &catalogue, const Request &request);

void UserStopRequest(TransportCatalogue &catalogue, const Request &request);