/*
 * Copyright 2016 Google LLC
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

#ifndef FIREBASE_AUTH_CLIENT_CPP_SRC_INCLUDE_FIREBASE_AUTH_TYPES_H_
#define FIREBASE_AUTH_CLIENT_CPP_SRC_INCLUDE_FIREBASE_AUTH_TYPES_H_

namespace firebase {
namespace auth {

/// All possible error codes from asynchronous calls.
/// For error details,
/// @if cpp_examples
/// call Future::ErrorMessage().
/// @endif
enum AuthError {
  /// Success.
  kAuthErrorNone = 0,

  /// Function will be implemented in a later revision of the API.
  kAuthErrorUnimplemented = -1,

  /// This indicates an internal error.
  /// Common error code for all API Methods.
  kAuthErrorFailure = 1,

  /// Indicates a validation error with the custom token.
  /// This error originates from "bring your own auth" methods.
  kAuthErrorInvalidCustomToken,

  /// Indicates the service account and the API key belong to different
  /// projects.
  /// Caused by "Bring your own auth" methods.
  kAuthErrorCustomTokenMismatch,

  /// Indicates the IDP token or requestUri is invalid.
  /// Caused by "Sign in with credential" methods.
  kAuthErrorInvalidCredential,

  /// Indicates the user’s account is disabled on the server.
  /// Caused by "Sign in with credential" methods.
  kAuthErrorUserDisabled,

  /// Indicates an account already exists with the same email address but using
  /// different sign-in credentials. Account linking is required.
  /// Caused by "Sign in with credential" methods.
  kAuthErrorAccountExistsWithDifferentCredentials,

  /// Indicates the administrator disabled sign in with the specified identity
  /// provider.
  /// Caused by "Set account info" methods.
  kAuthErrorOperationNotAllowed,

  /// Indicates the email used to attempt a sign up is already in use.
  /// Caused by "Set account info" methods.
  kAuthErrorEmailAlreadyInUse,

  /// Indicates the user has attemped to change email or password more than 5
  /// minutes after signing in, and will need to refresh the credentials.
  /// Caused by "Set account info" methods.
  kAuthErrorRequiresRecentLogin,

  /// Indicates an attempt to link with a credential that has already been
  /// linked with a different Firebase account.
  /// Caused by "Set account info" methods.
  kAuthErrorCredentialAlreadyInUse,

  /// Indicates an invalid email address.
  /// Caused by "Sign in with password" methods.
  kAuthErrorInvalidEmail,

  /// Indicates the user attempted sign in with a wrong password.
  /// Caused by "Sign in with password" methods.
  kAuthErrorWrongPassword,

  /// Indicates that too many requests were made to a server method.
  /// Common error code for all API methods.
  kAuthErrorTooManyRequests,

  /// Indicates the user account was not found.
  /// Send password request email error code.
  /// Common error code for all API methods.
  kAuthErrorUserNotFound,

  /// Indicates an attempt to link a provider to which the account is already
  /// linked.
  /// Caused by "Link credential" methods.
  kAuthErrorProviderAlreadyLinked,

  /// Indicates an attempt to unlink a provider that is not linked.
  /// Caused by "Link credential" methods.
  kAuthErrorNoSuchProvider,

  /// Indicates user's saved auth credential is invalid, the user needs to sign
  /// in again.
  /// Caused by requests with an STS id token.
  kAuthErrorInvalidUserToken,

  /// Indicates the saved token has expired.
  /// For example, the user may have changed account password on another device.
  /// The user needs to sign in again on the device that made this request.
  /// Caused by requests with an STS id token.
  kAuthErrorUserTokenExpired,

  /// Indicates a network error occurred (such as a timeout, interrupted
  /// connection, or unreachable host). These types of errors are often
  /// recoverable with a retry.
  /// Common error code for all API Methods.
  kAuthErrorNetworkRequestFailed,

  /// Indicates an invalid API key was supplied in the request.
  /// For Android these should no longer occur (as of 2016 v3).
  /// Common error code for all API Methods.
  kAuthErrorInvalidApiKey,

  /// Indicates the App is not authorized to use Firebase Authentication with
  /// the provided API Key.
  /// Common error code for all API Methods.
  /// On Android this error should no longer occur (as of 2016 v3).
  /// Common error code for all API Methods.
  kAuthErrorAppNotAuthorized,

  /// Indicates that an attempt was made to reauthenticate with a user which is
  /// not the current user.
  kAuthErrorUserMismatch,

  /// Indicates an attempt to set a password that is considered too weak.
  kAuthErrorWeakPassword,

  /// Internal api usage error code when there is no signed-in user
  /// and getAccessToken is called.
  ///
  /// @note This error is only reported on Android.
  kAuthErrorNoSignedInUser,

  /// This can happen when certain methods on App are performed, when the auth
  /// API is not loaded.
  ///
  /// @note This error is only reported on Android.
  kAuthErrorApiNotAvailable,

  /// Indicates the out-of-band authentication code is expired.
  kAuthErrorExpiredActionCode,

  /// Indicates the out-of-band authentication code is invalid.
  kAuthErrorInvalidActionCode,

  /// Indicates that there are invalid parameters in the payload during a
  /// "send password reset email" attempt.
  kAuthErrorInvalidMessagePayload,

  /// Indicates that an invalid phone number was provided.
  /// This is caused when the user is entering a phone number for verification.
  kAuthErrorInvalidPhoneNumber,

  /// Indicates that a phone number was not provided during phone number
  /// verification.
  ///
  /// @note This error is iOS-specific.
  kAuthErrorMissingPhoneNumber,

  /// Indicates that the recipient email is invalid.
  kAuthErrorInvalidRecipientEmail,

  /// Indicates that the sender email is invalid during a "send password reset
  /// email" attempt.
  kAuthErrorInvalidSender,

  /// Indicates that an invalid verification code was used in the
  /// verifyPhoneNumber request.
  kAuthErrorInvalidVerificationCode,

  /// Indicates that an invalid verification ID was used in the
  /// verifyPhoneNumber request.
  kAuthErrorInvalidVerificationId,

  /// Indicates that the phone auth credential was created with an empty
  /// verification code.
  kAuthErrorMissingVerificationCode,

  /// Indicates that the phone auth credential was created with an empty
  /// verification ID.
  kAuthErrorMissingVerificationId,

  /// Indicates that an email address was expected but one was not provided.
  kAuthErrorMissingEmail,

  /// Represents the error code for when an application attempts to create an
  /// email/password account with an empty/null password field.
  ///
  /// @note This error is only reported on Android.
  kAuthErrorMissingPassword,

  /// Indicates that the quota of SMS messages for a given project has been
  /// exceeded.
  kAuthErrorQuotaExceeded,

  /// Thrown when one or more of the credentials passed to a method fail to
  /// identify and/or authenticate the user subject of that operation. Inspect
  /// the error message to find out the specific cause.
  /// @note This error is only reported on Android.
  kAuthErrorRetryPhoneAuth,

  /// Indicates that the SMS code has expired.
  kAuthErrorSessionExpired,

  /// Indicates that the app could not be verified by Firebase during phone
  /// number authentication.
  ///
  /// @note This error is iOS-specific.
  kAuthErrorAppNotVerified,

  /// Indicates a general failure during the app verification flow.
  ///
  /// @note This error is iOS-specific.
  kAuthErrorAppVerificationFailed,

  /// Indicates that the reCAPTCHA token is not valid.
  ///
  /// @note This error is iOS-specific.
  kAuthErrorCaptchaCheckFailed,

  /// Indicates that an invalid APNS device token was used in the verifyClient
  /// request.
  ///
  /// @note This error is iOS-specific.
  kAuthErrorInvalidAppCredential,

  /// Indicates that the APNS device token is missing in the verifyClient
  /// request.
  ///
  /// @note This error is iOS-specific.
  kAuthErrorMissingAppCredential,

  /// Indicates that the clientID used to invoke a web flow is invalid.
  ///
  /// @note This error is iOS-specific.
  kAuthErrorInvalidClientId,

  /// Indicates that the domain specified in the continue URI is not valid.
  ///
  /// @note This error is iOS-specific.
  kAuthErrorInvalidContinueUri,

  /// Indicates that a continue URI was not provided in a request to the backend
  /// which requires one.
  kAuthErrorMissingContinueUri,

  /// Indicates an error occurred while attempting to access the keychain.
  /// Common error code for all API Methods.
  ///
  /// @note This error is iOS-specific.
  kAuthErrorKeychainError,

  /// Indicates that the APNs device token could not be obtained. The app may
  /// not have set up remote notification correctly, or may have failed to
  /// forward the APNs device token to FIRAuth if app delegate swizzling is
  /// disabled.
  ///
  /// @note This error is iOS-specific.
  kAuthErrorMissingAppToken,

  /// Indicates that the iOS bundle ID is missing when an iOS App Store ID is
  /// provided.
  ///
  /// @note This error is iOS-specific.
  kAuthErrorMissingIosBundleId,

  /// Indicates that the app fails to forward remote notification to FIRAuth.
  ///
  /// @note This error is iOS-specific.
  kAuthErrorNotificationNotForwarded,

  /// Indicates that the domain specified in the continue URL is not white-
  /// listed in the Firebase console.
  ///
  /// @note This error is iOS-specific.
  kAuthErrorUnauthorizedDomain,

  /// Indicates that an attempt was made to present a new web context while one
  /// was already being presented.
  kAuthErrorWebContextAlreadyPresented,

  /// Indicates that the URL presentation was cancelled prematurely by the user.
  kAuthErrorWebContextCancelled,

  /// Indicates that Dynamic Links in the Firebase Console is not activated.
  kAuthErrorDynamicLinkNotActivated,

  /// Indicates that the operation was cancelled.
  kAuthErrorCancelled,

  /// Indicates that the provider id given for the web operation is invalid.
  kAuthErrorInvalidProviderId,

  /// Indicates that an internal error occurred during a web operation.
  kAuthErrorWebInternalError,

  /// Indicates that 3rd party cookies or data are disabled, or that there was
  /// a problem with the browser.
  kAuthErrorWebStorateUnsupported,

  /// Indicates that the provided tenant ID does not match the Auth instance's
  /// tenant ID.
  kAuthErrorTenantIdMismatch,

  /// Indicates that a request was made to the backend with an associated tenant
  /// ID for an operation that does not support multi-tenancy.
  kAuthErrorUnsupportedTenantOperation,

  /// Indicates that an FDL domain used for an out of band code flow is either
  /// not configured or is unauthorized for the current project.
  kAuthErrorInvalidLinkDomain,

  /// Indicates that credential related request data is invalid. This can occur
  /// when there is a project number mismatch (sessionInfo, spatula header,
  /// temporary proof),
  /// an incorrect temporary proof phone number, or during game center sign in
  /// when the user is
  /// already signed into a different game center account.
  kAuthErrorRejectedCredential,

  /// Indicates that the phone number provided in the MFA sign in flow to be
  /// verified does not correspond to a phone second factor for the user.
  kAuthErrorPhoneNumberNotFound,

  /// Indicates that a request was made to the backend with an invalid tenant
  /// ID.
  kAuthErrorInvalidTenantId,
};

}  // namespace auth
}  // namespace firebase

#endif  // FIREBASE_AUTH_CLIENT_CPP_SRC_INCLUDE_FIREBASE_AUTH_TYPES_H_
