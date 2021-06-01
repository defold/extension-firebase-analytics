#define LIB_NAME "FirebaseAnalytics"
#define DLIB_LOG_DOMAIN LIB_NAME
#include "luautils.h"
#include <dmsdk/dlib/log.h>
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_ANDROID) || defined(DM_PLATFORM_IOS)

#include "firebase/app.h"
#include "firebase/analytics.h"
#include "firebase/analytics/event_names.h"
#include "firebase/analytics/parameter_names.h"
#include "firebase/future.h"

#include "firebase.h"
#include "firebase_analytics.h"


using namespace firebase;
using namespace firebase::analytics;

static dmScript::LuaCallbackInfo* g_FirebaseAnalytics_InstanceIdCallback;
static bool g_FirebaseAnalytics_Initialized = false;

#if defined(DM_PLATFORM_ANDROID)
void FirebaseAnalytics_Safe_LogEvent(lua_State*, const char* name, const firebase::analytics::Parameter* params, size_t number_of_parameters)
{
	LogEvent(name, params, number_of_parameters);
}
#endif


static int FirebaseAnalytics_Init(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);
	const firebase::App* app = Firebase_GetFirebaseApp();
	analytics::Initialize(*app);
	g_FirebaseAnalytics_Initialized = true;
	return 0;
}


static int FirebaseAnalytics_Analytics_InstanceId(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);
	if (!g_FirebaseAnalytics_Initialized)
	{
		dmLogWarning("Firebase Analytics has not been initialized! Make sure to call firebase.analytics.init().");
		return 0;
	}
	g_FirebaseAnalytics_InstanceIdCallback = dmScript::CreateCallback(L, 1);

	analytics::GetAnalyticsInstanceId()
	.OnCompletion([](const Future< std::string >& completed_future) {
		if (!dmScript::IsCallbackValid(g_FirebaseAnalytics_InstanceIdCallback))
		{
			dmLogWarning("Analytics InstanceId callback is not valid");
			return;
		}

		if (dmScript::SetupCallback(g_FirebaseAnalytics_InstanceIdCallback))
		{
			lua_State* L = dmScript::GetCallbackLuaContext(g_FirebaseAnalytics_InstanceIdCallback);

			if (completed_future.error() == 0) {
				lua_pushstring(L, completed_future.result()->c_str());
				int ret = lua_pcall(L, 2, 0, 0);
				if (ret != 0) {
					lua_pop(L, 1);
				}
			}
			else {
				dmLogError("%d: %s", completed_future.error(), completed_future.error_message());
				lua_pushnil(L);
				lua_pushstring(L, completed_future.error_message());
				int ret = lua_pcall(L, 3, 0, 0);
				if (ret != 0) {
					lua_pop(L, 2);
				}
			}
			dmScript::TeardownCallback(g_FirebaseAnalytics_InstanceIdCallback);
		}

		dmScript::DestroyCallback(g_FirebaseAnalytics_InstanceIdCallback);
		g_FirebaseAnalytics_InstanceIdCallback = 0;
	});
	return 0;
}

static int FirebaseAnalytics_Analytics_Log(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);
	if (!g_FirebaseAnalytics_Initialized)
	{
		dmLogWarning("Firebase Analytics has not been initialized! Make sure to call firebase.analytics.init().");
		return 0;
	}
	const char* name = luaL_checkstring(L, 1);
	LogEvent(name);
	return 0;
}

static int FirebaseAnalytics_Analytics_LogString(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);
	if (!g_FirebaseAnalytics_Initialized)
	{
		dmLogWarning("Firebase Analytics has not been initialized! Make sure to call firebase.analytics.init().");
		return 0;
	}
	const char* name = luaL_checkstring(L, 1);
	const char* parameter_name = luaL_checkstring(L, 2);
	const char* parameter_value = luaL_checkstring(L, 3);
	LogEvent(name, parameter_name, parameter_value);
	return 0;
}

