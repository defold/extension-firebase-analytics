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

    struct FirebaseAnalyticsJNI
    {
        jobject        m_JNI;

        jmethodID      m_Initialize;
        jmethodID      m_GetInstanceId;
        jmethodID      m_GetID;
        jmethodID      m_Log;
        jmethodID      m_LogString;
        jmethodID      m_LogInt;
        jmethodID      m_LogNumber;
        jmethodID      m_Reset;
        jmethodID      m_SetEnabled;
        jmethodID      m_SetUserProperty;
        jmethodID      m_SetUserID;
    };

    static FirebaseAnalyticsJNI g_firebaseAnalytics;

    static void CallVoidMethod(jobject instance, jmethodID method)
    {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();
        env->CallVoidMethod(instance, method);
    }

    static void CallVoidMethodChar(jobject instance, jmethodID method, const char* cstr)
    {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();

        jstring jstr = env->NewStringUTF(cstr);
        env->CallVoidMethod(instance, method, jstr);
        env->DeleteLocalRef(jstr);
    }

    static void CallVoidMethodCharChar(jobject instance, jmethodID method, const char* cstr, const char* cstr1)
    {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();

        jstring jstr = env->NewStringUTF(cstr);
        jstring jstr1 = env->NewStringUTF(cstr1);
        env->CallVoidMethod(instance, method, jstr, jstr1);
        env->DeleteLocalRef(jstr);
        env->DeleteLocalRef(jstr1);
    }

    static void InitJNIMethods(JNIEnv* env, jclass cls)
    {
        g_firebaseAnalytics.m_Initialize = env->GetMethodID(cls, "initialize", "()V");
        g_firebaseAnalytics.m_GetInstanceId = env->GetMethodID(cls, "getInstanceId", "()V");
        g_firebaseAnalytics.m_SetUserID = env->GetMethodID(cls, "setUserId", "(Ljava/lang/String;)V");
        g_firebaseAnalytics.m_SetEnabled = env->GetMethodID(cls, "setAnalyticsCollectionEnabled", "(Z)V");
        g_firebaseAnalytics.m_SetUserProperty = env->GetMethodID(cls, "setUserProperty", "(Ljava/lang/String;Ljava/lang/String;)V");
        g_firebaseAnalytics.m_Reset = env->GetMethodID(cls, "resetAnalyticsData", "()V");
        g_firebaseAnalytics.m_Log = env->GetMethodID(cls, "logEvent", "(Ljava/lang/String;)V");
        g_firebaseAnalytics.m_LogNumber = env->GetMethodID(cls, "logEventNumber", "(Ljava/lang/String;DLjava/lang/String;)V");
        g_firebaseAnalytics.m_LogInt = env->GetMethodID(cls, "logEventInt", "(Ljava/lang/String;ILjava/lang/String;)V");
        g_firebaseAnalytics.m_LogString = env->GetMethodID(cls, "logEventString", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
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

    void Initialize()
    {
        CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_Initialize);
    }

    void GetInstanceId()
    {
        CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_GetInstanceId);
    }
    
    void SetUserId(const char* id)
    {
        CallVoidMethodChar(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_SetUserID, id);
    }

    void SetUserProperty(const char* name, const char* value)
    {
        CallVoidMethodCharChar(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_SetUserProperty, name, value);
    }

    void LogEvent(const char* event_name)
    {
        CallVoidMethodChar(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_Log, event_name);
    }

    void LogEventString(const char* param_name, const char* param, const char* event_name)
    {
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

    void LogEventInt(const char* param_name, int param, const char* event_name)
    {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();
        jstring jparam_name = env->NewStringUTF(param_name);
        jstring jevent_name = env->NewStringUTF(event_name);
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_LogInt, jparam_name, param, jevent_name);
        env->DeleteLocalRef(jparam_name);
        env->DeleteLocalRef(jevent_name);
    }

    void LogEventNumber(const char* param_name, double param, const char* event_name)
    {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();
        jstring jparam_name = env->NewStringUTF(param_name);
        jstring jevent_name = env->NewStringUTF(event_name);
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_LogNumber, jparam_name, param, jevent_name);
        env->DeleteLocalRef(jparam_name);
        env->DeleteLocalRef(jevent_name);
    }

    void ResetAnalyticsData()
    {
        CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_Reset);
    }

    void SetAnalyticsCollectionEnabled(bool enabled)
    {
        dmAndroid::ThreadAttacher threadAttacher;
        JNIEnv* env = threadAttacher.GetEnv();
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_SetEnabled, enabled);
    }

} //namespace dmFirebaseAnalytics

#endif // DM_PLATFORM_ANDROID
