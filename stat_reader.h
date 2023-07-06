#pragma once

#include "transport_catalogue.h"
#include "input_reader.h"

#include <string>
#include <vector>

const std::vector<Request> ReadingUserRequests();

void ProcessingUserRequests(TransportCatalogue &catalogue, const std::vector<Request> &requests);