static int FirebaseAnalytics_Analytics_LogInt(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);
	if (!g_FirebaseAnalytics_Initialized)
	{
		dmLogWarning("Firebase Analytics has not been initialized! Make sure to call firebase.analytics.init().");
		return 0;
	}
	const char* name = luaL_checkstring(L, 1);
	const char* parameter_name = luaL_checkstring(L, 2);
	const int parameter_value = luaL_checkint(L, 3);
	LogEvent(name, parameter_name, parameter_value);
	return 0;
}

static int FirebaseAnalytics_Analytics_LogNumber(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);
	if (!g_FirebaseAnalytics_Initialized)
	{
		dmLogWarning("Firebase Analytics has not been initialized! Make sure to call firebase.analytics.init().");
		return 0;
	}
	const char* name = luaL_checkstring(L, 1);
	const char* parameter_name = luaL_checkstring(L, 2);
	const lua_Number parameter_value = luaL_checknumber(L, 3);
	LogEvent(name, parameter_name, parameter_value);
	return 0;
}

const size_t MAX_ELEMENTS = 25; //Specified in Firebase docs

static int FirebaseAnalytics_Analytics_LogTable(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);
	if (!g_FirebaseAnalytics_Initialized)
	{
		dmLogWarning("Firebase Analytics has not been initialized! Make sure to call firebase.analytics.init().");
		return 0;
	}

	analytics::Parameter params[MAX_ELEMENTS];

	const char* name = luaL_checkstring(L, 1);
	luaL_checktype(L, 2, LUA_TTABLE);

	lua_pushvalue(L, 2);
	lua_pushnil(L);
	int size = 0;
	while (lua_next(L, -2) != 0)
	{
		if (size == MAX_ELEMENTS) {
			char msg[256];
			snprintf(msg, sizeof(msg), "Too many parameters in '%s'", name);
			luaL_error(L, msg);
			lua_pop(L, 2);
			return 0;
		}
		const char* k = lua_tostring(L, -2);
		int t = lua_type(L, -1);
		switch (t) {
			case LUA_TSTRING:
				params[size] = analytics::Parameter(k, lua_tostring(L, -1));
			break;
			case LUA_TBOOLEAN:
				params[size] = analytics::Parameter(k, lua_toboolean(L, -1) != 0);
			break;
			case LUA_TNUMBER:
				params[size] = analytics::Parameter(k, lua_tonumber(L, -1));
			break;
			default:  /* other values */
				char msg[256];
				snprintf(msg, sizeof(msg), "Wrong type for table attribute '%s' , type: '%s'", k, luaL_typename(L, -1));
				luaL_error(L, msg);
				lua_pop(L, 3);
				return 0;
			break;
		}
		lua_pop(L, 1);
		++size;
	}

	FirebaseAnalytics_Safe_LogEvent(L, name, params, size);

	lua_pop(L, 1);
	return 0;
}

static int FirebaseAnalytics_Analytics_Reset(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);
	if (!g_FirebaseAnalytics_Initialized)
	{
		dmLogWarning("Firebase Analytics has not been initialized! Make sure to call firebase.analytics.init().");
		return 0;
	}
	ResetAnalyticsData();
	return 0;
}

static int FirebaseAnalytics_Analytics_SetEnabled(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);
	if (!g_FirebaseAnalytics_Initialized)
	{
		dmLogWarning("Firebase Analytics has not been initialized! Make sure to call firebase.analytics.init().");
		return 0;
	}
	int enabled = lua_toboolean(L, 1);
	SetAnalyticsCollectionEnabled((bool)enabled);
	return 0;
}

static int FirebaseAnalytics_Analytics_SetScreen(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);
	if (!g_FirebaseAnalytics_Initialized)
	{
		dmLogWarning("Firebase Analytics has not been initialized! Make sure to call firebase.analytics.init().");
		return 0;
	}
	const char* screen_name = luaL_checkstring(L, 1);
	const char* screen_class = luaL_checkstring(L, 2);
	SetCurrentScreen(screen_name, screen_class);
	return 0;
}

