package com.defold.firebase.analytics;

import android.app.Activity;

import com.google.firebase.analytics.FirebaseAnalytics;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;

import android.os.Bundle;
import android.util.Log;

import org.json.JSONObject;
import org.json.JSONException;

public class FirebaseAnalyticsJNI {
    private static final String TAG = "FirebaseAnalyticsJNI";
    
    public static native void firebaseAddToQueue(int msg, String json);

    private FirebaseAnalytics firebaseAnalytics;
    private Activity activity;

    private static final int MSG_ERROR              = 0;
    private static final int MSG_INSTANCE_ID        = 1;

    FirebaseAnalyticsJNI(Activity activity) {
        this.activity = activity;
    }

    public void initialize() {
        this.firebaseAnalytics = FirebaseAnalytics.getInstance(activity);
    }

    public void getInstanceId() {
        this.firebaseAnalytics.getAppInstanceId().addOnCompleteListener(new OnCompleteListener<String>() {
            @Override
            public void onComplete(Task<String> task) {
                    if (task.isSuccessful()) {
                        String appInstanceId = task.getResult();
                        sendSimpleMessage(MSG_INSTANCE_ID, "instance_id", appInstanceId);
                    } else {
                        Exception e = task.getException();
                        String errorMessage = e.getMessage();
                        sendErrorMessage("Firebase Analytics can't recieve instance id: " + errorMessage);
                    }
                }
            });
    }

    public void setUserId(String id) {
        firebaseAnalytics.setUserId(id);
    }

    public void setUserProperty(String name, String value) {
        firebaseAnalytics.setUserProperty(name, value);
    }

    public void logEvent(String event_name) {
        firebaseAnalytics.logEvent(event_name, null);
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

    public void resetAnalyticsData() {
        firebaseAnalytics.resetAnalyticsData();
    }

    public void setAnalyticsCollectionEnabled(boolean enabled) {
        firebaseAnalytics.setAnalyticsCollectionEnabled(enabled);
    }

    //---
    
    private Bundle g_EventParams;

    public void openEvent() {
        g_EventParams = new Bundle();
    }

    public void addEventParamNumber(String param_name, double param) {
        g_EventParams.putDouble(param_name, param);
    }

    public void addEventParamInt(String param_name, int param) {
        g_EventParams.putInt(param_name, param);
    }

    public void addEventParamString(String param_name, String param) {
        g_EventParams.putString(param_name, param);
    }

    public void sendEvent(String event_name) {
        firebaseAnalytics.logEvent(event_name, g_EventParams);
    }

    public void closeEvent() {
        g_EventParams = null;
    }

    //---
    
    private Bundle g_DefaultEventParams;

    public void openDefaultEventParams() {
        g_DefaultEventParams = new Bundle();
    }

    public void addDefaultEventParamNumber(String param_name, double param) {
        g_DefaultEventParams.putDouble(param_name, param);
    }

    public void addDefaultEventParamInt(String param_name, int param) {
        g_DefaultEventParams.putInt(param_name, param);
    }

    public void addDefaultEventParamString(String param_name, String param) {
        g_DefaultEventParams.putString(param_name, param);
    }

    public void setDefaultEventParams() {
        firebaseAnalytics.setDefaultEventParameters(g_DefaultEventParams);
    }

    public void closeDefaultEventParams() {
        g_DefaultEventParams = null;
    }

    //---

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

    private void sendSimpleMessage(int msg, String key_1, String value_1) {
        String message = null;
        try {
            JSONObject obj = new JSONObject();
            obj.put(key_1, value_1);
            message = obj.toString();
        } catch (JSONException e) {
            message = getJsonConversionErrorMessage(e.getLocalizedMessage());
        }
        firebaseAddToQueue(msg, message);
    }

    private void sendSimpleMessage(int msg) {
        firebaseAddToQueue(msg, "{}");
    }  
}
