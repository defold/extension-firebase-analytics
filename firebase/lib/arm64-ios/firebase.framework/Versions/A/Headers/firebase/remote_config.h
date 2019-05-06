// Copyright 2016 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef FIREBASE_REMOTE_CONFIG_CLIENT_CPP_INCLUDE_FIREBASE_REMOTE_CONFIG_H_
#define FIREBASE_REMOTE_CONFIG_CLIENT_CPP_INCLUDE_FIREBASE_REMOTE_CONFIG_H_

#include <cstdint>
#include <string>
#include <vector>

#include "firebase/app.h"
#include "firebase/future.h"
#include "firebase/internal/common.h"
#include "firebase/variant.h"

#if !defined(DOXYGEN) && !defined(SWIG)
FIREBASE_APP_REGISTER_CALLBACKS_REFERENCE(remote_config)
#endif  // !defined(DOXYGEN) && !defined(SWIG)

/// @brief Namespace that encompasses all Firebase APIs.
namespace firebase {

/// @brief Firebase Remote Config API.
///
/// Firebase Remote Config is a cloud service that lets you change the
/// appearance and behavior of your app without requiring users to download an
/// app update.
namespace remote_config {

/// @brief Describes the most recent fetch request status.
enum LastFetchStatus {
  /// The most recent fetch was a success, and its data is ready to be
  /// applied, if you have not already done so.
  kLastFetchStatusSuccess,
  /// The most recent fetch request failed.
  kLastFetchStatusFailure,
  /// The most recent fetch is still in progress.
  kLastFetchStatusPending,
};

/// @brief Describes the most recent fetch failure.
enum FetchFailureReason {
  /// The fetch has not yet failed.
  kFetchFailureReasonInvalid,
  /// The most recent fetch failed because it was throttled by the server.
  /// (You are sending too many fetch requests in too short a time.)
  kFetchFailureReasonThrottled,
  /// The most recent fetch failed for an unknown reason.
  kFetchFailureReasonError,
};

/// @brief Describes the state of the most recent Fetch() call.
/// Normally returned as a result of the GetInfo() function.
struct ConfigInfo {
  /// @brief The time (in milliseconds since the epoch) that the last fetch
  /// operation completed.
  uint64_t fetch_time;

  /// @brief The status of the last fetch request.
  LastFetchStatus last_fetch_status;

  /// @brief The reason the most recent fetch failed.
  FetchFailureReason last_fetch_failure_reason;

