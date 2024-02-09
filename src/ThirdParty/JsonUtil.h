//
// Created by syimlzhu on 2017/12/12.
//

#ifndef __JSON_UTIL_H_CODELIB
#define __JSON_UTIL_H_CODELIB

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "third-party-libs/rapidjson/stringbuffer.h"
#include "third-party-libs/rapidjson/writer.h"
#include <list>
#include <exception>
#include <string>
#include <cstdio>
#include <vector>
#include <third-party-libs/rapidjson/error/en.h>

using namespace rapidjson;
class JsonUtilException : public std::exception {

public:
    JsonUtilException(std::string reason) {
        this->reason = reason;
    }

    virtual char const *what() const throw() { return reason.c_str(); }

    std::string reason;
};

struct JsonUtilNode {
    JsonUtilNode(const char *path, Value *value) {
        this->path = path;
        this->value = value;
        hasItr = false;
    }

    JsonUtilNode(std::string &path, Value *value) {
        this->path = path;
        this->value = value;
        hasItr = false;
    }

    JsonUtilNode(std::string &path, Value *value, Value::ConstMemberIterator &memberItr) {
        this->path = path;
        this->value = value;
        hasItr = false;//TODO: 保存itr   hasItr=true
        this->memberItr = memberItr;
    }

    JsonUtilNode(const char *path, Value *value, Value::ConstMemberIterator &memberItr) {
        this->path = path;
        this->value = value;
        hasItr = false;//TODO: 保存itr   hasItr=true
        this->memberItr = memberItr;
    }

    std::string path;
    Value *value;
    Value::ConstMemberIterator memberItr;
    bool hasItr;
};

class JsonUtil;
class JsonReadableInterface
{
public:
    virtual void ReadByJson(const JsonUtil& json) = 0;
};

class JsonUtil {
private:
    Document d;
    //std::list<std::string> path;
    //std::list<Value*> values;
    mutable std::list<JsonUtilNode> nodes;
    mutable Value *nowValue;
    mutable Value::ConstMemberIterator nowMemberItr;
    mutable bool hasItr;
    mutable bool isEnd;

    Value *getFa() const {
        if (nodes.empty()) return NULL;
        return nodes.back().value;
    }

public:
    JsonUtil() {
        d.Parse("{}");
        nowValue = &d;
        isEnd = false;
        hasItr = false;
    }

    JsonUtil(const std::string& json) {
        d.Parse<kParseCommentsFlag|kParseTrailingCommasFlag>(json.c_str());
        if(d.HasParseError())
        {
            ParseErrorCode code = d.GetParseError();
            size_t offset = d.GetErrorOffset();
            std::string err = GetParseError_En(code);
            throw JsonUtilException("Parse Error [call JsonUtil(const char *, int)] ErrCode="+std::to_string(code)+"("+err+") offset="+std::to_string(offset));
        }
        nowValue = &d;
        isEnd = false;
        hasItr = false;
    }

    JsonUtil(const char *json, int len) {
        d.Parse<kParseCommentsFlag|kParseTrailingCommasFlag>(json,(size_t)len);
        if(d.HasParseError())
        {
            ParseErrorCode code = d.GetParseError();
            size_t offset = d.GetErrorOffset();
            std::string err = GetParseError_En(code);
            throw JsonUtilException("Parse Error [call JsonUtil(const char *, int)] ErrCode="+std::to_string(code)+"("+err+") offset="+std::to_string(offset));
        }
        nowValue = &d;
        isEnd = false;
        hasItr = false;
    }

    /*JsonUtil(FILE *file) {
        char ch[1024];
        FileReadStream fs(file, ch, sizeof(ch));

        d.ParseStream(fs);
        nowValue = &d;
        isEnd = false;
    }*/

