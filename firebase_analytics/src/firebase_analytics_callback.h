#if defined(DM_PLATFORM_ANDROID) || defined(DM_PLATFORM_IOS)
#pragma once

#include <dmsdk/sdk.h>

namespace dmFirebaseAnalytics {

enum Message
{
    MSG_ERROR               = 0,
    MSG_INSTANCE_ID         = 1,
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
