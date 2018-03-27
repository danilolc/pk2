#Don't clean the JNI called methods

-keep public class org.libsdl.app.*
-keepclassmembers class org.libsdl.app.* {
    *;
}
