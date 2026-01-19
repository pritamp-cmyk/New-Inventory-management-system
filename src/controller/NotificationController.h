#pragma once
#include "../external/httplib.h"

class NotificationController {
public:
    static void registerRoutes(httplib::Server& svr);
};
