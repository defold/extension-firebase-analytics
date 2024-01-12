#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_IOS)
#include <FirebaseAnalytics/FirebaseAnalytics.h>

#include "firebase_analytics_callback.h"

namespace dmFirebaseAnalytics {

void SendSimpleMessage(Message msg, id obj) {
    NSError* error;
    NSData* jsonData = [NSJSONSerialization dataWithJSONObject:obj options:(NSJSONWritingOptions)0 error:&error];

    if (jsonData)
    {
        NSString* nsstring = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
        AddToQueueCallback(msg, (const char*)[nsstring UTF8String]);
        [nsstring release];
    }
    else
    {
        NSMutableDictionary *dict = [NSMutableDictionary dictionary];
        [dict setObject:error.localizedDescription forKey:@"error"];
        NSError* error2;
        NSData* errorJsonData = [NSJSONSerialization dataWithJSONObject:dict options:(NSJSONWritingOptions)0 error:&error2];
        if (errorJsonData)
        {
            NSString* nsstringError = [[NSString alloc] initWithData:errorJsonData encoding:NSUTF8StringEncoding];
            AddToQueueCallback(MSG_ERROR, (const char*)[nsstringError UTF8String]);
            [nsstringError release];
        }
        else
        {
            AddToQueueCallback(MSG_ERROR, [[NSString stringWithFormat:@"{ \"error\": \"Error while converting simple message to JSON.\"}"] UTF8String]);
        }
    }
}

void SendSimpleMessage(Message msg, NSString *key, NSString *value) {
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    [dict setObject:value forKey:key];
    SendSimpleMessage(msg, dict);
}

void Initialize_Ext() {}
void Initialize() {}

void GetInstanceId() {
    SendSimpleMessage(MSG_INSTANCE_ID, @"instance_id", [FIRAnalytics appInstanceID]);
}

void SetUserId(const char* id) {
    [FIRAnalytics setUserID:[NSString stringWithUTF8String:id]];
}

void SetUserProperty(const char* name, const char* value) {
    [FIRAnalytics setUserPropertyString:[NSString stringWithUTF8String:value] forName:[NSString stringWithUTF8String:name]];
}

void LogEvent(const char* event_name) {
    [FIRAnalytics logEventWithName:[NSString stringWithUTF8String:event_name] parameters:@{}];
}

void LogEventString(const char* param_name, const char* param, const char* event_name) {
    [FIRAnalytics logEventWithName:[NSString stringWithUTF8String:event_name]
                    parameters:@{
                                [NSString stringWithUTF8String:event_name]: [NSString stringWithUTF8String:param]
                             }];
}

void LogEventInt(const char* param_name, int param, const char* event_name) {
    [FIRAnalytics logEventWithName:[NSString stringWithUTF8String:event_name]
                    parameters:@{
                                [NSString stringWithUTF8String:event_name]: @(param)
                             }];
}

void LogEventNumber(const char* param_name, double param, const char* event_name) {
        [FIRAnalytics logEventWithName:[NSString stringWithUTF8String:event_name]
                    parameters:@{
                                [NSString stringWithUTF8String:event_name]: @(param)
                             }];
}

void ResetAnalyticsData() {
    [FIRAnalytics resetAnalyticsData];
}

void SetAnalyticsCollectionEnabled(bool enabled) {
    [FIRAnalytics setAnalyticsCollectionEnabled:enabled? YES : NO];
}

} //namespace dmFirebaseAnalytics

#endif
