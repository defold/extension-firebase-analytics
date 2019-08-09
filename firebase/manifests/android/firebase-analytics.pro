-keep class com.google.firebase.iid.FirebaseInstanceId { com.google.firebase.iid.FirebaseInstanceId getInstance(); }
-keep class com.google.firebase.iid.FirebaseInstanceId { com.google.firebase.iid.FirebaseInstanceId getInstance(com.google.firebase.FirebaseApp); }
-keep class com.google.firebase.iid.FirebaseInstanceIdReceiver { *; }
-keep class com.google.firebase.iid.FirebaseInstanceIdService { *; }
-keep class com.google.firebase.iid.Registrar { *; }

-keep class com.google.firebase.FirebaseApp { com.google.firebase.FirebaseApp initializeApp(android.content.Context,com.google.firebase.FirebaseOptions,java.lang.String); }
-keep class com.google.firebase.FirebaseApp { com.google.firebase.FirebaseApp initializeApp(android.content.Context,com.google.firebase.FirebaseOptions); }
-keep class com.google.firebase.FirebaseApp { com.google.firebase.FirebaseApp initializeApp(android.content.Context); }
-keep class com.google.firebase.FirebaseApp { com.google.firebase.FirebaseApp getInstance(java.lang.String); }
-keep class com.google.firebase.FirebaseApp { com.google.firebase.FirebaseApp getInstance(); }
-keep class com.google.firebase.FirebaseApp { boolean isDataCollectionDefaultEnabled(); }
-keep class com.google.firebase.FirebaseApp { void setDataCollectionDefaultEnabled(boolean); }

-keep class com.google.firebase.FirebaseOptions$Builder { *; }
-keep class com.google.firebase.FirebaseOptions { *; }

-keep class com.google.firebase.analytics.** { *; }

-keep class com.google.android.gms.measurement.AppMeasurementInstallReferrerReceiver { *; }