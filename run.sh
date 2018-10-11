# Enable additional Firebase Analytics logging
adb shell setprop log.tag.FA VERBOSE

# enable Firebase Analytics debug view logging on the dashboard
adb shell setprop debug.firebase.analytics.app com.defold.firebase
#adb shell setprop debug.firebase.analytics.app .none.

adb shell am start -n com.defold.firebase/com.dynamo.android.DefoldActivity
#adb shell ps | grep com.defold.firebase | cut -c10-15
adb logcat | grep -F "`adb shell ps | grep com.defold.firebase | cut -c10-15`"
