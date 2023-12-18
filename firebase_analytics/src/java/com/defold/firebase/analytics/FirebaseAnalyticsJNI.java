package com.defold.firebase.analytics;

import android.app.Activity;
import com.google.firebase.analytics.FirebaseAnalytics;
import android.os.Bundle;
import android.util.Log;
import org.json.JSONObject;
import org.json.JSONException;

public class FirebaseAnalyticsJNI {
	private static final String TAG = "FirebaseAnalyticsJNI";
	
	public static native void firebaseAddToQueue(int msg, String json);

  	private FirebaseAnalytics firebaseAnalytics;
	private Activity activity;


	private static final int MSG_ERROR          	= 0;
	private static final int MSG_INITIALIZED		= 1;
	private static final int MSG_DEFAULTS_SET       = 2;
	private static final int MSG_SETTINGS_UPDATED	= 3;
	private static final int MSG_FETCHED        	= 4;
	private static final int MSG_ACTIVATED      	= 5;


	FirebaseAnalyticsJNI(Activity activity) {
		this.activity = activity;
	}

	public void initialize() {
		try {
	  		this.firebaseAnalytics = FirebaseAnalytics.getInstance(activity);
		} catch (Exception e) {
			sendErrorMessage(e.getLocalizedMessage());
		}
	}

	public void setUserId(String id) {
		firebaseAnalytics.setUserId(id);
	}

	public void setUserProperty(String name, String value) {
		firebaseAnalytics.setUserProperty(name, value);
	}

	public void setScreen(String screenName, String screenClassOverride) {
		firebaseAnalytics.setCurrentScreen(activity, screenName, screenClassOverride);
	}

	public void logEventString(String param_name, String param, String event_name) {
		Bundle bundle = new Bundle();
		bundle.putString(param_name, param);
		firebaseAnalytics.logEvent(event_name, bundle);
	}

	public void logEventInt(String param_name, int param, String event_name) {
		Bundle bundle = new Bundle();
		bundle.putInt(param_name, param);
		firebaseAnalytics.logEvent(event_name, bundle);
	}

	public void logEventNumber(String param_name, double param, String event_name) {
		Bundle bundle = new Bundle();
		bundle.putDouble(param_name, param);
		firebaseAnalytics.logEvent(event_name, bundle);
	}

	public void logEventAdRevenue(String unit_id, String adNetwork, double value) {
		Bundle purchaseParams = new Bundle();
		purchaseParams.putString(FirebaseAnalytics.Param.AD_PLATFORM, "appLovin");
		purchaseParams.putString(FirebaseAnalytics.Param.AD_UNIT_NAME, unit_id);
		purchaseParams.putString(FirebaseAnalytics.Param.AD_SOURCE, adNetwork);
		purchaseParams.putString(FirebaseAnalytics.Param.CURRENCY, "USD");
		purchaseParams.putDouble(FirebaseAnalytics.Param.VALUE, value);
		// purchaseParams.putDouble(FirebaseAnalytics.Param.Location, 14.98); TODO
		firebaseAnalytics.logEvent(FirebaseAnalytics.Event.AD_IMPRESSION, purchaseParams);
	}

	public void logEventPurchaseRevenue(String transaction_id, String sku, String currency, double value) {
		Bundle purchaseParams = new Bundle();
		purchaseParams.putString(FirebaseAnalytics.Param.TRANSACTION_ID, transaction_id);
		purchaseParams.putString(FirebaseAnalytics.Param.ITEM_ID, sku);
		purchaseParams.putString(FirebaseAnalytics.Param.CURRENCY, currency);
		purchaseParams.putDouble(FirebaseAnalytics.Param.VALUE, value);
		firebaseAnalytics.logEvent(FirebaseAnalytics.Event.PURCHASE, purchaseParams);
	}

	public void resetAnalyticsData() {
		firebaseAnalytics.resetAnalyticsData();
	}

	public void setAnalyticsCollectionEnabled(boolean enabled) {
		firebaseAnalytics.setAnalyticsCollectionEnabled(enabled);
	}

	private String getJsonConversionErrorMessage(String errorText) {
		String message = null;
		
		try {
			JSONObject obj = new JSONObject();
			obj.put("error", errorText);
			message = obj.toString();
		} catch (JSONException e) {
			message = "{ \"error\": \"Error while converting simple message to JSON.\"}";
		}

		return message;
	}

	private void sendErrorMessage(String errorText) {
		String message = getJsonConversionErrorMessage(errorText);
		Log.d(TAG, "Analytics Error");
		Log.d(TAG, message);
		firebaseAddToQueue(MSG_ERROR, message);
	}
}

// {"set_callback", Lua_SetCallback},

// {"get_id", Lua_GetInstanceId},
// {"log", Lua_Log},
// {"log_table", Lua_LogTable},