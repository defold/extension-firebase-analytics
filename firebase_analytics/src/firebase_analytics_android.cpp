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
        jmethodID      m_AddParamNumber;
        jmethodID      m_AddParamInt;
        jmethodID      m_AddParamString;
        jmethodID      m_SendEvent;
        jmethodID      m_CloseEvent;
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
        g_firebaseAnalytics.m_AddParamNumber = env->GetMethodID(cls, "addParamNumber", "(Ljava/lang/String;D)V");
        g_firebaseAnalytics.m_AddParamInt = env->GetMethodID(cls, "addParamInt", "(Ljava/lang/String;I)V");
        g_firebaseAnalytics.m_AddParamString = env->GetMethodID(cls, "addParamString", "(Ljava/lang/String;Ljava/lang/String;)V");
        g_firebaseAnalytics.m_SendEvent = env->GetMethodID(cls, "sendEvent", "(Ljava/lang/String;)V");
        g_firebaseAnalytics.m_CloseEvent = env->GetMethodID(cls, "closeEvent", "()V");
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

    void OpenEvent()
    {
        if (g_threadAttacher != 0x0)
        {
            delete g_threadAttacher;
            g_threadAttacher = 0x0;
        }
        g_threadAttacher = new dmAndroid::ThreadAttacher();
        JNIEnv* env = g_threadAttacher->GetEnv();
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI, g_firebaseAnalytics.m_OpenEvent);
    }

    void AddParamString(const char* param_name, const char* param)
    {
        JNIEnv* env = g_threadAttacher->GetEnv();
        jstring jparam_name = env->NewStringUTF(param_name);
        jstring jparam = env->NewStringUTF(param);
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_AddParamString, jparam_name, jparam);
        env->DeleteLocalRef(jparam_name);
        env->DeleteLocalRef(jparam);
    }

    void AddParamNumber(const char* param_name, double param)
    {
        JNIEnv* env = g_threadAttacher->GetEnv();
        jstring jparam_name = env->NewStringUTF(param_name);
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_AddParamNumber, jparam_name, param);
        env->DeleteLocalRef(jparam_name);
    }

    void AddParamInt(const char* param_name, int param)
    {
        JNIEnv* env = g_threadAttacher->GetEnv();
        jstring jparam_name = env->NewStringUTF(param_name);
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI , g_firebaseAnalytics.m_AddParamInt, jparam_name, param);
        env->DeleteLocalRef(jparam_name);
    }

    void SendEvent(const char* event_name)
    {
        JNIEnv* env = g_threadAttacher->GetEnv();
        jstring jevent_name = env->NewStringUTF(event_name);
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI, g_firebaseAnalytics.m_SendEvent, jevent_name);
        env->DeleteLocalRef(jevent_name); 
    }

    void CloseEvent()
    {
        JNIEnv* env = g_threadAttacher->GetEnv();
        env->CallVoidMethod(g_firebaseAnalytics.m_JNI, g_firebaseAnalytics.m_CloseEvent);
        delete g_threadAttacher;
        g_threadAttacher = 0x0;
    }

} //namespace dmFirebaseAnalytics

#endif // DM_PLATFORM_ANDROID
