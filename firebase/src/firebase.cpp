#define LIB_NAME "Firebase"
#define MODULE_NAME "firebase"
#define DLIB_LOG_DOMAIN LIB_NAME
#include <dmsdk/dlib/log.h>
#include "luautils.h"
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_ANDROID) || defined(DM_PLATFORM_IOS)

#include "firebase/app.h"
#include "firebase/analytics.h"
#include "firebase/analytics/event_names.h"
#include "firebase/analytics/parameter_names.h"
#include "firebase/future.h"


static firebase::App* firebase_app_;

using namespace firebase;
using namespace firebase::analytics;


#if defined(DM_PLATFORM_ANDROID)
static JNIEnv* GetJNIEnv()
{
	JNIEnv* env = 0;
	dmGraphics::GetNativeAndroidJavaVM()->AttachCurrentThread(&env, NULL);
	return env;
}

static void Firebase_Safe_LogEvent(lua_State*, const char* name, const Parameter* params, size_t number_of_parameters)
{
	LogEvent(name, params, number_of_parameters);
}

#else if defined(DM_PLATFORM_IOS)

void Firebase_Safe_LogEvent(lua_State*, const char* name, const Parameter* params, size_t number_of_parameters);

#endif


static int Firebase_Init(lua_State* L) {
	dmLogInfo("Firebase_Init");
	int top = lua_gettop(L);

	#if defined(DM_PLATFORM_ANDROID)
	dmLogInfo("Creating app");
	firebase_app_ = App::Create(GetJNIEnv(), dmGraphics::GetNativeAndroidActivity());
	#else
	dmLogInfo("Creating app");
	firebase_app_ = App::Create();
	#endif

	if(!firebase_app_) {
		dmLogError("firebase::App::Create failed");
		return 0;
	}
	analytics::Initialize(*firebase_app_);

	dmLogInfo("Firebase_Init done");
	assert(top == lua_gettop(L));
	return 0;
}


lua_Listener getInstanceIdListener;

static int Firebase_Analytics_InstanceId(lua_State* L) {
	int top = lua_gettop(L);

	luaL_checklistener(L, 1, getInstanceIdListener);

	Future<std::string> future = analytics::GetAnalyticsInstanceId();
	future.OnCompletion([](const Future< std::string >& completed_future, void* user_data) {
			lua_State* L = (lua_State*)user_data;
			if (completed_future.error() == 0) {
				lua_pushlistener(L, getInstanceIdListener);
				lua_pushstring(L, completed_future.result()->c_str());
				int ret = lua_pcall(L, 2, 0, 0);
				if (ret != 0) {
					lua_pop(L, 1);
				}
			}
			else {
				dmLogError("%d: %s", completed_future.error(), completed_future.error_message());
				lua_pushlistener(L, getInstanceIdListener);
				lua_pushnil(L);
				lua_pushstring(L, completed_future.error_message());
				int ret = lua_pcall(L, 3, 0, 0);
				if (ret != 0) {
					lua_pop(L, 2);
				}
			}
		}, L);

	assert(top == lua_gettop(L));
	return 0;
}

static int Firebase_Analytics_Log(lua_State* L) {
	int top = lua_gettop(L);

	const char* name = luaL_checkstring(L, 1);
	LogEvent(name);

	assert(top == lua_gettop(L));
	return 0;
}

static int Firebase_Analytics_LogString(lua_State* L) {
	int top = lua_gettop(L);

	const char* name = luaL_checkstring(L, 1);
	const char* parameter_name = luaL_checkstring(L, 2);
	const char* parameter_value = luaL_checkstring(L, 3);
	LogEvent(name, parameter_name, parameter_value);

	assert(top == lua_gettop(L));
	return 0;
}

static int Firebase_Analytics_LogInt(lua_State* L) {
	int top = lua_gettop(L);

	const char* name = luaL_checkstring(L, 1);
	const char* parameter_name = luaL_checkstring(L, 2);
	const int parameter_value = luaL_checkint(L, 3);
	LogEvent(name, parameter_name, parameter_value);

	assert(top == lua_gettop(L));
	return 0;
}

static int Firebase_Analytics_LogNumber(lua_State* L) {
	int top = lua_gettop(L);

	const char* name = luaL_checkstring(L, 1);
	const char* parameter_name = luaL_checkstring(L, 2);
	const lua_Number parameter_value = luaL_checknumber(L, 3);
	LogEvent(name, parameter_name, parameter_value);

	assert(top == lua_gettop(L));
	return 0;
}

const size_t MAX_ELEMENTS = 25; //Specified in Firebase docs

