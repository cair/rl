//
// Created by per on 9/15/21.
//

#ifndef CAIRL_MICRORTS_JAVA_H
#define CAIRL_MICRORTS_JAVA_H

#include "jni.h"
#include "Class.h"
#include <memory>

class Java{
    JavaVM* jvm = nullptr;       /* denotes a Java VM */
    JNIEnv* env = nullptr;       /* pointer to native method interface */

public:
    explicit Java(const std::vector<std::string>&& classPaths) {
        auto cp = _generate_class_paths(classPaths);
        JavaVMInitArgs vm_args; /* JDK/JRE 10 VM initialization arguments */
        auto options = JavaVMOption();
        options.optionString = cp.data();
        vm_args.version = JNI_VERSION_1_8;
        vm_args.nOptions = 1;
        vm_args.options = &options;
        vm_args.ignoreUnrecognized = false;

        JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);

    }
    ~Java(){
        jvm->DestroyJavaVM();
    }

    Enum getEnum(const std::string &cls){
        return Enum(*env, cls);
    }

    Class getClass(const std::string &cls){
        return Class(*env, cls);
    }

private:
    static std::string _generate_class_paths(const std::vector<std::string>& paths){
        std::string s = "-Djava.class.path=";
        std::ostringstream imploded;
        std::copy(paths.begin(), paths.end(),
                  std::ostream_iterator<std::string>(imploded, ":"));
        return s + imploded.str();
    }


};

#endif //CAIRL_MICRORTS_JAVA_H
