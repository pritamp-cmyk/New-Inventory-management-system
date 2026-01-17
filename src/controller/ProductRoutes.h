#pragma once
#include "external/httplib.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void registerProductRoutes(httplib::Server& server);
