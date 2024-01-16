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

    void OpenEvent();
    void AddParamString(const char* param_name, const char* param);
    void AddParamBoolean(const char* param_name, bool param);
    void AddParamNumber(const char* param_name, double param);
    void AddParamInt(const char* param_name, int param);
    void SendEvent(const char* event_name);
    void CloseEvent();

} //namespace dmFirebaseAnalytics

#endif
