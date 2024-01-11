#if defined(DM_PLATFORM_ANDROID) || defined(DM_PLATFORM_IOS)

#pragma once

namespace dmFirebaseAnalytics {
    void Initialize_Ext();
    void Initialize();
    void GetInstanceId();
    void SetUserId(const char* id);
    void SetUserProperty(const char* name, const char* value);
    void LogEvent(const char* event_name);
    void LogEventString(const char* param_name, const char* param, const char* event_name);
    void LogEventInt(const char* param_name, int param, const char* event_name);
    void LogEventNumber(const char* param_name, double param, const char* event_name);
    void ResetAnalyticsData();
    void SetAnalyticsCollectionEnabled(bool enabled);
} //namespace dmFirebaseAnalytics

#endif