    //废弃：
    // 删除json字符串中的注释
    // 在Parse的时候增加了kParseCommentsFlag参数。可以支持 // ... 及多行 /* ... */ 注释
    static std::string Minify(const std::string& str)
    {
        bool InComment = false;
        bool InStr = false;
        std::string ret;
        if(str.empty()) return ret;
        ret.push_back(str[0]);
        for(size_t i=1;i<str.length();i++)
        {
            if(!InComment && str[i] == '\"')
            {
                InStr = !InStr;
                ret.push_back(str[i]);
                continue;
            }
            if(!InStr)
            {
                if(str[i-1] == '/' && str[i] == '*')
                {
                    InComment = true;
                    ret.erase(ret.length()-1);
                    i++;
                    continue;
                }
            }
            if(InComment)
            {
                if(str[i-1] == '*' && str[i] == '/')
                {
                    InComment = false;
                    i++;
                    ret.push_back(str[i]);
                    continue;
                }
            }
            else
            {
                ret.push_back(str[i]);
            }
        }
        return ret;
    }

public:

    std::string toRootString() const
    {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);
        return buffer.GetString();
    }

    std::string toNowNodeString() const
    {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        nowValue->Accept(writer);
        return buffer.GetString();
    }

    void enter(std::string &key) const {
        if (!nowValue->IsObject()) {
            throw JsonUtilException(getPath() + " is not an JsonObject [call enter(string)]");
        }
        if (!nowValue->HasMember(key.c_str())) {
            throw JsonUtilException(getPath() + " has not member [" + key + "] [call enter(string)]");
        }
        Value *value = &((*nowValue)[key.c_str()]);
        if (hasItr) {
            nodes.push_back(JsonUtilNode(key, nowValue, nowMemberItr));
        } else {
            nodes.push_back(JsonUtilNode(key, nowValue));
        }
        nowValue = value;
    }

    void enter(const char *key) const {
        std::string k(key);
        enter(k);
    }

    void enter(int index) const {
        char sIndex[12];
        sprintf(sIndex, "%d", index);
        if (!nowValue->IsArray()) {
            throw JsonUtilException(getPath() + " is not an JsonArray [call enter(int)]");
        }
        if (index < 0 || index >= (int) nowValue->Size()) {
            throw JsonUtilException(getPath() + " has not index [" + sIndex + "] [call enter(int)]");
        }
        Value *value = &((*nowValue)[index]);
        if (hasItr) {
            nodes.push_back(JsonUtilNode(sIndex, nowValue, nowMemberItr));
        } else {
            nodes.push_back(JsonUtilNode(sIndex, nowValue));
        }
        nowValue = value;
    }

    void quit() const {
        if (nodes.empty()) {
            throw JsonUtilException("already in root [call quit()]");
        }
        nowValue = nodes.back().value;
        nowMemberItr = nodes.back().memberItr;
        hasItr = nodes.back().hasItr;
        nodes.pop_back();
        isEnd = false;
    }

    bool isInRoot() const
    {
        return nodes.empty();
    }

    bool beginFor() const {
        if (nowValue->IsObject()) {
            if (nowValue->MemberCount() == 0) {
                nodes.push_back(JsonUtilNode("-", nowValue));
                isEnd = true;
                nowValue = NULL;
                return false;
            }
            Value::ConstMemberIterator itr = nowValue->MemberBegin();
            std::string key = itr->name.GetString();
            enter(key);
            return true;
        }
        if (nowValue->IsArray()) {
            if (nowValue->Empty()) {
                nodes.push_back(JsonUtilNode("-", nowValue));
                isEnd = true;
                nowValue = NULL;
                return false;
            }
            enter(0);
            return true;
        }
        throw JsonUtilException(getPath() + " is not an JsonArray or an JsonObject [call enterFirst()]");
    }

    bool next() const {
        if (nodes.empty()) {
            throw JsonUtilException(getPath() + " in root [call next()]");
        }
        if (isEnd) {
            throw JsonUtilException(getPath() + " in end [call next()]");
        }
        Value *fa = getFa();
        if (fa->IsArray()) {
            int index;
            sscanf(getKey().c_str(), "%d", &index);
            if (index == (int) fa->Size() - 1) {
                isEnd = true;
                nodes.back().path = "-";
                return false;
            }
            quit();
            enter(index + 1);
        }
        if (fa->IsObject()) {
            Value::ConstMemberIterator itr;
            std::string key = getKey();
            if (!hasItr) {
                for (itr = fa->MemberBegin(); itr != fa->MemberEnd(); itr++) {
                    if (itr->name.GetString() == key) {
                        nowMemberItr = itr;
                        //hasItr = true; //TODO: 保存itr   itr后面会失效？  现在遍历的话每次调用next()都从头遍历
                        break;
                    }
                }
            }
            nowMemberItr++;
            if (nowMemberItr == fa->MemberEnd()) {
                isEnd = true;
                nodes.back().path = "-";
                return false;
            }
            std::string nextKey = nowMemberItr->name.GetString();
            quit();
            enter(nextKey);
            nowMemberItr = itr;
        }
        return false;
    }

    bool checkEnd() const {
        return isEnd;
    }

    bool endFor() const {
        if (isEnd) {
            quit();
            return true;
        }
        return false;
    }

    bool has(std::string &key) const {
        if (!nowValue->IsObject()) {
            return false;
        }
        return nowValue->HasMember(key.c_str());
    }

    bool has(const char *key) const {
        if (!nowValue->IsObject()) {
            return false;
        }
        return nowValue->HasMember(key);
    }

    std::string getPath() const {
        std::string ret;
        ret.append("json[");
        for (auto &it : nodes) {
            ret.append("/");
            ret.append(it.path);
        }
        ret.append("/]");
        return ret;
    }

    std::string getKey() const {
        if (nodes.empty()) return "";
        return nodes.back().path;
    };

    int getArraySize() const {
        if (!nowValue->IsArray()) {
            throw JsonUtilException(getPath() + " is not an JsonArray [call getArraySize()]");
        }
        return nowValue->Size();
    }

    bool isEmptyObject() const {
        return nowValue->IsObject() && nowValue->MemberCount() == 0;
    }

    bool isNull() const {
        return nowValue->IsNull();
    }

    bool isEmptyArray() const {
        return nowValue->IsArray() && nowValue->Empty();
    }

    int getInt() const {
        if (!nowValue->IsString() && !nowValue->IsInt()) {
            throw JsonUtilException(getPath() + " is not an Int [call getInt()]");
        }
        if (nowValue->IsInt()) return nowValue->GetInt();

        std::string str = nowValue->GetString();
        int v = 0;
        int ret = sscanf(str.c_str(), "%i", &v);
        if (ret != 1) { throw JsonUtilException(getPath() + " is not an Int [call getInt()]"); }
        return v;
//                if (!nowValue->IsInt()) { throw JsonUtilException(getPath() + " is not an Int [call getInt()]"); }
//                return nowValue->GetInt();
    }

    int64_t getInt64() const
    {
        if (/*!nowValue->IsString() &&*/ !nowValue->IsInt64()) {
            throw JsonUtilException(getPath() + " is not an Int64 [call getInt64()]");
        }
        /*if (nowValue->IsInt64())*/ return nowValue->GetInt64();
        /*
        std::string str = nowValue->GetString();
        int64_t v = 0;
        int ret = sscanf(str.c_str(), "%lld", &v);
        if (ret != 1) { throw JsonUtilException(getPath() + " is not an Int64 [call getInt()]"); }
        return v;*/
    }

    int get0xInt() const {
        if (!nowValue->IsString()) {
            throw JsonUtilException(getPath() + " is not an 0xInt [call get0xInt()]");
        }
        std::string str = nowValue->GetString();
        int v = 0;
        int ret = sscanf(str.c_str(), "%i", &v);
        if (ret != 1) { throw JsonUtilException(getPath() + " is not an 0xInt [call get0xInt()]"); }
        return v;
    }

    double getDouble() const {
        if (nowValue->IsInt()) {
            return getInt();
        }
        if (!nowValue->IsDouble()) {
            throw JsonUtilException(getPath() + " is not a Double [call getDouble()]");
        }
        return nowValue->GetDouble();
    }

    std::string getString() const {
        if (!nowValue->IsString()) {
            throw JsonUtilException(getPath() + " is not a String [call getString()]");
        }
        return nowValue->GetString();
    }

    bool getBool() const {
        if (!nowValue->IsBool()) { throw JsonUtilException(getPath() + " is not a Bool [call getBool()]"); }
        return nowValue->GetBool();
    }

    bool getBool(const char *key) const {
        enter(key);
        bool ret = getBool();
        quit();
        return ret;
    }

    void getIntVector(std::vector<int32_t> &vector) const {
        for (beginFor(); !endFor(); next()) {
            vector.push_back(get0xInt());
        }
    }

    void getIntVector(const char *key, std::vector<int32_t> &vector) const {
        enter(key);
        for (beginFor(); !endFor(); next()) {
            vector.push_back(get0xInt());
        }
        quit();
    }

    void getStringVector(const char *key, std::vector<std::string> &vector) const {
        enter(key);
        for (beginFor(); !endFor(); next()) {
            vector.push_back(getString());
        }
        quit();
    }

    void getStringVector(std::vector<std::string> &vector) const {
        for (beginFor(); !endFor(); next()) {
            vector.push_back(getString());
        }
    }

    void GetDoubleVector(const char *key, std::vector<double> &vector)const{
        enter(key);
        for (beginFor(); !endFor(); next()) {
            vector.push_back(getDouble());
        }
        quit();
    }
    //类型T实现 JsonReadableInterface 的 ReadByJson(JsonUtil& json);
    void getClass(JsonReadableInterface *t) const {
        t->ReadByJson(*this);
    }

    //类型T实现 ReadByJson(JsonUtil& json);
    void getClass(const char *key, JsonReadableInterface *t) const {
        enter(key);
        t->ReadByJson(*this);
        quit();
    }

    //类型T实现 ReadByJson(codelib::module::JsonUtil& json);
    template<typename T>
    void getClassVector(std::vector<T> *t) const {
        for (beginFor(); !endFor(); next()) {
            T cla;
            t->push_back(cla);
            (*t)[t->size() - 1].ReadByJson(*this);
        }
    }

    //类型T实现 ReadByJson(codelib::module::JsonUtil& json);
    template<typename T>
    void getClassVector(const char *key, std::vector<T> *t) const
    {
        enter(key);
        getClassVector<T>(t);
        quit();
    }

