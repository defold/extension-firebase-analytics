#if defined(DM_PLATFORM_ANDROID) || defined(DM_PLATFORM_IOS)

#pragma once

namespace dmFirebaseAnalytics {
    void Initialize_Ext();
    void Initialize();
    void GetInstanceId();
    void SetUserId(const char* id);
    void SetUserProperty(const char* name, const char* value);
    void ResetAnalyticsData();
    void SetAnalyticsCollectionEnabled(bool enabled);

    void OpenEvent();
    void AddEventParamString(const char* param_name, const char* param);
    void AddEventParamNumber(const char* param_name, double param);
    void AddEventParamInt(const char* param_name, int param);
    void SendEvent(const char* event_name);
    void CloseEvent();

    void OpenDefaultEventParams();
    void AddDefaultEventParamString(const char* param_name, const char* param);
    void AddDefaultEventParamNumber(const char* param_name, double param);
    void AddDefaultEventParamInt(const char* param_name, int param);
    void SetDefaultEventParams();
    void CloseDefaultEventParams();

} //namespace dmFirebaseAnalytics

#endif
