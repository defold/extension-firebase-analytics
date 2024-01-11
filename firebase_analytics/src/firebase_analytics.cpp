#define EXTENSION_NAME FirebaseAnalyticsExt
#define LIB_NAME "FirebaseAnalytics"
#define MODULE_NAME "firebase"
#define DLIB_LOG_DOMAIN LIB_NAME

#include <stdlib.h>

#include <dmsdk/dlib/log.h>
#include <dmsdk/sdk.h>

#include "firebase_analytics_private.h"
#include "firebase_analytics_callback.h"

#include "cpp_sdk_includes/event_names.h"
#include "cpp_sdk_includes/parameter_names.h"
#include "cpp_sdk_includes/user_property_names.h"

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
    GetInstanceId();
    return 0;
}

static int Lua_Log(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    const char* event_name = luaL_checkstring(L, 1);
    LogEvent(event_name);
    return 0;
}

static int Lua_LogString(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    const char* event_name = luaL_checkstring(L, 1);
    const char* param_name = luaL_checkstring(L, 2);
    const char* param = luaL_checkstring(L, 3);
    LogEventString(param_name, param, event_name);
    return 0;
}

static int Lua_LogInt(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    const char* event_name = luaL_checkstring(L, 1);
    const char* param_name = luaL_checkstring(L, 2);
    const int param = luaL_checkint(L, 3);
    LogEventInt(param_name, param, event_name);
    return 0;
}

static int Lua_LogNumber(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    const char* event_name = luaL_checkstring(L, 1);
    const char* param_name = luaL_checkstring(L, 2);
    const lua_Number param = luaL_checknumber(L, 3);
    LogEventNumber(param_name, param, event_name);
    return 0;
}

static int Lua_LogTable(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    //TODO
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

static int Lua_SetUserProperty(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    const char* name = luaL_checkstring(L, 1);
    const char* property = luaL_checkstring(L, 2);
    SetUserProperty(name, property);
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
    {"reset", Lua_Reset},
    {"set_enabled", Lua_SetEnabled},
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

    // From cpp_sdk_includes/event_names.h
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventAdImpression);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventAddPaymentInfo);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventAddShippingInfo);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventAddToCart);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventAddToWishlist);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventAppOpen);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventBeginCheckout);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventCampaignDetails);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventEarnVirtualCurrency);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventGenerateLead);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventJoinGroup);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventLevelEnd);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventLevelStart);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventLevelUp);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventLogin);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventPostScore);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventPurchase);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventRefund);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventRemoveFromCart);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventScreenView);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventSearch);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventSelectContent);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventSelectItem);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventSelectPromotion);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventShare);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventSignUp);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventSpendVirtualCurrency);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventTutorialBegin);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventTutorialComplete);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventUnlockAchievement);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventViewCart);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventViewItem);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventViewItemList);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventViewPromotion);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventViewSearchResults);
    // From cpp_sdk_includes/parameter_names.h
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterAchievementID);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterAdFormat);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterAdNetworkClickID);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterAdPlatform);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterAdSource);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterAdUnitName);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterAffiliation);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterCP1);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterCampaign);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterCampaignID);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterCharacter);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterContent);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterContentType);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterCoupon);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterCreativeFormat);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterCreativeName);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterCreativeSlot);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterCurrency);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterDestination);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterDiscount);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterEndDate);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterExtendSession);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterFlightNumber);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterGroupID);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterIndex);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterItemBrand);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterItemCategory);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterItemCategory2);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterItemCategory3);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterItemCategory4);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterItemCategory5);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterItemID);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterItemListID);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterItemListName);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterItemName);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterItemVariant);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterItems);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterLevel);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterLevelName);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterLocation);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterLocationID);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterMarketingTactic);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterMedium);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterMethod);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterNumberOfNights);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterNumberOfPassengers);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterNumberOfRooms);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterOrigin);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterPaymentType);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterPrice);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterPromotionID);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterPromotionName);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterQuantity);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterScore);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterScreenClass);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterScreenName);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterSearchTerm);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterShipping);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterShippingTier);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterSource);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterSourcePlatform);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterStartDate);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterSuccess);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterTax);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterTerm);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterTransactionID);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterTravelClass);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterValue);
    lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterVirtualCurrencyName);
    // From cpp_sdk_includes/user_property_names.h
    lua_pushtablestringstring(L, "PROP_ACHIEVEMENTID", kUserPropertyAllowAdPersonalizationSignals);
    lua_pushtablestringstring(L, "PROP_ACHIEVEMENTID", kUserPropertySignUpMethod);
    

    #define SETCONSTANT(name) \
    lua_pushnumber(L, (lua_Number) name); \
    lua_setfield(L, -2, #name); \

    SETCONSTANT(MSG_ERROR)
    SETCONSTANT(MSG_INSTANCE_ID)

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