/*
    template<typename T>
    void getClassProtobufRepeated(google::protobuf::RepeatedPtrField< T > *t) const
    {
        for (beginFor(); !endFor(); next()) {
            auto pT = t->Add();
            pT->ReadByJson(*this);
        }
    }

    template<typename T>
    void getClassProtobufRepeated(const char *key,google::protobuf::RepeatedPtrField< T > *t) const
    {
        enter(key);
        for (beginFor(); !endFor(); next()) {
            auto pT = t->Add();
            pT->ReadByJson(*this);
        }
        quit();
    }
*/


    int getInt(std::string &key) const {
        enter(key);
        int ret = getInt();
        quit();
        return ret;
    }

    int get0xInt(std::string &key) const {
        enter(key);
        int ret = get0xInt();
        quit();
        return ret;
    }

    double getDouble(std::string &key) const {
        enter(key);
        double ret = getDouble();
        quit();
        return ret;
    }

    std::string getString(std::string &key) const {
        enter(key);
        std::string ret = getString();
        quit();
        return ret;
    }

    int getInt(const char *key) const {
        std::string k(key);
        return getInt(k);
    }

    int get0xInt(const char *key) const {
        enter(key);
        int ret = get0xInt();
        quit();
        return ret;
    }

    int64_t getInt64(const char *key) const {
        std::string k(key);
        return getInt64(k);
    }
    int64_t getInt64(std::string &key) const {
        enter(key);
        int64_t ret = getInt64();
        quit();
        return ret;
    }

    double getDouble(const char *key) const {
        std::string k(key);
        return getDouble(k);
    }

    std::string getString(const char *key) const {
        std::string k(key);
        return getString(k);
    }

    bool isObject() const { return nowValue->IsObject(); }

    bool isArray() const { return nowValue->IsArray(); }

    bool isInt() const { return nowValue->IsInt(); }

    bool isDouble() const { return nowValue->IsDouble(); }

    bool isString() const { return nowValue->IsString(); }

    bool isInt64() const { return nowValue->IsInt64(); }

    bool isBool() const { return nowValue->IsBool(); }

    bool isInt(const char* key) const
    {
        if(!isObject()) return false;
        if(!has(key)) return false;
        enter(key);
        bool ret = isInt();
        if(!ret && isString())
        {
            //如果是字符串，并且能转化成整数，就返回true
            std::string str = nowValue->GetString();
            int v = 0;
            int ret2 = sscanf(str.c_str(), "%i", &v);
            if (ret2 == 1)
            {
                quit();
                return true;
            }
        }
        quit();
        return ret;
    }

    //编辑功能
    //object
    JsonUtil& addString(const char *key,const char *value)
    {
        if (!nowValue->IsObject()) {
            throw JsonUtilException(getPath() + " is not an Object [call addString()]");
        }
        Value str(rapidjson::kStringType);
        size_t len = strlen(value);
        str.SetString(value,len,d.GetAllocator());

        Value k(rapidjson::kStringType);
        k.SetString(key,strlen(key),d.GetAllocator());
        nowValue->AddMember(k,str,d.GetAllocator());
        return *this;
    }
    JsonUtil& addString(const char *key,const std::string& value)
    {
        if (!nowValue->IsObject()) {
            throw JsonUtilException(getPath() + " is not an Object [call addString()]");
        }
        Value str(rapidjson::kStringType);
        str.SetString(value.c_str(),value.length(),d.GetAllocator());

        Value k(rapidjson::kStringType);
        k.SetString(key,strlen(key),d.GetAllocator());
        nowValue->AddMember(k,str,d.GetAllocator());
        return *this;
    }
    JsonUtil& addInt(const char *key,int32_t value)
    {
        if (!nowValue->IsObject()) {
            throw JsonUtilException(getPath() + " is not an Object [call addInt()]");
        }
        Value k(rapidjson::kStringType);
        k.SetString(key,strlen(key),d.GetAllocator());
        nowValue->AddMember(k,value,d.GetAllocator());
        return *this;
    }
    JsonUtil& addJson(const char *key,const JsonUtil& json)
    {
        //深复制整个json，以免json失效
        if (!nowValue->IsObject()) {
            throw JsonUtilException(getPath() + " is not an Object [call addJson()]");
        }
        Value k(rapidjson::kStringType);
        k.SetString(key,strlen(key),d.GetAllocator());
        Value v(*json.nowValue,d.GetAllocator());
        nowValue->AddMember(k,v,d.GetAllocator());
        return *this;
    }
    JsonUtil& setJson(JsonUtil& json){
        Value v(*json.nowValue,d.GetAllocator());
        *nowValue = v;
        return *this;
    }
    JsonUtil& removeJson(const char *key){
        if (!nowValue->IsObject()) {
            throw JsonUtilException(getPath() + " is not an Object [call removeJson()]");
        }
        Value k(rapidjson::kStringType);
        k.SetString(key,strlen(key));
        nowValue->RemoveMember(k);
        return *this;
    }
    JsonUtil& removeArray(int32_t index){
        if (!nowValue->IsArray()) {
            throw JsonUtilException(getPath() + " is not an Array [call removeArray()]");
        }
        if (index < 0 || index >= (int) nowValue->Size()) {
            throw JsonUtilException(getPath() + " has not index ");
        }
        nowValue->Erase(nowValue->Begin() + index);
        return *this;
    }
