# Firebase
Google Firebase extension for the [Defold](https://www.defold.com) game engine.

# NOT PRODUCTION READY
This extension is still under development. The extension works on Android and iOS but it needs further testing.

# Setup
## 1. Firebase setup
The steps below taken from the [official Google Firebase Guides](https://firebase.google.com/docs/cpp/setup).

### 1.1 Create a Firebase project
* Create a Firebase project in the [Firebase console](https://console.firebase.google.com/), if you don't already have one. Click Add project. If you already have an existing Google project associated with your mobile app, select it from the Project name drop down menu. Otherwise, enter a project name to create a new project.
* Optional: Edit your Project ID. Your project is given a unique ID automatically, and it's used in publicly visible Firebase features such as database URLs and your Firebase Hosting subdomain. You can change it now if you want to use a specific subdomain.
* Follow the remaining setup steps and click Create project (or Add Firebase if you're using an existing project) to begin provisioning resources for your project. This typically takes a few minutes. When the process completes, you'll be taken to the project overview.

### 1.2 Setup for Android
* Click Add Firebase to your Android app and follow the setup steps. If you're importing an existing Google project, this may happen automatically and you can just download the config file.
* When prompted, enter your app's package name. It's important to enter the package name your app is using; this can only be set when you add an app to your Firebase project.
* During the process, you'll download a `google-services.json` file. You can download this file again at any time.

### 1.3 Setup for iOS
* Click Add Firebase to your iOS app and follow the setup steps. If you're importing an existing Google project, this may happen automatically and you can just download the config file.
* When prompted, enter your app's bundle ID. It's important to enter the bundle ID your app is using; this can only be set when you add an app to your Firebase project.
* During the process, you'll download a `GoogleService-Info.plist` file. You can download this file again at any time.

## 2. Defold setup
### 2.1 Add project dependencies
You can use the extension in your own project by adding this project as a [Defold library dependency](http://www.defold.com/manuals/libraries/). Open your game.project file and in the dependencies field under project add:

https://github.com/defold/extension-firebase/archive/master.zip

Or point to the ZIP file of a [specific release](https://github.com/defold/extension-firebase/releases).

### 2.2 Setup for Android
* Open `game.project` and scroll to the Android section. Select the `firebase/AndroidManifest.xml` file in the Android Manifest field. This is a modified version of the default Android Manifest file with all of the required Firebase and Google Play Game Services additions.
* Run `generate_xml_from_google_services_json.py` or `generate_xml_from_google_services_json.exe` (both from Firebase C++ SDK) to convert the previously downloaded `google-services.json` to an Android resource XML:

```
$ ./generate_xml_from_google_services_json.py -i google-services.json -o google-services.xml
```

* Copy the generated `google-services.xml` file to a folder structure like this:

```
<project_root>
 |
 +-bundle
    |
    +-android
       |
       +-res
          |
          +-values
             |
             +-google-services.xml
```

* Open `game.project` set the `Bundle Resources` entry under the `Project` section to `/bundle` to match the folder created in the step above. Read more about the `Bundle Resources` setting in the [Defold manual](https://www.defold.com/manuals/project-settings/#_project).

* Finally you also need to make sure that the default Android Support and Google Play Services libs are excluded from Defold engine in favor of the ones provided by this extension. You do this by setting `firebase/firebase.appmanifest` as the App Manifest in the Native Extension section of `game.project` (or merging with an existing `.appmanifest` if you already have one).

### 2.3 Setup for iOS
* Copy the generated `GoogleService-Info.plist` file to a folder structire like this:

```
<project_root>
 |
 +-bundle
    |
    +-ios
       |
       +-GoogleService-Info.plist
```

* Open `game.project` set the `Bundle Resources` entry under the `Project` section to `/bundle` to match the folder created in the step above. Read more about the `Bundle Resources` setting in the [Defold manual](https://www.defold.com/manuals/project-settings/#_project).


# Usage
## Firebase API
### firebase.init([config])
Initialize the Firebase API

## Analytics API
### firebase.analytics.log(name)
Log an event with no parameters. [Official docs](https://firebase.google.com/docs/reference/cpp/namespace/firebase/analytics#logevent_5).

### firebase.analytics.log_string(name, parameter_name, parameter_value)
Log an event with one string parameter. [Official docs](https://firebase.google.com/docs/reference/cpp/namespace/firebase/analytics#logevent).

### firebase.analytics.log_int(name, parameter_name, parameter_value)
Log an event with one integer parameter. [Official docs](https://firebase.google.com/docs/reference/cpp/namespace/firebase/analytics#logevent_4).

### firebase.analytics.log_number(name, parameter_name, parameter_value)
Log an event with one float parameter. [Official docs](https://firebase.google.com/docs/reference/cpp/namespace/firebase/analytics#logevent_2).

### firebase.analytics.log_table(name, parameters_table)
Log an event with table parameters. [Official docs](https://firebase.google.com/docs/reference/cpp/namespace/firebase/analytics#logevent_6).

### firebase.analytics.set_screen(screen_name, screen_class)
Sets the current screen name and screen class, which specifies the current visual context in your app. [Official docs](https://firebase.google.com/docs/reference/cpp/namespace/firebase/analytics#setcurrentscreen).

### firebase.analytics.set_minimum_session_duration(milliseconds)
Sets the minimum engagement time required before starting a session. [Official docs](https://firebase.google.com/docs/reference/cpp/namespace/firebase/analytics#setminimumsessionduration).

### firebase.analytics.set_user_id(user_id)
Sets the user ID property. [Official docs](https://firebase.google.com/docs/reference/cpp/namespace/firebase/analytics#setuserid).

### firebase.analytics.set_user_property(name, property)
Set a user property to the given value. [Official docs](https://firebase.google.com/docs/reference/cpp/namespace/firebase/analytics#setuserproperty).

### firebase.analytics.reset()
Clears all analytics data for this app from the device and resets the app instance id. [Official docs](https://firebase.google.com/docs/reference/cpp/namespace/firebase/analytics#resetanalyticsdata).

### firebase.analytics.get_id()
Get the instance ID from the analytics service. [Official docs](https://firebase.google.com/docs/reference/cpp/namespace/firebase/analytics#getanalyticsinstanceid).


# Notes on setup of this extension
The extension uses the [Firebase C++ SDK](https://firebase.google.com/docs/cpp/setup). The libs and includes have been copied from the SDK to the extension.

Download link: https://firebase.google.com/download/cpp.
Direct download link: https://dl.google.com/firebase/sdk/cpp/firebase_cpp_sdk_5.4.3.zip

## Android
Dependencies are downloaded using `configure.py`. The required dependencies and their versions are specified in `configure.py` and need to match the ones specified in the Firebase C++ SDK [Android dependencies section of the Firebase SDK documentation](https://firebase.google.com/docs/cpp/setup#dependencies_2). The Python script will download and parse the Maven POMs of the dependencies and resolving any additional dependencies. All of the AAR and JAR files will be downloaded and in the case of AAR files unpacked. A single AndroidManifest.xml will be generated with all permissions and various other settings applied.

Libs from `firebase_cpp_sdk/libs/android/armeabi-v7a/gnustl/*`

There is currently a conflict with resources from `com-google-android-gms-play-services-basement-play-services-basement-16.0.1.jar` and the same resources always being included by the Defold engine (as part of the built in Firebase Push support). The `configure.py` script will exclude the resources from `com-google-android-gms-play-services-basement-play-services-basement-16.0.1.jar`.

## iOS
Setup for iOS is made [without CocoaPods](https://firebase.google.com/docs/ios/setup#frameworks) using a direct download of the Firebase iOS SDK. It is important to use a version of the iOS SDK matching the [iOS dependencies section of the Firebase SDK documentation](https://firebase.google.com/docs/cpp/setup#dependencies).

Direct download link: https://dl.google.com/firebase/sdk/ios/5_12_0/Firebase-5.12.0.zip

## Windows
Libs from `firebase_cpp_sdk/libs/windows/VS2015/MT/<arch>/Debug/` (MT meaning Multi-threaded static library as opposed to MD meaning Multi-threaded dynamic library).

## Maven
https://mvnrepository.com/artifact/com.google.android
