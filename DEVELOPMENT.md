# Notes on setup of this extension
The extension uses the [Firebase C++ SDK](https://firebase.google.com/docs/cpp/setup) version 6.16.0. The libs and includes have been copied from the SDK to the extension.

* Download link: https://firebase.google.com/download/cpp
* Direct download link: https://dl.google.com/firebase/sdk/cpp/firebase_cpp_sdk_6.16.0.zip

## Android

### Java JAR dependencies
The dependencies used by this extension need to match the ones specified in `firebase_cpp_sdk/Android/firebase_dependencies.gradle` -> `firebaseDependenciesMap -> 'analytics'`.

### C++ library dependencies
Libs from `firebase_cpp_sdk/libs/android/armeabi-v7a/c++/*` and `firebase_cpp_sdk/libs/android/arm64-v8a/c++/*`

## iOS
Setup for iOS is made [without CocoaPods](https://firebase.google.com/docs/ios/setup#frameworks) using a direct download of the Firebase iOS SDK. It is important to use a version of the iOS SDK matching the [iOS dependencies section of the Firebase SDK documentation](https://firebase.google.com/docs/cpp/setup#dependencies).

Direct download link: https://github.com/firebase/firebase-ios-sdk/releases/download/CocoaPods-6.34.0/Firebase.zip

Defold doesn't support `*.xcframework` files. You need to unpack all the `*.framework` files from `*.xcframework` and put to folders according to architecture.

## Windows
Libs from `firebase_cpp_sdk/libs/windows/VS2015/MT/<arch>/Debug/` (MT meaning Multi-threaded static library as opposed to MD meaning Multi-threaded dynamic library).
