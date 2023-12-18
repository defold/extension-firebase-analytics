#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/dlib/android.h>
#include "com_defold_firebase_analytics_FirebaseAnalyticsJNI.h"
#include "firebase_analytics_callback.h"
#include "firebase_analytics_private.h"

JNIEXPORT void JNICALL Java_com_defold_firebase_analytics_FirebaseAnalyticsJNI_firebaseAddToQueue(JNIEnv * env, jclass cls, jint jmsg, jstring jjson)
{
    const char* json = env->GetStringUTFChars(jjson, 0);
    dmFirebaseAnalytics::AddToQueueCallback((dmFirebaseAnalytics::Message)jmsg, json);
    env->ReleaseStringUTFChars(jjson, json);
}

namespace dmFirebaseAnalytics {
    struct FirebaseAnalyticsJNI {
        jobject        m_JNI;

        jmethodID m_Initialize;
        jmethodID      m_GetID;
        jmethodID      m_Log;
        jmethodID      m_LogString;
        jmethodID      m_LogInt;
        jmethodID      m_LogNumber;
        jmethodID      m_Reset;
        jmethodID      m_SetEnabled;
        jmethodID      m_SetScreen;
        jmethodID      m_SetUserProperty;
        jmethodID      m_SetUserID;
        jmethodID      m_LogEventAdRevenue;
        jmethodID      m_LogEventPurchaseRevenue;
    };

    static FirebaseAnalyticsJNI g_firebaseAnalytics;

    static void CallVoidMethod(jobject instance, jmethodID method)
    {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();
        env->CallVoidMethod(instance, method);
    }

    static void InitJNIMethods(JNIEnv* env, jclass cls)
    {
        g_firebaseAnalytics.m_Initialize = env->GetMethodID(cls, "initialize", "()V");
        assert(g_firebaseAnalytics.m_Initialize != NULL && "initialize NULL");

        g_firebaseAnalytics.m_SetUserID = env->GetMethodID(cls, "setUserId", "(Ljava/lang/String;)V");
        assert(g_firebaseAnalytics.m_SetUserID != NULL && "setUserId NULL");

        g_firebaseAnalytics.m_SetEnabled = env->GetMethodID(cls, "setAnalyticsCollectionEnabled", "(Z)V");
        assert(g_firebaseAnalytics.m_SetEnabled != NULL && "setAnalyticsCollectionEnabled NULL");

        g_firebaseAnalytics.m_SetUserProperty = env->GetMethodID(cls, "setUserProperty", "(Ljava/lang/String;Ljava/lang/String;)V");
        assert(g_firebaseAnalytics.m_SetUserProperty != NULL && "setUserProperty NULL");

        g_firebaseAnalytics.m_SetScreen = env->GetMethodID(cls, "setScreen","(Ljava/lang/String;Ljava/lang/String;)V");
        assert(g_firebaseAnalytics.m_SetScreen != NULL && "setScreen NULL");

        g_firebaseAnalytics.m_Reset = env->GetMethodID(cls, "resetAnalyticsData", "()V");
        assert(g_firebaseAnalytics.m_Reset != NULL && "resetAnalyticsData NULL");

        // g_firebaseAnalytics.m_Log = env->GetMethodID(cls, "m_Log", "(Ljava/lang/String;)V");
        g_firebaseAnalytics.m_LogNumber = env->GetMethodID(cls, "logEventNumber", "(Ljava/lang/String;DLjava/lang/String;)V");
        assert(g_firebaseAnalytics.m_LogNumber != NULL && "logEventNumber NULL");

        g_firebaseAnalytics.m_LogInt = env->GetMethodID(cls, "logEventInt", "(Ljava/lang/String;ILjava/lang/String;)V");
        assert(g_firebaseAnalytics.m_LogInt != NULL && "logEventInt NULL");

        g_firebaseAnalytics.m_LogString = env->GetMethodID(cls, "logEventString", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
        assert(g_firebaseAnalytics.m_LogString != NULL && "logEventString NULL");

        g_firebaseAnalytics.m_LogEventAdRevenue = env->GetMethodID(cls, "logEventAdRevenue", "(Ljava/lang/String;Ljava/lang/String;D)V");
        assert(g_firebaseAnalytics.m_LogEventAdRevenue != NULL && "logEventAdRevenue NULL");

        g_firebaseAnalytics.m_LogEventPurchaseRevenue = env->GetMethodID(cls, "logEventPurchaseRevenue", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;D)V");
        assert(g_firebaseAnalytics.m_LogEventPurchaseRevenue != NULL && "logEventPurchaseRevenue NULL");

        // g_firebaseAnalytics.m_GetID = env->GetMethodID(cls, "logEventString", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    }

    void Initialize_Ext() 
    {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();
        jclass cls = dmAndroid::LoadClass(env, "com.defold.firebase.analytics.FirebaseAnalyticsJNI");

        InitJNIMethods(env, cls);

        jmethodID jni_constructor = env->GetMethodID(cls, "<init>", "(Landroid/app/Activity;)V");
        g_firebaseAnalytics.m_JNI = env->NewGlobalRef(env->NewObject(cls, jni_constructor, threadAttacher.GetActivity()->clazz));
    }

    void GetInstanceId() {

    }

    void Initialize() {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_Initialize);
    }
    
    void SetUserId(const char* id) {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();
        jstring user_id = env->NewStringUTF(id);
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_SetUserID, user_id);
        env->DeleteLocalRef(user_id);
    }

