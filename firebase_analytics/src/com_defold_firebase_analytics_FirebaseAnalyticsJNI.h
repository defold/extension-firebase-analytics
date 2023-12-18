#if defined(DM_PLATFORM_ANDROID)

#include <jni.h>
/* Header for class com_defold_firebase_analytics_FirebaseAnalyticsJNI */

#ifndef COM_DEFOLD_FIREBASE_ANALYTICS_FIREBASEANALYTICSJNI_H
#define COM_DEFOLD_FIREBASE_ANALYTICS_FIREBASEANALYTICSJNI_H
#ifdef __cplusplus
extern "C" {
#endif
	/*
	* Class:     com_defold_firebase_analytics_FirebaseAnalyticsJNI
	* Method:    firebaseAddToQueue_first_arg
	* Signature: (ILjava/lang/String;I)V
	*/
						   
	JNIEXPORT void JNICALL Java_com_defold_firebase_analytics_FirebaseAnalyticsJNI_firebaseAddToQueue
		(JNIEnv *, jclass, jint, jstring);

#ifdef __cplusplus
}
#endif
#endif

#endif
