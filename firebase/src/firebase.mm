#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_IOS)

#include "firebase/analytics.h"
#import <Foundation/Foundation.h>

void Firebase_Safe_LogEvent(lua_State* L, const char* name, const firebase::analytics::Parameter* params, size_t number_of_parameters)
{
    @try
    {
        firebase::analytics::LogEvent(name, params, number_of_parameters);
    }
    @catch(...)
    {
        luaL_error(L, "firebase: log event exception. \"%s\"", name);
    }
}

#endif
