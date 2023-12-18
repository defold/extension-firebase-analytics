#if defined(DM_PLATFORM_ANDROID) || defined(DM_PLATFORM_IOS)
#pragma once

#include <dmsdk/sdk.h>

namespace dmFirebaseAnalytics {

enum Message
{
    MSG_ERROR               = 0,
    MSG_INITIALIZED         = 1,
    MSG_DEFAULTS_SET        = 2,
    MSG_SETTINGS_UPDATED    = 3,
    MSG_FETCHED             = 4,
    MSG_ACTIVATED           = 5,
};


struct CallbackData
{
    Message msg;
    char* json;
};

void SetLuaCallback(lua_State* L, int pos);
void UpdateCallback();
void InitializeCallback();
void FinalizeCallback();

void AddToQueueCallback(Message type, const char* json);

} //namespace dmFirebaseAnalytics

#endif // defined(DM_PLATFORM_ANDROID) || defined(DM_PLATFORM_IOS)
