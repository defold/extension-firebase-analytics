#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_IOS1)

#include "firebase/app.h"
#include "firebase/analytics.h"
#include "firebase/analytics/event_names.h"
#include "firebase/analytics/parameter_names.h"
#include "firebase/future.h"

#include "firebase.h"
#include "firebase_analytics.h"

#import <Foundation/Foundation.h>

using namespace firebase;
using namespace firebase::analytics;

void FirebaseAnalytics_Safe_LogEvent(lua_State* L, const char* name, const firebase::analytics::Parameter* params, size_t number_of_parameters)
{
    @try
    {
        LogEvent(name, params, number_of_parameters);
    }
    @catch(...)
    {
        dmLogError("Exception occurred for event '%s' with parameters:", name)

        for(size_t index = 0; index < number_of_parameters; ++index)
        {
            const auto& parameter = params[index];
            const auto& value = parameter.value;

            if (value.is_double())
            {
                dmLogError("\t'%s' = %f", parameter.name, value.double_value())
            }
            else if (value.is_string() || value.is_bool() || value.is_null())
            {
                dmLogError("\t'%s' = '%s'", parameter.name, value.AsString().string_value());
            }
            else
            {
                dmLogError("\t'%s' = unsupported param type", parameter.name)
            }
        }

        luaL_error(L, "firebase: log event exception. \"%s\"", name);
    }
}

#endif