/*
    JsonUtil& addJson(const char* key, const JsonUtil& json)
    {
        if (!nowValue->IsObject()) {
            throw JsonUtilException(getPath() + " is not an Object [call addJson()]");
        }
        if(json.isObject())
        {
            addObject(key);
            enter(key);
            for(json.beginFor();!json.endFor();json.next())
            {
                if(json.isObject() || json.isArray())
                {
                    addJson(json.getKeyConstCharPtr(),json);
                }
                else if(json.isInt())
                {
                    addInt(json.getKeyConstCharPtr(),json.getInt());
                }
                else if(json.isString())
                {
                    addString(json.getKeyConstCharPtr(),json.getString());
                }
                else if(json.isDouble())
                {
                    addDouble(json.getKeyConstCharPtr(),json.getDouble());
                }
                else if(json.isInt64())
                {
                    addInt64(json.getKeyConstCharPtr(),json.getInt64());
                }
                else if(json.isBool())
                {
                    addInt64(json.getKeyConstCharPtr(),json.getBool());
                }
                //没别的情况了
            }
            quit();
        }
        else if(json.isArray())
        {
            addArray(key);
            enter(key);
            for(json.beginFor();!json.endFor();json.next())
            {
                if(json.isObject() || json.isArray())
                {
                    pushJson(json);
                }
                else if(json.isInt())
                {
                    pushInt(json.getInt());
                }
                else if(json.isString())
                {
                    pushString(json.getString().c_str());
                }
                else if(json.isDouble())
                {
                    pushDouble(json.getDouble());
                }
                else if(json.isInt64())
                {
                    pushInt64(json.getInt64());
                }
                else if(json.isBool())
                {
                    pushBool(json.getBool());
                }
                //没别的情况了
            }
            quit();
        }
        else if(json.isInt())
        {
            addInt(key,json.getInt());
        }
        else if(json.isString())
        {
            addString(key,json.getString().c_str());
        }
        else if(json.isDouble())
        {
            addDouble(key,json.getDouble());
        }
        else if(json.isInt64())
        {
            addInt64(key,json.getInt64());
        }
        else if(json.isBool())
        {
            addBool(key,json.getBool());
        }
        return *this;
    }
*/
    JsonUtil& addInt64(const char *key,int64_t value)
    {
        if (!nowValue->IsObject()) {
            throw JsonUtilException(getPath() + " is not an Object [call addInt64()]");
        }
        Value k(rapidjson::kStringType);
        k.SetString(key,strlen(key),d.GetAllocator());
        nowValue->AddMember(k,value,d.GetAllocator());
        return *this;
    }
    JsonUtil& addDouble(const char *key,double value)
    {
        if (!nowValue->IsObject()) {
            throw JsonUtilException(getPath() + " is not an Object [call addDouble()]");
        }
        Value k(rapidjson::kStringType);
        k.SetString(key,strlen(key),d.GetAllocator());
        nowValue->AddMember(k,value,d.GetAllocator());
        return *this;
    }
    JsonUtil& addBool(const char *key,bool value)
    {
        if (!nowValue->IsObject()) {
            throw JsonUtilException(getPath() + " is not an Object [call addBool()]");
        }
        Value k(rapidjson::kStringType);
        k.SetString(key,strlen(key),d.GetAllocator());
        nowValue->AddMember(k,value,d.GetAllocator());
        return *this;
    }
    JsonUtil& addArray(const char *key)
    {
        if (!nowValue->IsObject()) {
            throw JsonUtilException(getPath() + " is not an Object [call addArray()]");
        }
        Value arr(rapidjson::kArrayType);
        Value k(rapidjson::kStringType);
        k.SetString(key,strlen(key),d.GetAllocator());
        nowValue->AddMember(k,arr,d.GetAllocator());
        return *this;
    }
    JsonUtil& addObject(const char *key)
    {
        if (!nowValue->IsObject()) {
            throw JsonUtilException(getPath() + " is not an Object [call addObject()]");
        }
        Value arr(rapidjson::kObjectType);
        Value k(rapidjson::kStringType);
        k.SetString(key,strlen(key),d.GetAllocator());
        nowValue->AddMember(k,arr,d.GetAllocator());
        return *this;
    }
    JsonUtil& remove(const char* key)
    {
        nowValue->RemoveMember(key);
        return *this;
    }

    //array
    JsonUtil& pushString(const char *value)
    {
        if (!nowValue->IsArray()) {
            throw JsonUtilException(getPath() + " is not an Array [call pushString()]");
        }
        Value str(rapidjson::kStringType);
        size_t len = strlen(value);
        str.SetString(value,len,d.GetAllocator());
        nowValue->PushBack(str,d.GetAllocator());
        return *this;
    }
    JsonUtil& pushString(const std::string &value)
    {
        if (!nowValue->IsArray()) {
            throw JsonUtilException(getPath() + " is not an Array [call pushString()]");
        }
        Value str(rapidjson::kStringType);
        size_t len = value.length();
        str.SetString(value.c_str(),len,d.GetAllocator());
        nowValue->PushBack(str,d.GetAllocator());
        return *this;
    }
    JsonUtil& pushInt(int32_t value)
    {
        if (!nowValue->IsArray()) {
            throw JsonUtilException(getPath() + " is not an Array [call pushInt()]");
        }
        nowValue->PushBack(value,d.GetAllocator());
        return *this;
    }
    JsonUtil& pushInt64(int64_t value)
    {
        if (!nowValue->IsArray()) {
            throw JsonUtilException(getPath() + " is not an Array [call pushInt()]");
        }
        nowValue->PushBack(value,d.GetAllocator());
        return *this;
    }
    JsonUtil& pushDouble(double value)
    {
        if (!nowValue->IsArray()) {
            throw JsonUtilException(getPath() + " is not an Array [call pushDouble()]");
        }
        nowValue->PushBack(value,d.GetAllocator());
        return *this;
    }
    JsonUtil& pushBool(bool value)
    {
        if (!nowValue->IsArray()) {
            throw JsonUtilException(getPath() + " is not an Array [call pushBool()]");
        }
        nowValue->PushBack(value,d.GetAllocator());
        return *this;
    }
    JsonUtil& pushObject()
    {
        if (!nowValue->IsArray()) {
            throw JsonUtilException(getPath() + " is not an Array [call pushObject()]");
        }
        Value obj(rapidjson::kObjectType);
        nowValue->PushBack(obj,d.GetAllocator());
        return *this;
    }
    JsonUtil& pushArray()
    {
        if (!nowValue->IsArray()) {
            throw JsonUtilException(getPath() + " is not an Array [call pushArray()]");
        }
        Value arr(rapidjson::kArrayType);
        nowValue->PushBack(arr,d.GetAllocator());
        return *this;
    }
    JsonUtil& pushJson(const JsonUtil& json)
    {
        if (!nowValue->IsArray()) {
            throw JsonUtilException(getPath() + " is not an Array [call pushJson2()]");
        }
        //深复制
        Value v(*json.nowValue,d.GetAllocator());
        nowValue->PushBack(v,d.GetAllocator());
        return *this;
    }


    /*
    JsonUtil& pushJson(const JsonUtil& json)
    {
        if (!nowValue->IsArray()) {
            throw JsonUtilException(getPath() + " is not an Array [call pushJson()]");
        }
        if(json.isObject())
        {
            pushObject();
            enter(getArraySize() - 1);
            for(json.beginFor();!json.endFor();json.next())
            {
                if(json.isObject() || json.isArray())
                {
                    addJson(json.getKeyConstCharPtr(),json);
                }
                else if(json.isInt())
                {
                    addInt(json.getKeyConstCharPtr(),json.getInt());
                }
                else if(json.isString())
                {
                    addString(json.getKeyConstCharPtr(),json.getString());
                }
                else if(json.isDouble())
                {
                    addDouble(json.getKeyConstCharPtr(),json.getDouble());
                }
                else if(json.isInt64())
                {
                    addInt64(json.getKeyConstCharPtr(),json.getInt64());
                }
                else if(json.isBool())
                {
                    addBool(json.getKeyConstCharPtr(),json.getBool());
                }
                //没别的情况了
            }
            quit();
        }
        else if(json.isArray())
        {
            pushArray();
            enter(getArraySize() - 1);
            for(json.beginFor();!json.endFor();json.next())
            {
                if(json.isObject() || json.isArray())
                {
                    pushJson(json);
                }
                else if(json.isInt())
                {
                    pushInt(json.getInt());
                }
                else if(json.isString())
                {
                    pushString(json.getString().c_str());
                }
                else if(json.isDouble())
                {
                    pushDouble(json.getDouble());
                }
                else if(json.isInt64())
                {
                    pushInt64(json.getInt64());
                }
                else if(json.isBool())
                {
                    pushBool(json.getBool());
                }
                //没别的情况了
            }
            quit();
        }
        else if(json.isInt())
        {
            pushInt(json.getInt());
        }
        else if(json.isString())
        {
            pushString(json.getString().c_str());
        }
        else if(json.isDouble())
        {
            pushDouble(json.getDouble());
        }
        else if(json.isInt64())
        {
            pushInt64(json.getInt64());
        }
        else if(json.isBool())
        {
            pushBool(json.getBool());
        }

        return *this;
    }*/
};


#endif //MS2_JSONUTIL_H
