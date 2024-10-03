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
        jmethodID      m_Reset;
        jmethodID      m_SetEnabled;
        jmethodID      m_SetUserProperty;
        jmethodID      m_SetUserID;

        jmethodID      m_OpenEvent;
        jmethodID      m_AddEventParamNumber;
        jmethodID      m_AddEventParamInt;
        jmethodID      m_AddEventParamString;
        jmethodID      m_SendEvent;
        jmethodID      m_CloseEvent;

        jmethodID      m_OpenDefaultEventParams;
        jmethodID      m_AddDefaultEventParamNumber;
        jmethodID      m_AddDefaultEventParamInt;
        jmethodID      m_AddDefaultEventParamString;
        jmethodID      m_SetDefaultEventParams;
        jmethodID      m_CloseDefaultEventParams;
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

        g_firebaseAnalytics.m_OpenEvent = env->GetMethodID(cls, "openEvent", "()V");
        g_firebaseAnalytics.m_AddEventParamNumber = env->GetMethodID(cls, "addEventParamNumber", "(Ljava/lang/String;D)V");
        g_firebaseAnalytics.m_AddEventParamInt = env->GetMethodID(cls, "addEventParamInt", "(Ljava/lang/String;I)V");
        g_firebaseAnalytics.m_AddEventParamString = env->GetMethodID(cls, "addEventParamString", "(Ljava/lang/String;Ljava/lang/String;)V");
        g_firebaseAnalytics.m_SendEvent = env->GetMethodID(cls, "sendEvent", "(Ljava/lang/String;)V");
        g_firebaseAnalytics.m_CloseEvent = env->GetMethodID(cls, "closeEvent", "()V");

        g_firebaseAnalytics.m_OpenDefaultEventParams = env->GetMethodID(cls, "openDefaultEventParams", "()V");
        g_firebaseAnalytics.m_AddDefaultEventParamNumber = env->GetMethodID(cls, "addDefaultEventParamNumber", "(Ljava/lang/String;D)V");
        g_firebaseAnalytics.m_AddDefaultEventParamInt = env->GetMethodID(cls, "addDefaultEventParamInt", "(Ljava/lang/String;I)V");
        g_firebaseAnalytics.m_AddDefaultEventParamString = env->GetMethodID(cls, "addDefaultEventParamString", "(Ljava/lang/String;Ljava/lang/String;)V");
        g_firebaseAnalytics.m_SetDefaultEventParams = env->GetMethodID(cls, "setDefaultEventParams", "()V");
        g_firebaseAnalytics.m_CloseDefaultEventParams = env->GetMethodID(cls, "closeDefaultEventParams", "()V");
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

    //---
    dmAndroid::ThreadAttacher* g_threadAttacher = 0x0;

    void SetupThreadAttacher()
    {
        if (g_threadAttacher != 0x0)
        {
            delete g_threadAttacher;
            g_threadAttacher = 0x0;
        }
        g_threadAttacher = new dmAndroid::ThreadAttacher();

    }

    void OpenEvent()
    {
        SetupThreadAttacher();
        JNIEnv* env = g_threadAttacher->GetEnv();
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI, g_firebaseAnalytics.m_OpenEvent);
    }
    void OpenDefaultEventParams()
    {
        SetupThreadAttacher();
        JNIEnv* env = g_threadAttacher->GetEnv();
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI, g_firebaseAnalytics.m_OpenDefaultEventParams);
    }

    void CallStringString(jmethodID method, const char* str1, const char* str2)
    {
        JNIEnv* env = g_threadAttacher->GetEnv();
        jstring jstr1 = env->NewStringUTF(str1);
        jstring jstr2 = env->NewStringUTF(str2);
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI , method, jstr1, jstr2);
        env->DeleteLocalRef(jstr1);
        env->DeleteLocalRef(jstr2);
    }
    void CallStringDouble(jmethodID method, const char* str, double d)
    {
        JNIEnv* env = g_threadAttacher->GetEnv();
        jstring jstr = env->NewStringUTF(str);
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI , method, jstr, d);
        env->DeleteLocalRef(jstr);
    }
    void CallStringInt(jmethodID method, const char* str, int i)
    {
        JNIEnv* env = g_threadAttacher->GetEnv();
        jstring jstr = env->NewStringUTF(str);
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI, method, jstr, i);
        env->DeleteLocalRef(jstr);
    }

    void AddEventParamString(const char* param_name, const char* param)
    {
        CallStringString(g_firebaseAnalytics.m_AddEventParamString, param_name, param);
    }
    void AddDefaultEventParamString(const char* param_name, const char* param)
    {
        CallStringString(g_firebaseAnalytics.m_AddDefaultEventParamString, param_name, param);
    }

    void AddEventParamNumber(const char* param_name, double param)
    {
        CallStringDouble(g_firebaseAnalytics.m_AddEventParamNumber, param_name, param);
    }
    void AddDefaultEventParamNumber(const char* param_name, double param)
    {
        CallStringDouble(g_firebaseAnalytics.m_AddDefaultEventParamNumber, param_name, param);
    }

    void AddEventParamInt(const char* param_name, int param)
    {
        CallStringInt(g_firebaseAnalytics.m_AddEventParamInt, param_name, param);
    }
    void AddDefaultEventParamInt(const char* param_name, int param)
    {
        CallStringInt(g_firebaseAnalytics.m_AddDefaultEventParamInt, param_name, param);
    }

    void SendEvent(const char* event_name)
    {
        JNIEnv* env = g_threadAttacher->GetEnv();
        jstring jevent_name = env->NewStringUTF(event_name);
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI, g_firebaseAnalytics.m_SendEvent, jevent_name);
        env->DeleteLocalRef(jevent_name); 
    }
    void SetDefaultEventParams()
    {
        JNIEnv* env = g_threadAttacher->GetEnv();
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI, g_firebaseAnalytics.m_SetDefaultEventParams);
    }

    void CloseEvent()
    {
        JNIEnv* env = g_threadAttacher->GetEnv();
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI, g_firebaseAnalytics.m_CloseEvent);
        delete g_threadAttacher;
        g_threadAttacher = 0x0;
    }
    void CloseDefaultEventParams()
    {
        JNIEnv* env = g_threadAttacher->GetEnv();
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI, g_firebaseAnalytics.m_CloseDefaultEventParams);
        delete g_threadAttacher;
        g_threadAttacher = 0x0;
    }

} //namespace dmFirebaseAnalytics

#endif // DM_PLATFORM_ANDROID
