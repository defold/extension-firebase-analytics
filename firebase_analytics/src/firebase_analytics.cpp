#define EXTENSION_NAME FirebaseAnalyticsExt
#define LIB_NAME "FirebaseAnalytics"
#define MODULE_NAME "firebase"
#define DLIB_LOG_DOMAIN LIB_NAME

#include <stdlib.h>

#include <dmsdk/dlib/log.h>
#include <dmsdk/sdk.h>
#include "luautils.h"

#include "firebase_analytics_private.h"
#include "firebase_analytics_callback.h"

#if defined(DM_PLATFORM_ANDROID) || defined(DM_PLATFORM_IOS)

#include "cpp_sdk_includes/event_names.h"
#include "cpp_sdk_includes/parameter_names.h"
#include "cpp_sdk_includes/user_property_names.h"
using namespace firebase::analytics;

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
    lua_pushtablestringstring(L, "EVENT_ADIMPRESSION", kEventAdImpression);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventAddPaymentInfo);
    lua_pushtablestringstring(L, "EVENT_ADDSHIPPINGINFO", kEventAddShippingInfo);
    lua_pushtablestringstring(L, "EVENT_ADDTOCART", kEventAddToCart);
    lua_pushtablestringstring(L, "EVENT_ADDTOWISHLIST", kEventAddToWishlist);
    lua_pushtablestringstring(L, "EVENT_APPOPEN", kEventAppOpen);
    lua_pushtablestringstring(L, "EVENT_BEGINCHECKOUT", kEventBeginCheckout);
    lua_pushtablestringstring(L, "EVENT_CAMPAIGNDETAILS", kEventCampaignDetails);
    lua_pushtablestringstring(L, "EVENT_EARNVIRTUALCURRENCY", kEventEarnVirtualCurrency);
    lua_pushtablestringstring(L, "EVENT_GENERATELEAD", kEventGenerateLead);
    lua_pushtablestringstring(L, "EVENT_JOINGROUP", kEventJoinGroup);
    lua_pushtablestringstring(L, "EVENT_LEVELEND", kEventLevelEnd);
    lua_pushtablestringstring(L, "EVENT_LEVELSTART", kEventLevelStart);
    lua_pushtablestringstring(L, "EVENT_LEVELUP", kEventLevelUp);
    lua_pushtablestringstring(L, "EVENT_LOGIN", kEventLogin);
    lua_pushtablestringstring(L, "EVENT_POSTSCORE", kEventPostScore);
    lua_pushtablestringstring(L, "EVENT_PURCHASE", kEventPurchase);
    lua_pushtablestringstring(L, "EVENT_REFUND", kEventRefund);
    lua_pushtablestringstring(L, "EVENT_REMOVEFROMCART", kEventRemoveFromCart);
    lua_pushtablestringstring(L, "EVENT_SCREENVIEW", kEventScreenView);
    lua_pushtablestringstring(L, "EVENT_SEARCH", kEventSearch);
    lua_pushtablestringstring(L, "EVENT_SELECTCONTENT", kEventSelectContent);
    lua_pushtablestringstring(L, "EVENT_SELECTITEM", kEventSelectItem);
    lua_pushtablestringstring(L, "EVENT_SELECTPROMOTION", kEventSelectPromotion);
    lua_pushtablestringstring(L, "EVENT_SHARE", kEventShare);
    lua_pushtablestringstring(L, "EVENT_SIGNUP", kEventSignUp);
    lua_pushtablestringstring(L, "EVENT_SPENDVIRTUALCURRENCY", kEventSpendVirtualCurrency);
    lua_pushtablestringstring(L, "EVENT_TUTORIALBEGIN", kEventTutorialBegin);
    lua_pushtablestringstring(L, "EVENT_TUTORIALCOMPLETE", kEventTutorialComplete);
    lua_pushtablestringstring(L, "EVENT_UNLOCKACHIEVEMENT", kEventUnlockAchievement);
    lua_pushtablestringstring(L, "EVENT_VIEWCART", kEventViewCart);
    lua_pushtablestringstring(L, "EVENT_VIEWITEM", kEventViewItem);
    lua_pushtablestringstring(L, "EVENT_VIEWITEMLIST", kEventViewItemList);
    lua_pushtablestringstring(L, "EVENT_VIEWPROMOTION", kEventViewPromotion);
    lua_pushtablestringstring(L, "EVENT_VIEWSEARCHRESULTS", kEventViewSearchResults);
    // From cpp_sdk_includes/parameter_names.h
    lua_pushtablestringstring(L, "PARAM_ADFORMAT", kParameterAdFormat);
    lua_pushtablestringstring(L, "PARAM_ADNETWORKCLICKID", kParameterAdNetworkClickID);
    lua_pushtablestringstring(L, "PARAM_ADPLATFORM", kParameterAdPlatform);
    lua_pushtablestringstring(L, "PARAM_ADSOURCE", kParameterAdSource);
    lua_pushtablestringstring(L, "PARAM_ADUNITNAME", kParameterAdUnitName);
    lua_pushtablestringstring(L, "PARAM_AFFILIATION", kParameterAffiliation);
    lua_pushtablestringstring(L, "PARAM_CP1", kParameterCP1);
    lua_pushtablestringstring(L, "PARAM_CAMPAIGN", kParameterCampaign);
    lua_pushtablestringstring(L, "PARAM_CAMPAIGNID", kParameterCampaignID);
    lua_pushtablestringstring(L, "PARAM_CHARACTER", kParameterCharacter);
    lua_pushtablestringstring(L, "PARAM_CONTENT", kParameterContent);
    lua_pushtablestringstring(L, "PARAM_CONTENTTYPE", kParameterContentType);
    lua_pushtablestringstring(L, "PARAM_COUPON", kParameterCoupon);
    lua_pushtablestringstring(L, "PARAM_CREATIVEFORMAT", kParameterCreativeFormat);
    lua_pushtablestringstring(L, "PARAM_CREATIVENAME", kParameterCreativeName);
    lua_pushtablestringstring(L, "PARAM_CREATIVESLOT", kParameterCreativeSlot);
    lua_pushtablestringstring(L, "PARAM_CURRENCY", kParameterCurrency);
    lua_pushtablestringstring(L, "PARAM_DESTINATION", kParameterDestination);
    lua_pushtablestringstring(L, "PARAM_DISCOUNT", kParameterDiscount);
    lua_pushtablestringstring(L, "PARAM_ENDDATE", kParameterEndDate);
    lua_pushtablestringstring(L, "PARAM_EXTENDSESSION", kParameterExtendSession);
    lua_pushtablestringstring(L, "PARAM_FLIGHTNUMBER", kParameterFlightNumber);
    lua_pushtablestringstring(L, "PARAM_GROUPID", kParameterGroupID);
    lua_pushtablestringstring(L, "PARAM_INDEX", kParameterIndex);
    lua_pushtablestringstring(L, "PARAM_ITEMBRAND", kParameterItemBrand);
    lua_pushtablestringstring(L, "PARAM_ITEMCATEGORY", kParameterItemCategory);
    lua_pushtablestringstring(L, "PARAM_ITEMCATEGORY2", kParameterItemCategory2);
    lua_pushtablestringstring(L, "PARAM_ITEMCATEGORY3", kParameterItemCategory3);
    lua_pushtablestringstring(L, "PARAM_ITEMCATEGORY4", kParameterItemCategory4);
    lua_pushtablestringstring(L, "PARAM_ITEMCATEGORY5", kParameterItemCategory5);
    lua_pushtablestringstring(L, "PARAM_ITEMID", kParameterItemID);
    lua_pushtablestringstring(L, "PARAM_ITEMLISTID", kParameterItemListID);
    lua_pushtablestringstring(L, "PARAM_ITEMLISTNAME", kParameterItemListName);
    lua_pushtablestringstring(L, "PARAM_ITEMNAME", kParameterItemName);
    lua_pushtablestringstring(L, "PARAM_ITEMVARIANT", kParameterItemVariant);
    lua_pushtablestringstring(L, "PARAM_ITEMS", kParameterItems);
    lua_pushtablestringstring(L, "PARAM_LEVEL", kParameterLevel);
    lua_pushtablestringstring(L, "PARAM_LEVELNAME", kParameterLevelName);
    lua_pushtablestringstring(L, "PARAM_LOCATION", kParameterLocation);
    lua_pushtablestringstring(L, "PARAM_LOCATIONID", kParameterLocationID);
    lua_pushtablestringstring(L, "PARAM_MARKETINGTACTIC", kParameterMarketingTactic);
    lua_pushtablestringstring(L, "PARAM_MEDIUM", kParameterMedium);
    lua_pushtablestringstring(L, "PARAM_METHOD", kParameterMethod);
    lua_pushtablestringstring(L, "PARAM_NUMBEROFNIGHTS", kParameterNumberOfNights);
    lua_pushtablestringstring(L, "PARAM_NUMBEROFPASSENGERS", kParameterNumberOfPassengers);
    lua_pushtablestringstring(L, "PARAM_NUMBEROFROOMS", kParameterNumberOfRooms);
    lua_pushtablestringstring(L, "PARAM_ORIGIN", kParameterOrigin);
    lua_pushtablestringstring(L, "PARAM_PAYMENTTYPE", kParameterPaymentType);
    lua_pushtablestringstring(L, "PARAM_PRICE", kParameterPrice);
    lua_pushtablestringstring(L, "PARAM_PROMOTIONID", kParameterPromotionID);
    lua_pushtablestringstring(L, "PARAM_PROMOTIONNAME", kParameterPromotionName);
    lua_pushtablestringstring(L, "PARAM_QUANTITY", kParameterQuantity);
    lua_pushtablestringstring(L, "PARAM_SCORE", kParameterScore);
    lua_pushtablestringstring(L, "PARAM_SCREENCLASS", kParameterScreenClass);
    lua_pushtablestringstring(L, "PARAM_SCREENNAME", kParameterScreenName);
    lua_pushtablestringstring(L, "PARAM_SEARCHTERM", kParameterSearchTerm);
    lua_pushtablestringstring(L, "PARAM_SHIPPING", kParameterShipping);
    lua_pushtablestringstring(L, "PARAM_SHIPPINGTIER", kParameterShippingTier);
    lua_pushtablestringstring(L, "PARAM_SOURCE", kParameterSource);
    lua_pushtablestringstring(L, "PARAM_SOURCEPLATFORM", kParameterSourcePlatform);
    lua_pushtablestringstring(L, "PARAM_STARTDATE", kParameterStartDate);
    lua_pushtablestringstring(L, "PARAM_SUCCESS", kParameterSuccess);
    lua_pushtablestringstring(L, "PARAM_TAX", kParameterTax);
    lua_pushtablestringstring(L, "PARAM_TERM", kParameterTerm);
    lua_pushtablestringstring(L, "PARAM_TRANSACTIONID", kParameterTransactionID);
    lua_pushtablestringstring(L, "PARAM_TRAVELCLASS", kParameterTravelClass);
    lua_pushtablestringstring(L, "PARAM_VALUE", kParameterValue);
    lua_pushtablestringstring(L, "PARAM_VIRTUALCURRENCYNAME", kParameterVirtualCurrencyName);
    // From cpp_sdk_includes/user_property_names.h
    lua_pushtablestringstring(L, "PROP_ALLOWADPERSONALIZATIONSIGNALS", kUserPropertyAllowAdPersonalizationSignals);
    lua_pushtablestringstring(L, "PROP_SIGNUPMETHOD", kUserPropertySignUpMethod);
    

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