    void SetUserProperty(const char* name, const char* value) {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();
        jstring jname = env->NewStringUTF(name);
        jstring jvalue = env->NewStringUTF(value);
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_SetUserProperty, jname, jvalue);
        env->DeleteLocalRef(jname);
        env->DeleteLocalRef(jvalue);
    }

    void SetScreen(const char* screenName, const char* screenClassOverride) {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();
        jstring jscreenName = env->NewStringUTF(screenName);
        jstring jscreenClassOverride = env->NewStringUTF(screenClassOverride);
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_SetScreen, jscreenName, jscreenClassOverride);
        env->DeleteLocalRef(jscreenName);
        env->DeleteLocalRef(jscreenClassOverride);
    }
    void LogEventString(const char* param_name, const char* param, const char* event_name) {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();
        jstring jparam_name = env->NewStringUTF(param_name);
        jstring jparam = env->NewStringUTF(param);
        jstring jevent_name = env->NewStringUTF(event_name);
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_LogString, jparam_name, jparam, jevent_name);
        env->DeleteLocalRef(jparam_name);
        env->DeleteLocalRef(jparam);
        env->DeleteLocalRef(jevent_name);
    }
    void LogEventInt(const char* param_name, int param, const char* event_name) {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();
        jstring jparam_name = env->NewStringUTF(param_name);
        jstring jevent_name = env->NewStringUTF(event_name);
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_LogInt, jparam_name, param, jevent_name);
        env->DeleteLocalRef(jparam_name);
        env->DeleteLocalRef(jevent_name);
    }

    void LogEventNumber(const char* param_name, double param, const char* event_name) {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();
        jstring jparam_name = env->NewStringUTF(param_name);
        jstring jevent_name = env->NewStringUTF(event_name);
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_LogNumber, jparam_name, param, jevent_name);
        env->DeleteLocalRef(jparam_name);
        env->DeleteLocalRef(jevent_name);
    }
    void LogEventAdRevenue(const char* unit_id, const char* adNetwork, double value) {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();
        jstring junit_id = env->NewStringUTF(unit_id);
        jstring jadNetwork = env->NewStringUTF(adNetwork);
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_LogEventAdRevenue, junit_id, jadNetwork, value);
        env->DeleteLocalRef(junit_id);
        env->DeleteLocalRef(jadNetwork);
    }

    void LogEventPurchaseRevenue(const char* transaction_id, const char* sku, const char* currency, double value) {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();
        jstring jtransaction_id = env->NewStringUTF(transaction_id);
        jstring jsku = env->NewStringUTF(sku);
        jstring jcurrency = env->NewStringUTF(currency);
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_LogEventPurchaseRevenue, jtransaction_id, jsku, jcurrency, value);
        env->DeleteLocalRef(jtransaction_id);
        env->DeleteLocalRef(jsku);
        env->DeleteLocalRef(jcurrency);
    }

    void ResetAnalyticsData() {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_Reset);
    }

    void SetAnalyticsCollectionEnabled(bool enabled) {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_SetEnabled, enabled);
    }
} //namespace dmFirebaseAnalytics



#endif // DM_PLATFORM_ANDROID