static int Firebase_Analytics_LogTable(lua_State* L) {
	int top = lua_gettop(L);

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
			assert(top == lua_gettop(L));
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
				assert(top == lua_gettop(L));
			return 0;
			break;
		}
		lua_pop(L, 1);
		++size;
	}

	Firebase_Safe_LogEvent(L, name, params, size);

	lua_pop(L, 1);
	assert(top == lua_gettop(L));
	return 0;
}

static int Firebase_Analytics_Reset(lua_State* L) {
	int top = lua_gettop(L);
	ResetAnalyticsData();
	assert(top == lua_gettop(L));
	return 0;
}

static int Firebase_Analytics_SetEnabled(lua_State* L) {
	int top = lua_gettop(L);
	int enabled = lua_toboolean(L, 1);
	SetAnalyticsCollectionEnabled((bool)enabled);
	assert(top == lua_gettop(L));
	return 0;
}

static int Firebase_Analytics_SetScreen(lua_State* L) {
	int top = lua_gettop(L);

	const char* screen_name = luaL_checkstring(L, 1);
	const char* screen_class = luaL_checkstring(L, 2);
	SetCurrentScreen(screen_name, screen_class);

	assert(top == lua_gettop(L));
	return 0;
}

static int Firebase_Analytics_SetUserProperty(lua_State* L) {
	int top = lua_gettop(L);

	const char* name = luaL_checkstring(L, 1);
	const char* property = luaL_checkstring(L, 2);
	SetUserProperty(name, property);

	assert(top == lua_gettop(L));
	return 0;
}

static int Firebase_Analytics_SetUserId(lua_State* L) {
	int top = lua_gettop(L);

	const char* user_id = luaL_checkstring(L, 1);
	SetUserId(user_id);

	assert(top == lua_gettop(L));
	return 0;
}

static int Firebase_Analytics_SetMinimumSessionDuration(lua_State* L) {
	int top = lua_gettop(L);
	int duration = luaL_checkint(L, 1);
	SetMinimumSessionDuration(duration);
	assert(top == lua_gettop(L));
	return 0;
}


static const luaL_reg Module_methods[] = {
	{"init", Firebase_Init},
	{0, 0}
};

static void LuaInit(lua_State* L) {
	int top = lua_gettop(L);
	luaL_register(L, MODULE_NAME, Module_methods);

	lua_pushstring(L, "analytics");
	lua_newtable(L);
	lua_pushtablestringfunction(L, "get_id", Firebase_Analytics_InstanceId);
	lua_pushtablestringfunction(L, "log", Firebase_Analytics_Log);
	lua_pushtablestringfunction(L, "log_string", Firebase_Analytics_LogString);
	lua_pushtablestringfunction(L, "log_int", Firebase_Analytics_LogInt);
	lua_pushtablestringfunction(L, "log_number", Firebase_Analytics_LogNumber);
	lua_pushtablestringfunction(L, "log_table", Firebase_Analytics_LogTable);
	lua_pushtablestringfunction(L, "reset", Firebase_Analytics_Reset);
	lua_pushtablestringfunction(L, "set_enabled", Firebase_Analytics_SetEnabled);
	lua_pushtablestringfunction(L, "set_screen", Firebase_Analytics_SetScreen);
	lua_pushtablestringfunction(L, "set_user_property", Firebase_Analytics_SetUserProperty);
	lua_pushtablestringfunction(L, "set_user_id", Firebase_Analytics_SetUserId);
	lua_pushtablestringfunction(L, "set_minimum_session_duration", Firebase_Analytics_SetMinimumSessionDuration);
	lua_settable(L, -3);

	lua_pop(L, 1);

	assert(top == lua_gettop(L));
}

#endif

dmExtension::Result AppInitializeFirebaseExtension(dmExtension::AppParams* params) {
	return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeFirebaseExtension(dmExtension::Params* params) {
	#if !defined(DM_PLATFORM_ANDROID) && !defined(DM_PLATFORM_IOS)
		printf("Extension %s is not supported\n", MODULE_NAME);
	#else
		LuaInit(params->m_L);
	#endif
	return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeFirebaseExtension(dmExtension::AppParams* params) {
	return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeFirebaseExtension(dmExtension::Params* params) {
	return dmExtension::RESULT_OK;
}

dmExtension::Result UpdateFirebaseExtension(dmExtension::Params* params) {
	return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(Firebase, LIB_NAME, AppInitializeFirebaseExtension, AppFinalizeFirebaseExtension, InitializeFirebaseExtension, UpdateFirebaseExtension, 0, FinalizeFirebaseExtension)
