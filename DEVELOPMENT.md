# Notes on setup of this extension
The extension uses the [Firebase C++ SDK](https://firebase.google.com/docs/cpp/setup) version 5.7.0. The libs and includes have been copied from the SDK to the extension.

* Download link: https://firebase.google.com/download/cpp.
* Direct download link: https://dl.google.com/firebase/sdk/cpp/firebase_cpp_sdk_5.7.0.zip

## Android

### Java JAR dependencies
Base extension dependencies are generated and provided via the [android-base-extensions project](https://github.com/defold/android-base-extensions). The dependencies used by this extension need to match the ones specified in the Firebase C++ SDK [Android dependencies section of the Firebase SDK documentation](https://firebase.google.com/docs/cpp/setup#dependencies_2).

### C++ library dependencies
Libs from `firebase_cpp_sdk/libs/android/armeabi-v7a/c++/*` and `firebase_cpp_sdk/libs/android/arm64-v8a/c++/*`

## iOS
Setup for iOS is made [without CocoaPods](https://firebase.google.com/docs/ios/setup#frameworks) using a direct download of the Firebase iOS SDK. It is important to use a version of the iOS SDK matching the [iOS dependencies section of the Firebase SDK documentation](https://firebase.google.com/docs/cpp/setup#dependencies).

Direct download link: https://dl.google.com/firebase/sdk/ios/5_20_2/Firebase-5.20.2.zip

## Windows
Libs from `firebase_cpp_sdk/libs/windows/VS2015/MT/<arch>/Debug/` (MT meaning Multi-threaded static library as opposed to MD meaning Multi-threaded dynamic library).
