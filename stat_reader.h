#pragma once

#include "transport_catalogue.h"

#include <string>
#include <vector>

std::vector<std::string> ReadingUserRequests();

void ProcessingUserRequests(TransportCatalogue &catalogue, const std::vector<std::string> &requests);