  /// @brief The time (in milliseconds since the epoch) when the refreshing of
  /// Remote Config data is throttled.
  uint64_t throttled_end_time;
};

/// @brief Describes the source a config value was retrieved from.
enum ValueSource {
  /// The value was not specified and no default was specified, so a static
  /// value (0 for numeric values, an empty string for strings) was returned.
  kValueSourceStaticValue,
  /// The value was found in the remote data store, and returned.
  kValueSourceRemoteValue,
  /// The value was not specified, so the specified default value was
  /// returned instead.
  kValueSourceDefaultValue,
};

/// @brief Describes a retrieved value.
struct ValueInfo {
  /// Where the config value was retrieved from (Default Config or Active
  /// Config).
  ValueSource source;
  /// If <code>true</code> this indicates conversion to the requested type
  /// succeeded, otherwise conversion failed so the static value for the
  /// requested type was retrieved instead.
  bool conversion_successful;
};

/// @brief Keys of API settings.
///
/// @see SetConfigSetting
/// @see GetConfigSetting
enum ConfigSetting {
  /// Set the value associated with this key to "1" to enable developer mode
  /// (i.e disable throttling) and "0" to disable.
  kConfigSettingDeveloperMode,
};

/// @brief The default cache expiration used by Fetch(), equal to 12 hours,
/// in seconds.
static const uint64_t kDefaultCacheExpiration = 60 * 60 * 12;

/// @brief Describes a mapping of a key to a string value. Used to set default
/// values.
struct ConfigKeyValue {
  /// The lookup key string.
  ///
  /// @note Ensure this string stays valid for the duration of the
  /// call to SetDefaults.
  const char* key;
  /// The value string to be stored.
  ///
  /// @note Ensure this string stays valid for the duration of the
  /// call to SetDefaults.
  const char* value;
};

/// @brief Describes a mapping of a key to a value of any type. Used to set
/// default values.
struct ConfigKeyValueVariant {
  /// The lookup key string.
  ///
  /// @note Ensure this string stays valid for the duration of the
  /// call to SetDefaults.
  const char* key;
  /// The value to be stored. The type of the Variant determines the type of
  /// default data for the given key.
  ///
  /// @note If you use a Variant of type StaticString, ensure it stays
  /// valid for the duration of the call to SetDefaults.
  Variant value;
};

/// @brief Initialize the RemoteConfig API.
///
/// This must be called prior to calling any other methods in the
/// firebase::remote_config namespace.
///
/// @param[in] app Default @ref firebase::App instance.
///            @see firebase::App::GetInstance().
///
/// @return kInitResultSuccess if initialization succeeded, or
/// kInitResultFailedMissingDependency on Android if Google Play services is
/// not available on the current device.
InitResult Initialize(const App& app);

/// @brief Terminate the RemoteConfig API.
///
/// Call this method to free resources associated with the Remote Config API.
void Terminate();

#if defined(__ANDROID__) || defined(DOXYGEN)
/// @brief Sets the default values, using an XML resource.
///
/// @note This method is specific to the Android implementation.
///
/// This completely overwrites all previous default values.
///
/// @param[in] defaults_resource_id Id for the XML resource, which should be in
/// your applications res/xml folder.
void SetDefaults(int defaults_resource_id);

/// @brief Sets the default values, using an XML resource.
///
/// @note This method is specific to the Android implementation.
///
/// This completely overwrites all previous default values.
///
/// @param[in] defaults_resource_id Id for the XML resource, which should be in
/// your applications res/xml folder.
/// @param[in] defaults_namespace Namespace these default values will be
/// applied to.
void SetDefaults(int defaults_resource_id, const char* defaults_namespace);
#endif  // defined(__ANDROID__) || defined(DOXYGEN)

/// @brief Sets the default values based on a mapping of string to Variant.
/// This allows you to specify defaults of type other than string.
///
/// The type of each Variant in the map determines the type of data for which
/// you are providing a default. For example, boolean values can be retrieved
/// with GetBool(), integer values can be retrieved with GetLong(), double
/// values can be retrieved with GetDouble(), string values can be retrieved
/// with GetString(), and binary data can be retrieved with GetData(). Aggregate
/// Variant types are not allowed.
///
/// @see firebase::Variant for more information on how to create a Variant of
/// each type.
///
/// @note This completely overrides all previous values.
///
/// @param defaults Array of ConfigKeyValueVariant, representing the new set of
/// defaults to apply. If the same key is specified multiple times, the
/// value associated with the last duplicate key is applied.
/// @param number_of_defaults Number of elements in the defaults array.
void SetDefaults(const ConfigKeyValueVariant* defaults,
                 size_t number_of_defaults);

/// @brief Sets the default values based on a mapping of string to Variant.
/// This allows you to specify defaults of type other than string.
///
/// The type of each Variant in the map determines the type of data for which
/// you are providing a default. For example, boolean values can be retrieved
/// with GetBool(), integer values can be retrieved with GetLong(), double
/// values can be retrieved with GetDouble(), string values can be retrieved
/// with GetString(), and binary data can be retrieved with GetData(). Aggregate
/// Variant types are not allowed.
///
/// @see firebase::Variant for more information on how to create a Variant of
/// each type.
///
/// @note This completely overrides all previous values.
///
/// @param defaults Array of ConfigKeyValueVariant, representing the new set of
/// defaults to apply. If the same key is specified multiple times, the
/// value associated with the last duplicate key is applied.
/// @param number_of_defaults Number of elements in the defaults array.
/// @param config_namespace Namespace these default values will be applied
/// to.
///
/// @deprecated Support for configuration values in namespaces is deprecated.
/// You can now only assign configuration values to the default namespace.
/// Please use SetDefaults(defaults, number_of_defaults) instead.
FIREBASE_DEPRECATED void SetDefaults(const ConfigKeyValueVariant* defaults,
                                     size_t number_of_defaults,
                                     const char* config_namespace);

/// @brief Sets the default values based on a string map.
///
/// @note This completely overrides all previous values.
///
/// @param defaults Array of ConfigKeyValue, representing the new set of
/// defaults to apply. If the same key is specified multiple times, the
/// value associated with the last duplicate key is applied.
/// @param number_of_defaults Number of elements in the defaults array.
void SetDefaults(const ConfigKeyValue* defaults, size_t number_of_defaults);

/// @brief Sets the default values based on a string map.
///
/// @note This completely overrides all previous values in the specified
/// namespace.
///
/// @param defaults Array of ConfigKeyValue, representing the new set of
/// defaults to apply. If the same key is specified multiple times, the
/// value associated with the last duplicate key is applied.
/// @param number_of_defaults Number of elements in the defaults array.
/// @param config_namespace Namespace these default values will be applied
/// to.
///
/// @deprecated Support for configuration values in namespaces is deprecated.
/// You can now only assign configuration values to the default namespace.
/// Please use SetDefaults(defaults, number_of_defaults) instead.
FIREBASE_DEPRECATED void SetDefaults(const ConfigKeyValue* defaults,
                                     size_t number_of_defaults,
                                     const char* config_namespace);

/// @brief Retrieve an internal configuration setting.
///
/// @param[in] setting Setting to retrieve.
///
/// @return The value of the config
///
/// @see SetConfigSetting
std::string GetConfigSetting(ConfigSetting setting);

/// @brief Sets an internal configuration setting.
///
/// @param[in] setting Setting to set.
/// @param[in] value Value to apply to the setting.
///
/// @see ConfigSetting
void SetConfigSetting(ConfigSetting setting, const char* value);

/// @brief Returns the value associated with a key, converted to a bool.
///
/// Values of "1", "true", "t", "yes", "y" and "on" are interpreted (case
/// insensitive) as <code>true</code> and "0", "false", "f", "no", "n", "off",
/// and empty strings are interpreted (case insensitive) as <code>false</code>.
///
/// @param[in] key Key of the value to be retrieved.
///
/// @returns Value associated with the specified key converted to a boolean
/// value.
bool GetBoolean(const char* key);

/// @brief Returns the value associated with a key, converted to a bool.
///
/// Values of "1", "true", "t", "yes", "y" and "on" are interpreted (case
/// insensitive) as <code>true</code> and "0", "false", "f", "no", "n", "off",
/// and empty strings are interpreted (case insensitive) as <code>false</code>.
///
/// @param[in] key Key of the value to be retrieved.
/// @param[in] config_namespace Namespace to query for the key.
///
/// @returns Value associated with the specified key converted to a boolean
/// value.
///
/// @deprecated Support for configuration values in namespaces is deprecated.
/// You can now only retrieve configuration values from the default namespace.
/// Please use GetBoolean(key) instead.
FIREBASE_DEPRECATED bool GetBoolean(const char* key,
                                    const char* config_namespace);

/// @brief Returns the value associated with a key, converted to a bool.
///
/// Values of "1", "true", "t", "yes", "y" and "on" are interpreted (case
/// insensitive) as <code>true</code> and "0", "false", "f", "no", "n", "off",
/// and empty strings are interpreted (case insensitive) as <code>false</code>.
///
/// @param[in] key Key of the value to be retrieved.
/// @param[out] info A return value, specifying the source of the returned
/// value.
///
/// @returns Value associated with the specified key converted to a boolean
/// value.
bool GetBoolean(const char* key, ValueInfo* info);

/// @brief Returns the value associated with a key, converted to a bool.
///
/// Values of "1", "true", "t", "yes", "y" and "on" are interpreted (case
/// insensitive) as <code>true</code> and "0", "false", "f", "no", "n", "off",
/// and empty strings are interpreted (case insensitive) as <code>false</code>.
///
/// @param[in] key Key of the value to be retrieved.
/// @param[in] config_namespace Namespace to query for the key.
/// @param[out] info A return value, specifying the source of the returned
/// value.
///
/// @returns Value associated with the specified key converted to a boolean
/// value.
///
/// @deprecated Support for configuration values in namespaces is deprecated.
/// You can now only retrieve configuration values from the default namespace.
/// Please use GetBoolean(key, info) instead.
FIREBASE_DEPRECATED bool GetBoolean(const char* key,
                                    const char* config_namespace,
                                    ValueInfo* info);

/// @brief Returns the value associated with a key, converted to a 64-bit
/// integer.
///
/// @param[in] key Key of the value to be retrieved.
///
/// @returns Value associated with the specified key converted to a 64-bit
/// integer.
int64_t GetLong(const char* key);

/// @brief Returns the value associated with a key, converted to a 64-bit
/// integer.
///
/// @param[in] key Key of the value to be retrieved.
/// @param[in] config_namespace Namespace to query for the key.
///
/// @returns Value associated with the specified key converted to a 64-bit
/// integer.
///
/// @deprecated Support for configuration values in namespaces is deprecated.
/// You can now only retrieve configuration values from the default namespace.
/// Please use GetLong(key) instead.
FIREBASE_DEPRECATED int64_t GetLong(const char* key,
                                    const char* config_namespace);

/// @brief Returns the value associated with a key, converted to a 64-bit
/// integer.
///
/// @param[in] key Key of the value to be retrieved.
/// @param[out] info A return value, specifying the source of the returned
/// value.
///
/// @returns Value associated with the specified key converted to a 64-bit
/// integer.
int64_t GetLong(const char* key, ValueInfo* info);

/// @brief Returns the value associated with a key, converted to a 64-bit
/// integer.
///
/// @param[in] key Key of the value to be retrieved.
/// @param[in] config_namespace Namespace to query for the key.
/// @param[out] info A return value, specifying the source of the returned
/// value.
///
/// @returns Value associated with the specified key converted to a 64-bit
/// integer.
///
/// @deprecated Support for configuration values in namespaces is deprecated.
/// You can now only retrieve configuration values from the default namespace.
/// Please use GetLong(key, info) instead.
FIREBASE_DEPRECATED int64_t GetLong(const char* key,
                                    const char* config_namespace,
                                    ValueInfo* info);

/// @brief Returns the value associated with a key, converted to a double.
///
/// @param[in] key Key of the value to be retrieved.
///
/// @returns Value associated with the specified key converted to a double.
double GetDouble(const char* key);

/// @brief Returns the value associated with a key, converted to a double.
///
/// @param[in] key Key of the value to be retrieved.
/// @param[in] config_namespace Namespace to query for the key.
///
/// @returns Value associated with the specified key converted to a double.
///
/// @deprecated Support for configuration values in namespaces is deprecated.
/// You can now only retrieve configuration values from the default namespace.
/// Please use GetDouble(key) instead.
FIREBASE_DEPRECATED double GetDouble(const char* key,
                                     const char* config_namespace);

/// @brief Returns the value associated with a key, converted to a double.
///
/// @param[in] key Key of the value to be retrieved.
/// @param[out] info A return value, specifying the source of the returned
/// value.
///
/// @returns Value associated with the specified key converted to a double.
double GetDouble(const char* key, ValueInfo* info);

/// @brief Returns the value associated with a key, converted to a double.
///
/// @param[in] key Key of the value to be retrieved.
/// @param[in] config_namespace Namespace to query for the key.
/// @param[out] info A return value, specifying the source of the returned
/// value.
///
/// @returns Value associated with the specified key converted to a double.
///
/// @deprecated Support for configuration values in namespaces is deprecated.
/// You can now only retrieve configuration values from the default namespace.
/// Please use GetDouble(key, info) instead.
FIREBASE_DEPRECATED double GetDouble(const char* key,
                                     const char* config_namespace,
                                     ValueInfo* info);

/// @brief Returns the value associated with a key, converted to a string.
///
/// @param[in] key Key of the value to be retrieved.
///
/// @return Value as a string associated with the specified key.
std::string GetString(const char* key);

/// @brief Returns the value associated with a key, converted to a string.
///
/// @param[in] key Key of the value to be retrieved.
/// @param[in] config_namespace Namespace the api lives in
///
/// @return Value as a string associated with the specified key.
///
/// @deprecated Support for configuration values in namespaces is deprecated.
/// You can now only retrieve configuration values from the default namespace.
/// Please use GetString(key) instead.
FIREBASE_DEPRECATED std::string GetString(const char* key,
                                          const char* config_namespace);

/// @brief Returns the value associated with a key, converted to a string.
///
/// @param[in] key Key of the value to be retrieved.
/// @param[out] info A return value, specifying the source of the returned
/// value.
///
/// @return Value as a string associated with the specified key.
std::string GetString(const char* key, ValueInfo* info);

/// @brief Returns the value associated with a key, converted to a string.
///
/// @param[in] key Key of the value to be retrieved.
/// @param[in] config_namespace Namespace to query for the key.
/// @param[out] info A return value, specifying the source of the returned
/// value.
///
/// @deprecated Support for configuration values in namespaces is deprecated.
/// You can now only retrieve configuration values from the default namespace.
/// Please use GetString(key, info) instead.
FIREBASE_DEPRECATED std::string GetString(const char* key,
                                          const char* config_namespace,
                                          ValueInfo* info);

/// @brief Returns the value associated with a key, as a vector of raw
/// byte-data.
///
/// @param[in] key Key of the value to be retrieved.
///
/// @returns Vector of bytes.
std::vector<unsigned char> GetData(const char* key);

/// @brief Returns the value associated with a key, as a vector of raw
/// byte-data.
///
/// @param[in] key Key of the value to be retrieved.
/// @param[in] config_namespace Namespace to query for the key.
///
/// @returns Vector of bytes.
///
/// @deprecated Support for configuration values in namespaces is deprecated.
/// You can now only retrieve configuration values from the default namespace.
/// Please use GetData(key) instead.
FIREBASE_DEPRECATED std::vector<unsigned char> GetData(
    const char* key, const char* config_namespace);

/// @brief Returns the value associated with a key, as a vector of raw
/// byte-data.
///
/// @param[in] key Key of the value to be retrieved.
/// @param[out] info A return value, specifying the source of the returned
/// value.
///
/// @returns Vector of bytes.
std::vector<unsigned char> GetData(const char* key, ValueInfo* info);

/// @brief Returns the value associated with a key, as a vector of raw
/// byte-data.
///
/// @param[in] key Key of the value to be retrieved.
/// @param[in] config_namespace Namespace to query for the key.
/// @param[out] info A return value, specifying the source of the returned
/// value.
///
/// @returns Vector of bytes.
///
/// @deprecated Support for configuration values in namespaces is deprecated.
/// You can now only retrieve configuration values from the default namespace.
/// Please use GetData(key, info) instead.
FIREBASE_DEPRECATED std::vector<unsigned char> GetData(
    const char* key, const char* config_namespace, ValueInfo* info);

/// @brief Gets the set of keys that start with the given prefix, in the
/// default namespace.
///
/// @param[in] prefix The key prefix to look for. If empty or null, this
/// method will return all keys.
///
/// @returns Set of Remote Config parameter keys that start with the specified
/// prefix. Will return an empty set if there are no keys with the given
/// prefix.
std::vector<std::string> GetKeysByPrefix(const char* prefix);

/// @brief Gets the set of keys that start with the given prefix, in the given
/// namespace.
///
/// @param[in] prefix The key prefix to look for. If empty or null, this
/// method will return all keys in the given namespace.
/// @param[in] config_namespace The namespace in which to look up the keys.
///
/// @returns Set of Remote Config parameter keys that start with the specified
/// prefix. Will return an empty set if there are no keys with the given
/// prefix.
///
/// @deprecated Support for configuration values in namespaces is deprecated.
/// You can now only retrieve configuration keys from the default namespace.
/// Please use GetKeysByPrefix(prefix) instead.
FIREBASE_DEPRECATED std::vector<std::string> GetKeysByPrefix(
    const char* prefix, const char* config_namespace);

/// @brief Gets the set of all keys in the default namespace.
///
/// @returns Set of all Remote Config parameter keys in the default namespace.
std::vector<std::string> GetKeys();

/// @brief Gets the set of all keys in the given namespace.
///
/// @param[in] config_namespace The namespace in which to look up the keys.
///
/// @returns Set of all Remote Config parameter keys in the given namespace.
///
/// @deprecated Support for configuration values in namespaces is deprecated.
/// You can now only retrieve configuration keys from the default namespace.
/// Please use GetKeys() instead.
FIREBASE_DEPRECATED std::vector<std::string> GetKeys(
    const char* config_namespace);

/// @brief Fetches config data from the server.
///
/// @note This does not actually apply the data or make it accessible,
/// it merely retrieves it and caches it.  To accept and access the newly
/// retrieved values, you must call @ref ActivateFetched().
/// Note that this function is asynchronous, and will normally take an
/// unspecified amount of time before completion.
///
/// @returns A Future which can be used to determine when the fetch is
/// complete.
Future<void> Fetch();

/// @brief Fetches config data from the server.
///
/// @note This does not actually apply the data or make it accessible,
/// it merely retrieves it and caches it.  To accept and access the newly
/// retrieved values, you must call @ref ActivateFetched().
/// Note that this function is asynchronous, and will normally take an
/// unspecified amount of time before completion.
///
/// @param[in] cache_expiration_in_seconds The number of seconds to keep
/// previously fetch data available.  If cached data is available that is newer
/// than cache_expiration_in_seconds, then the function returns immediately
/// and does not fetch any data. A cache_expiration_in_seconds of zero will
/// always cause a fetch.
///
/// @returns A Future which can be used to determine when the fetch is
/// complete.
Future<void> Fetch(uint64_t cache_expiration_in_seconds);

/// @brief Get the (possibly still pending) results of the most recent Fetch()
/// call.
///
/// @return The future result from the last call to Fetch().
Future<void> FetchLastResult();

/// @brief Applies the most recently fetched data, so that its values can be
/// accessed.
///
/// Calls to @ref GetLong(), @ref GetDouble(), @ref GetString() and
/// @ref GetData() will not reflect the new data retrieved by @ref Fetch()
/// until @ref ActivateFetched() is called.  This gives the developer control
/// over when newly fetched data is visible to their application.
///
/// @returns true if a previously fetch configuration was activated, false
/// if a fetched configuration wasn't found or the configuration was previously
/// activated.
bool ActivateFetched();

/// @brief Returns information about the last fetch request, in the form
/// of a ConfigInfo struct.
///
/// @returns A ConfigInfo struct, containing fields reflecting the state
/// of the most recent fetch request.
const ConfigInfo& GetInfo();

}  // namespace remote_config
}  // namespace firebase

#endif  // FIREBASE_REMOTE_CONFIG_CLIENT_CPP_INCLUDE_FIREBASE_REMOTE_CONFIG_H_
