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

void ResetAnalyticsData() {
    [FIRAnalytics resetAnalyticsData];
}

void SetAnalyticsCollectionEnabled(bool enabled) {
    [FIRAnalytics setAnalyticsCollectionEnabled:enabled? YES : NO];
}

// ---

NSMutableDictionary *g_EventParams;
NSMutableDictionary *g_DefaultEventParams;

static void ReleaseDictionary(NSMutableDictionary *dict) {
    for (NSString *key in dict)
    {
        [[dict objectForKey:key] release];
        [key release];
    }
    [dict release];
}

static void ReleaseEventParams() {
    ReleaseDictionary(g_EventParams);
    g_EventParams = nil;
}
static void ReleaseDefaultEventParams() {
    ReleaseDictionary(g_DefaultEventParams);
    g_DefaultEventParams = nil;
}

void OpenEvent() {
    if (g_EventParams != nil)
    {
        ReleaseEventParams();
    }
    g_EventParams = [[NSMutableDictionary alloc] init];
}
void OpenDefaultEventParams() {
    if (g_DefaultEventParams != nil)
    {
        ReleaseDefaultEventParams();
    }
    g_DefaultEventParams = [[NSMutableDictionary alloc] init];
}

void AddParamStringToDict(NSMutableDictionary *dict, const char* param_name, const char* param) {
    NSString *key = [[NSString alloc] initWithUTF8String:param_name];
    NSString *value = [[NSString alloc] initWithUTF8String:param];
    [dict setObject:value forKey:key];
}
void AddParamNumberToDict(NSMutableDictionary *dict, const char* param_name, double param) {
    NSString *key = [[NSString alloc] initWithUTF8String:param_name];
    NSNumber *value = [[NSNumber alloc] initWithDouble:param];
    [dict setObject:value forKey:key];
}
void AddParamIntToDict(NSMutableDictionary *dict, const char* param_name, int param) {
    NSString *key = [[NSString alloc] initWithUTF8String:param_name];
    NSNumber *value = [[NSNumber alloc] initWithInt:param];
    [dict setObject:value forKey:key];
}

void AddEventParamString(const char* param_name, const char* param) {
    AddParamStringToDict(g_EventParams, param_name, param);
}
void AddDefaultEventParamString(const char* param_name, const char* param) {
    AddParamStringToDict(g_DefaultEventParams, param_name, param);
}
void AddEventParamNumber(const char* param_name, double param) {
    AddParamNumberToDict(g_EventParams, param_name, param);
}
void AddDefaultEventParamNumber(const char* param_name, double param) {
    AddParamNumberToDict(g_DefaultEventParams, param_name, param);
}
void AddEventParamInt(const char* param_name, int param) {
    AddParamIntToDict(g_EventParams, param_name, param);
}
void AddDefaultEventParamInt(const char* param_name, int param) {
    AddParamIntToDict(g_DefaultEventParams, param_name, param);
}

void SendEvent(const char* event_name) {
    NSString *name = [[NSString alloc] initWithUTF8String:event_name];
    [FIRAnalytics logEventWithName:name parameters:g_EventParams];
    [name release];
}
void SetDefaultEventParameters() {
    [FIRAnalytics setDefaultEventParameters:g_DefaultEventParams];
}

void CloseEvent() {
    ReleaseEventParams();
}
void CloseDefaultEventParams() {
    ReleaseDefaultEventParams();
}

// ---

} //namespace dmFirebaseAnalytics

#endif
