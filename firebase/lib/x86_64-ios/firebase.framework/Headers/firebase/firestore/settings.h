/*
 * Copyright 2018 Google
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FIREBASE_FIRESTORE_CLIENT_CPP_SRC_INCLUDE_FIREBASE_FIRESTORE_SETTINGS_H_
#define FIREBASE_FIRESTORE_CLIENT_CPP_SRC_INCLUDE_FIREBASE_FIRESTORE_SETTINGS_H_

#if defined(__OBJC__)
#include <dispatch/dispatch.h>
#endif

#include <iosfwd>
#include <memory>
#include <string>

namespace firebase {
namespace firestore {

#if !defined(__ANDROID__) && !defined(FIRESTORE_STUB_BUILD)
namespace util {
class Executor;
}
#endif

class FirestoreInternal;

/** Settings used to configure a Firestore instance. */
class Settings final {
 public:
  /**
   * @brief Creates the default settings.
   */
  Settings();

  /**
   * @brief Copy constructor.
   *
   * This performs a deep copy, creating an independent instance.
   *
   * @param[in] other `Settings` to copy from.
   */
  Settings(const Settings& other) = default;

  /**
   * @brief Move constructor.
   *
   * Moving is more efficient than copying for `Settings`. After being moved
   * from, `Settings` is in a valid but unspecified state.
   *
   * @param[in] other `Settings` to move data from.
   */
  Settings(Settings&& other) = default;

  /**
   * @brief Copy assignment operator.
   *
   * This performs a deep copy, creating an independent instance.
   *
   * @param[in] other `Settings` to copy from.
   *
   * @return Reference to the destination `Settings`.
   */
  Settings& operator=(const Settings& other) = default;

  /**
   * @brief Move assignment operator.
   *
   * Moving is more efficient than copying for `Settings`. After being moved
   * from, `Settings` is in a valid but unspecified state.
   *
   * @param[in] other `Settings` to move data from.
   *
   * @return Reference to the destination `Settings`.
   */
  Settings& operator=(Settings&& other) = default;

  /**
   * Gets the host of the Firestore backend to connect to.
   */
  const std::string& host() const { return host_; }

  /**
   * Returns whether to use SSL when communicating.
   */
  bool is_ssl_enabled() const { return ssl_enabled_; }

  /**
   * Returns whether to enable local persistent storage.
   */
  bool is_persistence_enabled() const { return persistence_enabled_; }

  /**
   * Sets the host of the Firestore backend. The default is
   * "firestore.googleapis.com".
   *
   * @param host The host string.
   */
  void set_host(std::string host);

  /**
   * Enables or disables SSL for communication.
   *
   * @param enabled Set true to enable SSL for communication.
   */
  void set_ssl_enabled(bool enabled);

  /**
   * Enables or disables local persistent storage.
   *
   * @param enabled Set true to enable local persistent storage.
   */
  void set_persistence_enabled(bool enabled);

#if defined(__OBJC__)
  /**
   * A dispatch queue to be used to execute all completion handlers and event
   * handlers. By default, the main queue is used.
   */
  dispatch_queue_t dispatch_queue() const;

  void set_dispatch_queue(dispatch_queue_t queue);
#endif

  /**
   * Returns a string representation of these `Settings` for
   * logging/debugging purposes.
   *
   * @note the exact string representation is unspecified and subject to
   * change; don't rely on the format of the string.
   */
  std::string ToString() const;

  /**
   * Outputs the string representation of these `Settings` to the given
   * stream.
   *
   * @see `ToString()` for comments on the representation format.
   */
  friend std::ostream& operator<<(std::ostream& out, const Settings& settings);

 private:
  std::string host_;
  bool ssl_enabled_ = true;
  bool persistence_enabled_ = true;

#if !defined(__ANDROID__) && !defined(FIRESTORE_STUB_BUILD)
  friend class FirestoreInternal;
  std::unique_ptr<util::Executor> CreateExecutor() const;

  std::shared_ptr<const util::Executor> executor_;
#endif
};

}  // namespace firestore
}  // namespace firebase

#endif  // FIREBASE_FIRESTORE_CLIENT_CPP_SRC_INCLUDE_FIREBASE_FIRESTORE_SETTINGS_H_
