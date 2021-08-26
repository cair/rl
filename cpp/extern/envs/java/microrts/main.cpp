//
// Created by per on 8/26/21.
//

#include <jni.h>       /* where everything is defined */
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <iostream>

std::string generate_options(const std::vector<std::string>& paths){
    std::string s = "-Djava.class.path=";
    std::ostringstream imploded;
    std::copy(paths.begin(), paths.end(),
              std::ostream_iterator<std::string>(imploded, ":"));
    return s + imploded.str();
}


int main() {

    std::string option = generate_options({
        "/usr/bin/java",
        "MicroRTS-1.0.0.jar"
    });
    std::cout << option << std::endl;

    // .c_str();

    JavaVM *jvm;       /* denotes a Java VM */
    JNIEnv *env;       /* pointer to native method interface */
    JavaVMInitArgs vm_args; /* JDK/JRE 10 VM initialization arguments */
    auto options = JavaVMOption();
    options.optionString = option.data();
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 1;
    vm_args.options = &options;
    vm_args.ignoreUnrecognized = false;
    /* load and initialize a Java VM, return a JNI interface
     * pointer in env */
    JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);


    /*
     *   public GameSettings(LaunchMode launchMode, String serverAddress, int serverPort,
        int serializationType, String mapLocation, int maxCycles, int updateInterval,
        boolean partiallyObservable, int uttVersion, int confictPolicy,
        boolean includeConstantsInState, boolean compressTerrain, boolean headless, String AI1,
        String AI2)
     */

    /* invoke the Main.test method using the JNI */
    jclass cls = env->FindClass("rts/MicroRTS");
    if(!cls){
        std::cout << "Could not find MicroRTS" << std::endl;
        jvm->DestroyJavaVM();
        return 1;
    }

    std::vector<std::string> args = {
            "-f",
            "/home/per/git/code/rl/cpp/extern/envs/java/microrts/cmake-build-debug/_deps/microrts-src/resources/config.properties"
    };
    auto javaArgs = (jobjectArray)env->NewObjectArray(args.size(),env->FindClass("java/lang/String"),env->NewStringUTF(""));
    for(int i = 0; i < args.size(); i++) env->SetObjectArrayElement(javaArgs,i,env->NewStringUTF(args[i].c_str()));



    jmethodID mid = env->GetStaticMethodID(cls, "main", "([Ljava/lang/String;)V");
    env->CallStaticVoidMethod(cls, mid, javaArgs);

    /* We are done. */
    jvm->DestroyJavaVM();
}