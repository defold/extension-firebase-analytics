---
title: Defold Firebase Analytics documentation
brief: This manual covers how to setup and use Firebase Analytics in Defold.
---

# Defold Firebase Analytics documentation

This extension allows you to interact with Firebase Analytics in a uniform way for games on iOS and Android.


## Installation
To use Firabase in your Defold project, add a version of the Firebase extension to your `game.project` dependencies from the list of available [Firebase Releases](https://github.com/defold/extension-firebase/releases) and corresponding [Firebase Config Release](https://github.com/defold/extension-firebase-remoteconfig/releases).
Find the version you want for both extensions, copy the URLs to ZIP archive of the release and add it to the project dependencies.

![](add-dependency.png)


## Setup
Follow the [main setup guide for integration of Firebase in Defold](https://www.defold.com/extension-firebase).


## Usage

```lua
local function firebase_analytics_callback(self, message_id, message)
    if message_id == remoteconfig.MSG_ERROR then
        -- an error was detected when performing an analytics config operation
        print("Firebase Analytics Config error: ", message.error)
        return
    end

    if message_id == remoteconfig.MSG_INSTANCE_ID then
        -- result of the firebase.analytics.get_id() call
        print("Firebase Analytics Config instance_id: ", message.instance_id)
        return
    end
end

function init(self)
    -- use firebase only if it is supported on the current platform
    if not firebase then
        return
    end

    -- initialise firebase and check that it was successful
    firebase.set_callback(function(self, message_id, message)
        if message_id == firebase.MSG_INITIALIZED then
            firebase.remoteconfig.set_callback(firebase_analytics_callback)
            firebase.analytics.initialize()

            -- log data
            firebase.analytics.log_string("character", "storm trooper")
            firebase.analytics.log_int("kills", 152)
            firebase.analytics.log("tutorial_done")
            firebase.analytics.log_number("speed", 1.15)
            local t = {
                number = math.random(1,100),
                boolean = true,
                string = "some_string"
            }
            firebase.analytics.log_table("stats", t)
            firebase.analytics.get_id()
        end
    end)
end
```

## Source code

The source code is available on [GitHub](https://github.com/defold/extension-firebase-analytics)


## API reference
