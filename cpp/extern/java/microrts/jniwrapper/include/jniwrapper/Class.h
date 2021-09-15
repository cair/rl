#include <utility>

//
// Created by per on 9/15/21.
//

#ifndef CAIRL_MICRORTS_CLASS_H
#define CAIRL_MICRORTS_CLASS_H
#include <iostream>

class Object{
    JNIEnv& env;
    jobject obj_ref;
    jfieldID obj_field_id;

public:
    explicit Object(JNIEnv& env, jobject obj, jfieldID obj_field_id)
    : env(env)
    , obj_ref(obj)
    , obj_field_id(obj_field_id){

    }

    int getInteger(){
        auto j =  obj_ref;
        return 1;
    }


};

class Class{
protected:
    JNIEnv& env;
    std::string className;
    jclass cls_ref;
public:



    explicit Class(JNIEnv& env, std::string _className)
    : env(env)
    , className(std::move(_className))
    , cls_ref(env.FindClass(className.c_str())){

    }

    Class main(const std::vector<std::string>& args){
        auto javaArgs = (jobjectArray)env.NewObjectArray(args.size(),env.FindClass("java/lang/String"),env.NewStringUTF(""));
        for(int i = 0; i < args.size(); i++) env.SetObjectArrayElement(javaArgs,i,env.NewStringUTF(args[i].c_str()));
        jmethodID mid = env.GetStaticMethodID(cls_ref, "main", "([Ljava/lang/String;)V");
        env.CallStaticVoidMethod(cls_ref, mid, javaArgs);
    }


    Object newInstance(){
        auto constructor = env.GetMethodID(cls_ref, "<init>", "()V");



    }


};


class Enum: public Class{

public:
    using Class::Class;


    Object getEnumValue(const std::string& enumName){
        auto fieldID = env.GetStaticFieldID(cls_ref, enumName.data(), ("L" + className +  ";").c_str());
        jobject jOject = env.GetStaticObjectField(cls_ref, fieldID);
        return Object(env, jOject, fieldID);
    }

    int getEnumValueInt(const std::string& enumName){
        auto fieldID = env.GetStaticFieldID(cls_ref, enumName.data(), ("L" + className +  ";").c_str());
        jint jOject = env.GetStaticIntField(cls_ref, fieldID);
        return jOject;
    }

    long getEnumValueLong(const std::string& enumName){
        auto fieldID = env.GetStaticFieldID(cls_ref, enumName.data(), ("L" + className +  ";").c_str());
        jlong jOject = env.GetStaticLongField(cls_ref, fieldID);
        return jOject;
    }




};



#endif //CAIRL_MICRORTS_CLASS_H
