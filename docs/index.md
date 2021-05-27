---
title: Defold Firebase Analytics documentation
brief: This manual covers how to setup and use Firebase Analytics in Defold.
---

# Defold Firebase Analytics documentation

This extension allows you to interact with Firebase Analytics in a uniform way for games on iOS and Android.


## Installation
To use this library in your Defold project, add the following URLs to your `game.project` dependencies:

| Firebase C++ SDK       | Dependencies |
|------------------------|--------------|
| Firebase C++ SDK 7.3.0 | [https://github.com/defold/extension-firebase/archive/refs/tags/1.1.2.zip](https://github.com/defold/extension-firebase/archive/master.zip)<br>[https://github.com/defold/extension-firebase-analytics/archive/refs/tags/2.0.1.zip](https://github.com/defold/extension-firebase-analytics/archive/refs/tags/2.0.1.zip) |


## Setup
Follow the [main setup guide for integration of Firebase in Defold](https://www.defold.com/extension-firebase).



## Usage

```lua
function init(self)
	-- use firebase only if it is supported on the current platform
    if not firebase then
        return
    end

	-- initialise firebase and check that it was successful
    local ok, err = firebase.init()
    if not ok then
        print(err)
        return
    end

	-- initialise analytics
	firebase.analytics.init(function(self, ok, err)
		if not ok then
			print(err)
			return
		end

		-- log data
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
	end)
end
```

## Source code

The source code is available on [GitHub](https://github.com/defold/extension-firebase-analytics)


## API reference
