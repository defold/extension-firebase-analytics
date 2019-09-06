# Notes on setup of this extension
The extension uses the [Firebase C++ SDK](https://firebase.google.com/docs/cpp/setup). The libs and includes have been copied from the SDK to the extension.

Download link: https://firebase.google.com/download/cpp.
Direct download link: https://dl.google.com/firebase/sdk/cpp/firebase_cpp_sdk_5.7.0.zip

## Android
Dependencies are downloaded using `configure.py`. The required dependencies and their versions are specified in `configure.py` and need to match the ones specified in the Firebase C++ SDK [Android dependencies section of the Firebase SDK documentation](https://firebase.google.com/docs/cpp/setup#dependencies_2). The Python script will download and parse the Maven POMs of the dependencies and resolving any additional dependencies. All of the AAR and JAR files will be downloaded and in the case of AAR files unpacked. A single AndroidManifest.xml will be generated with all permissions and various other settings applied.

Libs from `firebase_cpp_sdk/libs/android/armeabi-v7a/c++/*` and `firebase_cpp_sdk/libs/android/arm64-v8a/c++/*`

We already have `jar` dependencies included in the Defold engine, that's why we use only `com-google-firebase-firebase-analytics-firebase-analytics-16.4.0`. Remove all `jar` files except `com-google-firebase-firebase-analytics-firebase-analytics-16.4.0` after `configure.py` running.

## iOS
Setup for iOS is made [without CocoaPods](https://firebase.google.com/docs/ios/setup#frameworks) using a direct download of the Firebase iOS SDK. It is important to use a version of the iOS SDK matching the [iOS dependencies section of the Firebase SDK documentation](https://firebase.google.com/docs/cpp/setup#dependencies).

Direct download link: https://dl.google.com/firebase/sdk/ios/5_20_2/Firebase-5.20.2.zip

## Windows
Libs from `firebase_cpp_sdk/libs/windows/VS2015/MT/<arch>/Debug/` (MT meaning Multi-threaded static library as opposed to MD meaning Multi-threaded dynamic library).

## Maven
https://mvnrepository.com/artifact/com.google.android
