// Copyright 2016 Google Inc. All Rights Reserved.

#ifndef FIREBASE_FUNCTIONS_CLIENT_CPP_SRC_INCLUDE_FIREBASE_FUNCTIONS_H_
#define FIREBASE_FUNCTIONS_CLIENT_CPP_SRC_INCLUDE_FIREBASE_FUNCTIONS_H_

#include <string>

#include "firebase/app.h"
#include "firebase/functions/callable_reference.h"
#include "firebase/functions/callable_result.h"
#include "firebase/functions/common.h"

namespace firebase {

/// Namespace for the Firebase C++ SDK for Cloud Functions.
namespace functions {

/// @cond FIREBASE_APP_INTERNAL
namespace internal {
class FunctionsInternal;
}  // namespace internal
/// @endcond

class FunctionsReference;

/// @brief Entry point for the Firebase C++ SDK for Cloud Functions.
///
/// To use the SDK, call firebase::functions::Functions::GetInstance() to
/// obtain an instance of Functions, then use GetHttpsCallable() to obtain
/// references to callable functions. From there you can call them with
/// CallableReference::Call().
class Functions {
 public:
  /// @brief Destructor. You may delete an instance of Functions when
  /// you are finished using it, to shut down the Functions library.
  ~Functions();

  /// @brief Get an instance of Functions corresponding to the given App.
  ///
  /// Cloud Functions uses firebase::App to communicate with Firebase
  /// Authentication to authenticate users to the server backend.
  ///
  /// @param[in] app An instance of firebase::App. Cloud Functions will use
  /// this to communicate with Firebase Authentication.
  /// @param[out] init_result_out Optional: If provided, write the init result
  /// here. Will be set to kInitResultSuccess if initialization succeeded, or
  /// kInitResultFailedMissingDependency on Android if Google Play services is
  /// not available on the current device.
  ///
  /// @returns An instance of Functions corresponding to the given App.
  static Functions* GetInstance(::firebase::App* app,
                                InitResult* init_result_out = nullptr);

  /// @brief Get an instance of Functions corresponding to the given App.
  ///
  /// Cloud Functions uses firebase::App to communicate with Firebase
  /// Authentication to authenticate users to the server backend.
  ///
  /// @param[in] app An instance of firebase::App. Cloud Functions will use
  /// this to communicate with Firebase Authentication.
  /// @param[in] region The region to call functions in.
  /// @param[out] init_result_out Optional: If provided, write the init result
  /// here. Will be set to kInitResultSuccess if initialization succeeded, or
  /// kInitResultFailedMissingDependency on Android if Google Play services is
  /// not available on the current device.
  ///
  /// @returns An instance of Functions corresponding to the given App.
  static Functions* GetInstance(::firebase::App* app, const char* region,
                                InitResult* init_result_out = nullptr);

  /// @brief Get the firebase::App that this Functions was created with.
  ///
  /// @returns The firebase::App this Functions was created with.
  ::firebase::App* app();

  /// @brief Get a FunctionsReference for the specified path.
  HttpsCallableReference GetHttpsCallable(const char* name) const;

  /// @brief Sets an origin for a Cloud Functions emulator to use.
  void UseFunctionsEmulator(const char* origin);

 private:
  /// @cond FIREBASE_APP_INTERNAL
  Functions(::firebase::App* app, const char* region);
  Functions(const Functions& src);
  Functions& operator=(const Functions& src);

  internal::FunctionsInternal* internal_;
  /// @endcond
};

}  // namespace functions
}  // namespace firebase

#endif  // FIREBASE_FUNCTIONS_CLIENT_CPP_SRC_INCLUDE_FIREBASE_FUNCTIONS_H_
