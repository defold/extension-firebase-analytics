#define EXTENSION_NAME FirebaseAnalyticsExt
#define LIB_NAME "FirebaseAnalytics"
#define MODULE_NAME "firebase"
#define DLIB_LOG_DOMAIN LIB_NAME

#include <stdlib.h>

#include <dmsdk/dlib/log.h>
#include <dmsdk/sdk.h>

#include "firebase_analytics_private.h"
#include "firebase_analytics_callback.h"

#if defined(DM_PLATFORM_ANDROID) || defined(DM_PLATFORM_IOS1)

namespace dmFirebaseAnalytics {

static int Lua_Initialize(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);
	Initialize();
	return 0;
}

static int Lua_SetCallback(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	SetLuaCallback(L, 1);
	return 0;
}

static int Lua_GetInstanceId(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	// GetInstanceId();
	return 0;
}

static int Lua_Log(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	// SetLuaCallback(L, 1);
	return 0;
}

static int Lua_LogString(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	const char* param_name = luaL_checkstring(L, 1);
	const char* param = luaL_checkstring(L, 2);
	const char* event_name = luaL_checkstring(L, 3);
	LogEventString(param_name, param, event_name);
	return 0;
}

static int Lua_LogInt(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	const char* param_name = luaL_checkstring(L, 1);
	const int param = luaL_checkint(L, 2);
	const char* event_name = luaL_checkstring(L, 3);
	LogEventInt(param_name, param, event_name);
	return 0;
}

static int Lua_LogNumber(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	const char* param_name = luaL_checkstring(L, 1);
	const lua_Number param = luaL_checknumber(L, 2);
	const char* event_name = luaL_checkstring(L, 3);
	LogEventNumber(param_name, param, event_name);
	return 0;
}

static int Lua_LogTable(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	// SetLuaCallback(L, 1);
	return 0;
}

static int Lua_Reset(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	ResetAnalyticsData();
	return 0;
}

static int Lua_SetEnabled(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	int enabled = lua_toboolean(L, 1);
	SetAnalyticsCollectionEnabled(enabled);
	return 0;
}

static int Lua_SetScreen(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	dmLogWarning("set_screen() has been deprecated. Use log_string() and EVENT_SCREENVIEW instead.");
	return 0;
}

static int Lua_SetUserProperty(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	const char* name = luaL_checkstring(L, 1);
	const char* property = luaL_checkstring(L, 2);
	SetUserProperty(name, property);
	return 0;
}

static int Lua_LogEventPurchaseRevenue(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);
	const char* transaction_id = luaL_checkstring(L, 1);
	const char* sku = luaL_checkstring(L, 2);
	const char* currency = luaL_checkstring(L, 3);
	const lua_Number value = luaL_checknumber(L, 4);
	LogEventPurchaseRevenue(transaction_id, sku, currency, value);
	return 0;
}

static int Lua_LogEventAdRevenue(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);
	const char* unit_id = luaL_checkstring(L, 1);
	const char* adNetwork = luaL_checkstring(L, 2);
	const lua_Number value = luaL_checknumber(L, 3);
	LogEventAdRevenue(unit_id, adNetwork, value);
	return 0;
}


static int Lua_SetUserId(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	const char* user_id = luaL_checkstring(L, 1);
	SetUserId(user_id);
	return 0;
}

static const luaL_reg Module_methods[] =
{
	{"initialize", Lua_Initialize},
	{"set_callback", Lua_SetCallback},

	{"get_id", Lua_GetInstanceId},
	{"log", Lua_Log},
	{"log_string", Lua_LogString},
	{"log_int", Lua_LogInt},
	{"log_number", Lua_LogNumber},
	{"log_table", Lua_LogTable},
	{"log_purchase_revenue", Lua_LogEventPurchaseRevenue},
	{"log_event_ad_revenue", Lua_LogEventAdRevenue},
	{"reset", Lua_Reset},
	{"set_enabled", Lua_SetEnabled},
	{"set_screen", Lua_SetScreen},
	{"set_user_property", Lua_SetUserProperty},

	{"set_user_id", Lua_SetUserId},

	{0, 0}
};


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

	luaL_register(L, NULL, Module_methods);
	

    #define SETCONSTANT(name) \
	lua_pushnumber(L, (lua_Number) name); \
	lua_setfield(L, -2, #name); \

	SETCONSTANT(MSG_ERROR)
	SETCONSTANT(MSG_INITIALIZED)
	SETCONSTANT(MSG_DEFAULTS_SET)
	SETCONSTANT(MSG_SETTINGS_UPDATED)
	SETCONSTANT(MSG_FETCHED)
	SETCONSTANT(MSG_ACTIVATED)

    #undef SETCONSTANT

	lua_settable(L, -3);

	lua_pop(L, 1);
}


//--------------------------- Extension callbacks -------------------------//

dmExtension::Result AppInitializeFirebaseExtension(dmExtension::AppParams* params) {
	return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeFirebaseExtension(dmExtension::Params* params) {
	dmLogInfo("Initialize Firebase Analytics");

	LuaInit(params->m_L);
	Initialize_Ext();
	InitializeCallback();

	return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeFirebaseExtension(dmExtension::AppParams* params) {
	return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeFirebaseExtension(dmExtension::Params* params) {
	FinalizeCallback();
	return dmExtension::RESULT_OK;
}

dmExtension::Result UpdateFirebaseExtension(dmExtension::Params* params) {
	UpdateCallback();
	return dmExtension::RESULT_OK;
}

} // namespace dmFirebaseAnalytics

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, dmFirebaseAnalytics::AppInitializeFirebaseExtension, dmFirebaseAnalytics::AppFinalizeFirebaseExtension, dmFirebaseAnalytics::InitializeFirebaseExtension, dmFirebaseAnalytics::UpdateFirebaseExtension, 0, dmFirebaseAnalytics::FinalizeFirebaseExtension)

#else // defined(DM_PLATFORM_ANDROID) || defined(DM_PLATFORM_IOS) 

//-------------------- Not supported for other platforms --------------------//

static  dmExtension::Result InitializeFirebaseAnalytics(dmExtension::Params* params)
{
	dmLogInfo("Registered extension Firebase Analytics (null)");
	return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeFirebaseAnalytics(dmExtension::Params* params)
{
	return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, 0, 0, InitializeFirebaseAnalytics, 0, 0, FinalizeFirebaseAnalytics)

#endif // defined(DM_PLATFORM_ANDROID) || defined(DM_PLATFORM_IOS) 

