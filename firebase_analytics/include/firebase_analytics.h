#pragma once

#include <dmsdk/sdk.h>
#include "firebase/analytics.h"

void FirebaseAnalytics_Safe_LogEvent(lua_State*, const char* name, const firebase::analytics::Parameter* params, size_t number_of_parameters);