static int FirebaseAnalytics_Analytics_SetUserProperty(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);
	if (!g_FirebaseAnalytics_Initialized)
	{
		dmLogWarning("Firebase Analytics has not been initialized! Make sure to call firebase.analytics.init().");
		return 0;
	}
	const char* name = luaL_checkstring(L, 1);
	const char* property = luaL_checkstring(L, 2);
	SetUserProperty(name, property);
	return 0;
}

static int FirebaseAnalytics_Analytics_SetUserId(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);
	if (!g_FirebaseAnalytics_Initialized)
	{
		dmLogWarning("Firebase Analytics has not been initialized! Make sure to call firebase.analytics.init().");
		return 0;
	}
	const char* user_id = luaL_checkstring(L, 1);
	SetUserId(user_id);
	return 0;
}


static void LuaInit(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);

	// get or create firebase global table
	lua_getglobal(L, "firebase");
	if (lua_isnil(L, lua_gettop(L)))
	{
		lua_pushstring(L, "firebase");
		lua_newtable(L);
		lua_settable(L, LUA_GLOBALSINDEX);
		lua_pop(L, 1);
		lua_getglobal(L, "firebase");
	}

	lua_pushstring(L, "analytics");
	lua_newtable(L);
	lua_pushtablestringfunction(L, "init", FirebaseAnalytics_Init);
	lua_pushtablestringfunction(L, "get_id", FirebaseAnalytics_Analytics_InstanceId);
	lua_pushtablestringfunction(L, "log", FirebaseAnalytics_Analytics_Log);
	lua_pushtablestringfunction(L, "log_string", FirebaseAnalytics_Analytics_LogString);
	lua_pushtablestringfunction(L, "log_int", FirebaseAnalytics_Analytics_LogInt);
	lua_pushtablestringfunction(L, "log_number", FirebaseAnalytics_Analytics_LogNumber);
	lua_pushtablestringfunction(L, "log_table", FirebaseAnalytics_Analytics_LogTable);
	lua_pushtablestringfunction(L, "reset", FirebaseAnalytics_Analytics_Reset);
	lua_pushtablestringfunction(L, "set_enabled", FirebaseAnalytics_Analytics_SetEnabled);
	lua_pushtablestringfunction(L, "set_screen", FirebaseAnalytics_Analytics_SetScreen);
	lua_pushtablestringfunction(L, "set_user_property", FirebaseAnalytics_Analytics_SetUserProperty);
	lua_pushtablestringfunction(L, "set_user_id", FirebaseAnalytics_Analytics_SetUserId);
	lua_settable(L, -3);

	lua_pop(L, 1); // pop "firebase" global table
}

#endif

dmExtension::Result AppInitializeFirebaseAnalyticsExtension(dmExtension::AppParams* params) {
	return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeFirebaseAnalyticsExtension(dmExtension::Params* params) {
	#if !defined(DM_PLATFORM_ANDROID) && !defined(DM_PLATFORM_IOS)
		dmLogInfo("Extension %s is not supported", LIB_NAME);
	#else
		dmLogInfo("Extension %s is supported", LIB_NAME);
		LuaInit(params->m_L);
	#endif
	return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeFirebaseAnalyticsExtension(dmExtension::AppParams* params) {
	return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeFirebaseAnalyticsExtension(dmExtension::Params* params) {
	return dmExtension::RESULT_OK;
}

dmExtension::Result UpdateFirebaseAnalyticsExtension(dmExtension::Params* params) {
	return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(FirebaseAnalytics, LIB_NAME, AppInitializeFirebaseAnalyticsExtension, AppFinalizeFirebaseAnalyticsExtension, InitializeFirebaseAnalyticsExtension, UpdateFirebaseAnalyticsExtension, 0, FinalizeFirebaseAnalyticsExtension)
