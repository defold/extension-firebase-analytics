---
title: Defold Firebase Analytics documentation
brief: This manual covers how to setup and use Firebase Analytics in Defold.
---

# Defold Firebase Analytics documentation

This extension allows you to interact with Firebase Analytics in a uniform way for games on iOS and Android.


## Installation
To use this library in your Defold project, add the following URL to your `game.project` dependencies:

https://github.com/defold/extension-firebase-analytics/archive/master.zip

We recommend using a link to a zip file of a [specific release](https://github.com/defold/extension-firebase-analytics/releases).

## Setup
### 1. Firebase setup
The steps below taken from the [official Google Firebase Guides](https://firebase.google.com/docs/cpp/setup).

#### 1.1 Create a Firebase project
* Create a Firebase project in the [Firebase console](https://console.firebase.google.com/), if you don't already have one. Click Add project. If you already have an existing Google project associated with your mobile app, select it from the Project name drop down menu. Otherwise, enter a project name to create a new project.
* Optional: Edit your Project ID. Your project is given a unique ID automatically, and it's used in publicly visible Firebase features such as database URLs and your Firebase Hosting subdomain. You can change it now if you want to use a specific subdomain.
* Follow the remaining setup steps and click Create project (or Add Firebase if you're using an existing project) to begin provisioning resources for your project. This typically takes a few minutes. When the process completes, you'll be taken to the project overview.

#### 1.2 Setup for Android
* Click Add Firebase to your Android app and follow the setup steps. If you're importing an existing Google project, this may happen automatically and you can just download the config file.
* When prompted, enter your app's package name. It's important to enter the package name your app is using; this can only be set when you add an app to your Firebase project.
* During the process, you'll download a `google-services.json` file. You can download this file again at any time.

#### 1.3 Setup for iOS
* Click Add Firebase to your iOS app and follow the setup steps. If you're importing an existing Google project, this may happen automatically and you can just download the config file.
* When prompted, enter your app's bundle ID. It's important to enter the bundle ID your app is using; this can only be set when you add an app to your Firebase project.
* During the process, you'll download a `GoogleService-Info.plist` file. You can download this file again at any time.

### 2. Defold setup
#### 2.1 Add project dependencies
You can use the extension in your own project by adding this project as a [Defold library dependency](http://www.defold.com/manuals/libraries/). Open your game.project file and in the dependencies field under project add:

[https://github.com/defold/extension-firebase/archive/master.zip](https://github.com/defold/extension-firebase/archive/master.zip)

Or point to the ZIP file of a [specific release](https://github.com/defold/extension-firebase/releases) (recommended!).

#### 2.2 Setup for Android

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

* Open `game.project` and set the `Bundle Resources` entry under the `Project` section to `/bundle` to match the folder created in the step above. Read more about the `Bundle Resources` setting in the [Defold manual](https://www.defold.com/manuals/project-settings/#_project).


#### 2.3 Setup for iOS
* Copy the generated `GoogleService-Info.plist` file to a folder structure like this:

```
<project_root>
 |
 +-bundle
    |
    +-ios
       |
       +-GoogleService-Info.plist
```

* Open `game.project` and set the `Bundle Resources` entry under the `Project` section to `/bundle` to match the folder created in the step above. Read more about the `Bundle Resources` setting in the [Defold manual](https://www.defold.com/manuals/project-settings/#_project).


## Usage

```lua
function init(self)
	-- use firebase only if it is supported on the current platform
	if firebase then
		firebase.init()
		firebase.analytics.set_screen("myscreen", "collection")
		firebase.analytics.log_string("character", "storm trooper")
		firebase.analytics.log_int("kills", 152)
		firebase.analytics.log_number("speed", 1.15)
		local t = {
			number = math.random(1,100),
			boolean = true,
			string = "some_string"
		}
		firebase.analytics.log_table("stats", t)
	end
end
```

## Source code

The source code is available on [GitHub](https://github.com/defold/extension-firebase-analytics)



## API reference
