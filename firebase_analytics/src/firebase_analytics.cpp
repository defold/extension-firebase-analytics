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

	// From firebase/analytics/event_names.h
	lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventAddPaymentInfo);
	lua_pushtablestringstring(L, "EVENT_ADDTOCART", kEventAddToCart);
	lua_pushtablestringstring(L, "EVENT_ADDTOWISHLIST", kEventAddToWishlist);
	lua_pushtablestringstring(L, "EVENT_ADIMPRESSION", kEventAdImpression);
	lua_pushtablestringstring(L, "EVENT_APPOPEN", kEventAppOpen);
	lua_pushtablestringstring(L, "EVENT_BEGINCHECKOUT", kEventBeginCheckout);
	lua_pushtablestringstring(L, "EVENT_CAMPAIGNDETAILS", kEventCampaignDetails);
	lua_pushtablestringstring(L, "EVENT_CHECKOUTPROGRESS", kEventCheckoutProgress);
	lua_pushtablestringstring(L, "EVENT_EARNVIRTUALCURRENCY", kEventEarnVirtualCurrency);
	lua_pushtablestringstring(L, "EVENT_ECOMMERCEPURCHASE", kEventEcommercePurchase);
	lua_pushtablestringstring(L, "EVENT_GENERATELEAD", kEventGenerateLead);
	lua_pushtablestringstring(L, "EVENT_JOINGROUP", kEventJoinGroup);
	lua_pushtablestringstring(L, "EVENT_LEVELEND", kEventLevelEnd);
	lua_pushtablestringstring(L, "EVENT_LEVELSTART", kEventLevelStart);
	lua_pushtablestringstring(L, "EVENT_LEVELUP", kEventLevelUp);
	lua_pushtablestringstring(L, "EVENT_LOGIN", kEventLogin);
	lua_pushtablestringstring(L, "EVENT_POSTSCORE", kEventPostScore);
	lua_pushtablestringstring(L, "EVENT_PRESENTOFFER", kEventPresentOffer);
	lua_pushtablestringstring(L, "EVENT_PURCHASEREFUND", kEventPurchaseRefund);
	lua_pushtablestringstring(L, "EVENT_REMOVEFROMCART", kEventRemoveFromCart);
	lua_pushtablestringstring(L, "EVENT_SCREENVIEW", kEventScreenView);
	lua_pushtablestringstring(L, "EVENT_SEARCH", kEventSearch);
	lua_pushtablestringstring(L, "EVENT_SELECTCONTENT", kEventSelectContent);
	lua_pushtablestringstring(L, "EVENT_SETCHECKOUTOPTION", kEventSetCheckoutOption);
	lua_pushtablestringstring(L, "EVENT_SHARE", kEventShare);
	lua_pushtablestringstring(L, "EVENT_SIGNUP", kEventSignUp);
	lua_pushtablestringstring(L, "EVENT_SPENDVIRTUALCURRENCY", kEventSpendVirtualCurrency);
	lua_pushtablestringstring(L, "EVENT_TUTORIALBEGIN", kEventTutorialBegin);
	lua_pushtablestringstring(L, "EVENT_TUTORIALCOMPLETE", kEventTutorialComplete);
	lua_pushtablestringstring(L, "EVENT_UNLOCKACHIEVEMENT", kEventUnlockAchievement);
	lua_pushtablestringstring(L, "EVENT_VIEWITEM", kEventViewItem);
	lua_pushtablestringstring(L, "EVENT_VIEWITEMLIST", kEventViewItemList);
	lua_pushtablestringstring(L, "EVENT_VIEWSEARCHRESULTS", kEventViewSearchResults);
	lua_pushtablestringstring(L, "EVENT_ADDSHIPPINGINFO", kEventAddShippingInfo);
	lua_pushtablestringstring(L, "EVENT_PURCHASE", kEventPurchase);
	lua_pushtablestringstring(L, "EVENT_REFUND", kEventRefund);
	lua_pushtablestringstring(L, "EVENT_SELECTITEM", kEventSelectItem);
	lua_pushtablestringstring(L, "EVENT_SELECTPROMOTION", kEventSelectPromotion);
	lua_pushtablestringstring(L, "EVENT_VIEWCART", kEventViewCart);
	lua_pushtablestringstring(L, "EVENT_VIEWPROMOTION", kEventViewPromotion);

	// From firebase/analytics/parameter_names.h
	lua_pushtablestringstring(L, "PARAM_ACHIEVEMENTID", kParameterAchievementID);
	lua_pushtablestringstring(L, "PARAM_ADFORMAT", kParameterAdFormat);
	lua_pushtablestringstring(L, "PARAM_ADNETWORKCLICKID", kParameterAdNetworkClickID);
	lua_pushtablestringstring(L, "PARAM_ADPLATFORM", kParameterAdPlatform);
	lua_pushtablestringstring(L, "PARAM_ADSOURCE", kParameterAdSource);
	lua_pushtablestringstring(L, "PARAM_ADUNITNAME", kParameterAdUnitName);
	lua_pushtablestringstring(L, "PARAM_AFFILIATION", kParameterAffiliation);
	lua_pushtablestringstring(L, "PARAM_CAMPAIGN", kParameterCampaign);
	lua_pushtablestringstring(L, "PARAM_CHARACTER", kParameterCharacter);
	lua_pushtablestringstring(L, "PARAM_CHECKOUTSTEP", kParameterCheckoutStep);
	lua_pushtablestringstring(L, "PARAM_CHECKOUTOPTION", kParameterCheckoutOption);
	lua_pushtablestringstring(L, "PARAM_CONTENT", kParameterContent);
	lua_pushtablestringstring(L, "PARAM_CONTENTTYPE", kParameterContentType);
	lua_pushtablestringstring(L, "PARAM_COUPON", kParameterCoupon);
	lua_pushtablestringstring(L, "PARAM_CP1", kParameterCP1);
	lua_pushtablestringstring(L, "PARAM_CREATIVENAME", kParameterCreativeName);
	lua_pushtablestringstring(L, "PARAM_CREATIVESLOT", kParameterCreativeSlot);
	lua_pushtablestringstring(L, "PARAM_CURRENCY", kParameterCurrency);
	lua_pushtablestringstring(L, "PARAM_DESTINATION", kParameterDestination);
	lua_pushtablestringstring(L, "PARAM_ENDDATE", kParameterEndDate);
	lua_pushtablestringstring(L, "PARAM_FLIGHTNUMBER", kParameterFlightNumber);
	lua_pushtablestringstring(L, "PARAM_GROUPID", kParameterGroupID);
	lua_pushtablestringstring(L, "PARAM_INDEX", kParameterIndex);
	lua_pushtablestringstring(L, "PARAM_ITEMBRAND", kParameterItemBrand);
	lua_pushtablestringstring(L, "PARAM_ITEMCATEGORY", kParameterItemCategory);
	lua_pushtablestringstring(L, "PARAM_ITEMID", kParameterItemID);
	lua_pushtablestringstring(L, "PARAM_ITEMLOCATIONID", kParameterItemLocationID);
	lua_pushtablestringstring(L, "PARAM_ITEMNAME", kParameterItemName);
	lua_pushtablestringstring(L, "PARAM_ITEMLIST", kParameterItemList);
	lua_pushtablestringstring(L, "PARAM_ITEMVARIANT", kParameterItemVariant);
	lua_pushtablestringstring(L, "PARAM_LEVEL", kParameterLevel);
	lua_pushtablestringstring(L, "PARAM_LOCATION", kParameterLocation);
	lua_pushtablestringstring(L, "PARAM_MEDIUM", kParameterMedium);
	lua_pushtablestringstring(L, "PARAM_NUMBEROFNIGHTS", kParameterNumberOfNights);
	lua_pushtablestringstring(L, "PARAM_NUMBEROFPASSENGERS", kParameterNumberOfPassengers);
	lua_pushtablestringstring(L, "PARAM_NUMBEROFROOMS", kParameterNumberOfRooms);
	lua_pushtablestringstring(L, "PARAM_ORIGIN", kParameterOrigin);
	lua_pushtablestringstring(L, "PARAM_PRICE", kParameterPrice);
	lua_pushtablestringstring(L, "PARAM_QUANTITY", kParameterQuantity);
	lua_pushtablestringstring(L, "PARAM_SCORE", kParameterScore);
	lua_pushtablestringstring(L, "PARAM_SCREENCLASS", kParameterScreenClass);
	lua_pushtablestringstring(L, "PARAM_SCREENNAME", kParameterScreenName);
	lua_pushtablestringstring(L, "PARAM_SEARCHTERM", kParameterSearchTerm);
	lua_pushtablestringstring(L, "PARAM_SHIPPING", kParameterShipping);
	lua_pushtablestringstring(L, "PARAM_SIGNUPMETHOD", kParameterSignUpMethod);
	lua_pushtablestringstring(L, "PARAM_METHOD", kParameterMethod);
	lua_pushtablestringstring(L, "PARAM_SOURCE", kParameterSource);
	lua_pushtablestringstring(L, "PARAM_STARTDATE", kParameterStartDate);
	lua_pushtablestringstring(L, "PARAM_TAX", kParameterTax);
	lua_pushtablestringstring(L, "PARAM_TERM", kParameterTerm);
	lua_pushtablestringstring(L, "PARAM_TRANSACTIONID", kParameterTransactionID);
	lua_pushtablestringstring(L, "PARAM_TRAVELCLASS", kParameterTravelClass);
	lua_pushtablestringstring(L, "PARAM_VALUE", kParameterValue);
	lua_pushtablestringstring(L, "PARAM_VIRTUALCURRENCYNAME", kParameterVirtualCurrencyName);
	lua_pushtablestringstring(L, "PARAM_LEVELNAME", kParameterLevelName);
	lua_pushtablestringstring(L, "PARAM_SUCCESS", kParameterSuccess);
	lua_pushtablestringstring(L, "PARAM_EXTENDSESSION", kParameterExtendSession);
	lua_pushtablestringstring(L, "PARAM_DISCOUNT", kParameterDiscount);
	lua_pushtablestringstring(L, "PARAM_ITEMCATEGORY2", kParameterItemCategory2);
	lua_pushtablestringstring(L, "PARAM_ITEMCATEGORY3", kParameterItemCategory3);
	lua_pushtablestringstring(L, "PARAM_ITEMCATEGORY4", kParameterItemCategory4);
	lua_pushtablestringstring(L, "PARAM_ITEMCATEGORY5", kParameterItemCategory5);
	lua_pushtablestringstring(L, "PARAM_ITEMLISTID", kParameterItemListID);
	lua_pushtablestringstring(L, "PARAM_ITEMLISTNAME", kParameterItemListName);
	lua_pushtablestringstring(L, "PARAM_ITEMS", kParameterItems);
	lua_pushtablestringstring(L, "PARAM_LOCATIONID", kParameterLocationID);
	lua_pushtablestringstring(L, "PARAM_PAYMENTTYPE", kParameterPaymentType);
	lua_pushtablestringstring(L, "PARAM_PROMOTIONID", kParameterPromotionID);
	lua_pushtablestringstring(L, "PARAM_PROMOTIONNAME", kParameterPromotionName);
	lua_pushtablestringstring(L, "PARAM_SHIPPINGTIER", kParameterShippingTier);

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
