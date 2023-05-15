#ifndef DFE_JSON_HPP
#define DFE_JSON_HPP

#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<map>
#include<unordered_map>
#include<tuple>
#include<initializer_list>
#include<stdexcept>
#include<algorithm>
#include<cmath>
#include<cstdlib>
#include<type_traits>

#ifdef __GNUC__
#define INLINE inline __attribute__((always_inline))
#define NOINLINE inline __attribute__((noinline))
#else
#define INLINE inline
#define NOINLINE inline
#endif

namespace detail{

}

/**
 * @namespace dfe
 * @brief deep-fried-eggplant.com
*/
namespace dfe{
    /**
     * @class Json
     * @brief Json class
    */    
    class Json;

    /**
     * @namespace detail
     * @brief detail implement
    */
    namespace detail{
        class JsonDecoder;

        /**
         * @class JsonIterator
         * @brief the implement of the Json instance::iterator
        */
        class JsonIterator;

        /**
         * @class JsonConstIterator
         * @brief the implement of the Json instance::ConstIterator
        */
        class JsonConstIterator;

        class JsonNumber{
            friend std::ostream& operator<<(std::ostream&,const JsonNumber&);
            public:
            using Int   =long long int;
            using Float =long double;
            template<typename T>
            using enable_if_float=typename std::enable_if<std::is_floating_point<T>::value,std::nullptr_t>;
            template<typename T>
            using enable_if_int=typename std::enable_if<std::is_integral<T>::value,std::nullptr_t>;
            private:
            union{
                Int _int;
                Float _float;
            };
            bool _isFloat;
            public:
            JsonNumber();
            template<typename T,typename enable_if_float<T>::type=nullptr>
            JsonNumber(const T value);
            template<typename T,typename enable_if_int<T>::type=nullptr>
            JsonNumber(const T value);

            template<typename T>
            T get() const;
            template<typename T,typename enable_if_float<T>::type=nullptr>
            void set(const T value);
            template<typename T,typename enable_if_int<T>::type=nullptr>
            void set(const T value);
            bool isFloat() const;
            bool isInt() const;

            bool operator==(const JsonNumber& other) const;
            bool operator!=(const JsonNumber& other) const;
        };
        std::ostream& operator<<(std::ostream& os,const JsonNumber& value);
    }

    /**
     * @namespace exception
     * @brief namespace for exception class
    */
    namespace exception{
        /**
         * @class JsonException
         * @brief the exception class for Json
        */
        class JsonException;
    }

    class Json{
        friend class detail::JsonDecoder;
        friend std::ostream& operator<< (std::ostream&,const Json&) noexcept;

        public:

        using value_type=Json;
        using reference=Json&;
        using Null=std::nullptr_t;
        using Number=detail::JsonNumber;
        using Bool=bool;
        using String=std::string;
        using Array=std::vector<Json>;
        using Object=std::unordered_map<std::string,Json>;

        using Iterator=detail::JsonIterator;
        using ConstIterator=detail::JsonConstIterator;
        using Exception=exception::JsonException;

        public:

        /**
         * @enum ValueType
         * @brief enumerate class representing the value type of the Json instance
        */
        enum class ValueType{
            Null,Bool,Number,String,Array,Object
        };

        /**
         * @brief default constructor
        */
        Json() noexcept;

        /**
         * @brief constructor for Number-type
         * @param value Integer or floating point, which defined as Number, or bool, as Bool
        */
        template<typename NumberT,typename std::enable_if<
            std::is_integral<NumberT>::value||std::is_floating_point<NumberT>::value,std::nullptr_t
        >::type=nullptr>
        Json(const NumberT &value) noexcept;

        /**
         * @brief constructor for Null-type
         * @param value std::nullptr_t, which defined as Null
        */
        Json(const Null     value) noexcept;

        /**
         * @brief constructor for String-type
         * @param value const char* to create std::string, which defined as String
        */
        Json(const char*    value) noexcept;

        /**
         * @brief constructor for String-type
         * @param value std::string, which defined as String
        */
        Json(const String  &value) noexcept;

        /**
         * @brief constructor for Array-type
         * @param value std::vector<Json>, which defined as Array
        */
        Json(const Array   &value) noexcept;

        
        /**
         * @brief constructor for Object-type
         * @param value std::vector<Json>, which defined as Array
        */
        Json(const Object  &value) noexcept;

        /**
         * @brief copy-constructor
         * @param value lvalue of the Json instance
        */
        Json(const Json    &value) noexcept;
        
        /**
         * @brief move-constructor for rvalue
         * @param value rvalue of the Json instance
        */
        Json(      Json   &&value) noexcept;

        /**
         * @brief constructor for Array-type
         * @param value initializer_list to make std::vector<Json>, which defined as Array
        */
        Json(const std::initializer_list<Json> &list) noexcept;

        /**
         * @brief constructor for the type of the value only specified
         * @param value the type of the value
        */
        Json(const ValueType type) noexcept;
        ~Json() noexcept;

        private:
        union{
            Null    _null;
            Bool    _bool;
            Number  _number;
            String *_string;
            Array  *_array;
            Object *_object;
        };
        ValueType _valueType;

        public:
        
        /**
         * @fn getNull
         * @brief get the Null-type value of the Json instance
         * @throw exeption::JsonException if the type of the value is not Null
         * @return nullptr
        */
        Null    getNull()   const;

        /**
         * @fn getBool
         * @brief get the Bool-type value of the Json instance
         * @throw exception::JsonException if the type of the value is not Bool
         * @return boolean value in the Json instance
        */
        Bool    getBool()   const;

        /**
         * @fn getNumber
         * @brief get the Number-type value of the Json instance
         * @throw exception::JsonException if the type of the value is not Number
         * @return number value in the Json instance
        */
        Number  getNumber() const;

        template<typename NumberT>
        NumberT getNumber() const;

        /**
         * @fn getString
         * @brief get the String-type value of the Json instance
         * @throw exception::JsonException if the type of the value is not String
         * @return string value in the Json instance
        */
        String  getString() const;

        /**
         * @fn getArray
         * @brief get the Array-type value of the Json instance 
         * @throw exception::JsonException if the type of the value is not Array
         * @return array value in the Json instance
        */
        Array   getArray()  const;

        /**
         * @fn getObject
         * @brief get the Object-type value of the Json instance
         * @throw exception::JsonException if the type of the value is not Object
         * @return object value in the Json instance
        */
        Object  getObject() const;

        /**
         * @fn valueType
         * @brief get the value type of the Json instance
         * @return the value type of the Json instance
        */
        ValueType valueType() const noexcept;

        /**
         * @fn isNull
         * @brief whether the value type of the Json instance is Null or not
         * @return true if the type of the value is Bool, or false
        */
        bool isNull  () const noexcept;
        
        /**
         * @fn isBool
         * @brief whether the value type of the Json instance is Bool or not
         * @return true if the type of the value is Bool, or false
        */
        bool isBool  () const noexcept;
        
        /**
         * @fn isNumber
         * @brief whether the value type of the Json instance is Number or not
         * @return true if the type of the value is Number, or false
        */
        bool isNumber() const noexcept;
        
        /**
         * @fn isNull
         * @brief whether the value type of the Json instance is String or not
         * @return true if the type of the value is String, or false
        */
        bool isString() const noexcept;
        
        /**
         * @fn isArray
         * @brief whether the value type of the Json instance is Array or not
         * @return true if the type of the value is Array, or false
        */
        bool isArray () const noexcept;
        
        /**
         * @fn isObject
         * @brief whether the value type of the Json instance is Object or not
         * @return true if the type of the value is Object, or false
        */
        bool isObject() const noexcept;

        /**
         * @fn clear
         * @brief clear the value of the Json instance and make it empty
        */
        void clear() noexcept;
        
        /**
         * @fn reset
         * @brief clear the value of the Json instance and reset its type of value
         * @param type the new type of value in the Json instance
        */
        void reset(ValueType type) noexcept;
        
        /**
         * @fn load
         * @brief read the given JSON file and create a Json instance
         * @param path the path to the JSON file
         * @return a Json instance which its value is of the given JSON file
        */
        static Json load(const std::string& path);

        /**
         * @fn save
         * @brief save the value of the Json instance to a JSON file
         * @param value the Json instance to save
         * @param path the path to the JSON file
         * @param indent if true, save with indent of 4 spaces (default is true)
         * @return the Json instance which its value is of the JSON file
        */
        static void save(const Json &json,const std::string& path,const bool indent=true);
        
        /**
         * @fn fromString
         * @brief Parse the JSON string and create the instance
         * @param jsonStr JSON string
         * @return the Json instance which its value is of the JSON file
        */
        static Json fromString(const std::string& jsonStr);

        /**
         * @fn toString
         * @brief dump the value of the Json instance to a JSON string
         * @param value the Json instance to dump
         * @param indent if true, dump with indent of 4 spaces (default is false)
         * @return the JSON string
        */
        static std::string toString(const Json &value,const bool indent=false) noexcept;

        /**
         * @fn toString
         * @brief dump the type of the value to string
         * @param type the value of enum class ValueType
         * @return the string reresenting ValueType
        */
        static std::string toString(const ValueType type) noexcept;

        Json& operator= (const Json &value);
        Json& operator= (      Json&&value);

        bool operator==(const Json& other) const;
        bool operator!=(const Json& other) const;

        //functions for both Array and Object
        
        /**
         * @fn begin
         * @brief return the read/write iterator to the first element of Array/Object
         * @throw exception::JsonException if the Json instance is neither Array-type nor Object-type
         * @return the read/write iterator to the first element in the Array/Object
        */
        Iterator begin();

        /**
         * @fn begin
         * @brief return the read only ConstIterator to the first element of Array/Object
         * @throw exception::JsonException if the Json instance is neither Array-type nor Object-type
         * @return the the read-only iterator to the first element in the Array/Object
        */
        ConstIterator begin() const;

        /**
         * @fn cbegin
         * @brief return the read only ConstIterator to the first element of Array/Object
         * @throw exception::JsonException if the Json instance is neither Array-type nor Object-type
         * @return the the read-only iterator to the first element in the Array/Object
        */
        ConstIterator cbegin() const;

        /**
         * @fn end
         * @brief return the read/write iterator next to the last element of Array/Object
         * @throw exception::JsonException if the Json instance is neither Array-type nor Object-type
         * @return the read/write iterator next to the last element in the Array/Object
        */
        Iterator end();

        /**
         * @fn end
         * @brief return the read-only iterator next to the last element of the Array/Object-type Json instance
         * @throw exception::JsonException if the Json instance is neither Array-type nor Object-type
         * @return the read-only iterator pointing next to the last element in the Array/Object
        */
        ConstIterator end() const;

        /**
         * @fn cend
         * @brief return the read only iterator next to the last element of the Array/Object-type Json instance
         * @throw exception::JsonException if the type of the value is neither Array nor Object
         * @return the read-only iterator pointing next to the last element in the Array/Object
        */
        ConstIterator cend() const;

        /**
         * @fn size
         * @brief return the number of elements in the Array/Object-type Json instance
         * @throw exception::JsonException if the Json instance is neither Array-type nor Object-type
         * @return the number of elements in the Array/Object
        */
        size_t size() const;

        /**
         * @fn find
         * @brief find the given value in the Array-type Json instance
         * @throw exception::JsonException if the Json instance is not Array-type
         * @return the read/write iterator to the element if the given value has found, or end()
        */
        Iterator find(const Json& value);

        /**
         * @fn find
         * @brief find the given value from the Object-type Json instance
         * @throw exception::JsonException if the value type of the Json instance is not Object
         * @return the read/write iterator to the element if the given value has found, or end()
        */
        Iterator find(const std::string &key);

        /**
         * @fn find
         * @brief find the given value from the Array-type Json instance.
         * @throw exception::JsonException if the value type of the Json instance is not Array
         * @return the read/write iterator to the element if the given value has found, or cend()
        */
        ConstIterator find(const Json& value) const;
        
        /**
         * @fn find
         * @brief find the given value from the Object-type Json instance.
         * @throw exception::JsonException if the value type of the Json instance is not Object
         * @return the read/write iterator to the element if the given value has found, or cend()
        */
        ConstIterator find(const std::string &key) const;

        //functions for Array
        void append(const Json& value);
        void insert(const Array::size_type index, const Json& value);
        Json& at(const Array::size_type index);
        const Json& at(const Array::size_type index) const;
        Json& operator [](const Array::size_type index);
        const Json& operator [](const Array::size_type index) const;

        //functions for Object
        void append(const std::string &key,const Json &value);
        void insert(const std::string &key,const Json &value);
        bool contains(const std::string &key) const;
        std::vector<std::string> keys() const;
        void erase(const std::string &key);
        Json& at(const std::string &key);
        const Json& at(const std::string &key) const;
        Json& operator [] (const std::string &key);
        const Json& operator [] (const std::string &key) const;

    };
    
    namespace detail{
        class JsonDecoder{
            friend class ::dfe::Json;
            using StrIter=std::string::const_iterator;
            using ValueType=::dfe::Json::ValueType;
            static bool decode(const StrIter &start,const StrIter &end,::dfe::Json &out);
            static std::pair<bool,StrIter> getParsedValue(const StrIter &start,const StrIter &end,::dfe::Json &out) noexcept;
            static std::tuple<bool,StrIter,ValueType> detectValueType(const StrIter &start,const StrIter &end) noexcept;
            static std::tuple<bool,StrIter,Json::Null  > tryParseToNull  (const StrIter &start,const StrIter &end) noexcept;
            static std::tuple<bool,StrIter,Json::Bool  > tryParseToBool  (const StrIter &start,const StrIter &end) noexcept;
            static std::tuple<bool,StrIter,Json::Number> tryParseToNumber(const StrIter &start,const StrIter &end) noexcept;
            static std::tuple<bool,StrIter,Json::String> tryParseToString(const StrIter &start,const StrIter &end) noexcept;
            static std::tuple<bool,StrIter,Json::Array > tryParseToArray (const StrIter &start,const StrIter &end) noexcept;
            static std::tuple<bool,StrIter,Json::Object> tryParseToObject(const StrIter &start,const StrIter &end) noexcept;
            static bool isValidEmptyChar(const char ch) noexcept;
            static bool isValidEmptyString(const StrIter &start,const StrIter &end) noexcept;
            static std::string escapeStr(const std::string &str) noexcept;
            static std::string unicodeToUtf8(const uint16_t b) noexcept;
            public:
            JsonDecoder()=delete;
        };
        class JsonIterator{
            friend class ::dfe::Json;
            Json& _json;
            union{
                Json::Array::iterator  _arrayIter;
                Json::Object::iterator _objectIter;
            };
            Json::ValueType _valueType;
            
            JsonIterator()=delete;
            JsonIterator(const Json::Array::iterator  &iter, Json& json);
            JsonIterator(const Json::Object::iterator &iter, Json& json);

            public:
            JsonIterator(const JsonIterator& iterator);
            std::string key() const;
            size_t index() const;
            Json value() const;
            Json& operator *();
            const Json& operator *() const;
            JsonIterator& operator ++();
            JsonIterator operator ++(int i);
            bool operator == (const JsonIterator &iter) const;
            bool operator != (const JsonIterator &iter) const;
        };
        class JsonConstIterator{
            friend class ::dfe::Json;

            const Json& _json;
            union{
                Json::Array::const_iterator  _arrayIter;
                Json::Object::const_iterator _objectIter;
            };
            Json::ValueType _valueType;

            JsonConstIterator()=delete;
            JsonConstIterator(const Json::Array::const_iterator  &iter, const Json& json);
            JsonConstIterator(const Json::Object::const_iterator &iter, const Json& json);

            public:
            JsonConstIterator(const JsonConstIterator& iterator);
            std::string key() const;
            size_t index() const;
            Json value() const;
            const Json& operator *() const;
            JsonConstIterator& operator ++();
            JsonConstIterator operator ++(int i);
            bool operator == (const JsonConstIterator &iter) const;
            bool operator != (const JsonConstIterator &iter) const;
        };
    }
    namespace exception{
        class JsonException : public std::runtime_error {
            public:
            enum class ErrorCode{ OtherError, TypeError, BadCast, OutOfRange, KeyError, DecodeError } errorCode;
            JsonException(ErrorCode code,const char* message):runtime_error(message),errorCode(code){}
            static JsonException otherError (const char* message=""             ){return JsonException(ErrorCode::OtherError ,message);}
            static JsonException typeError  (const char* message="type error"   ){return JsonException(ErrorCode::TypeError  ,message);}
            static JsonException badCast    (const char* message="bad cast"     ){return JsonException(ErrorCode::BadCast    ,message);}
            static JsonException outOfRange (const char* message="out of range" ){return JsonException(ErrorCode::OutOfRange ,message);}
            static JsonException keyError   (const char* message="key error"    ){return JsonException(ErrorCode::KeyError   ,message);}
            static JsonException decodeError(const char* message="decode error" ){return JsonException(ErrorCode::DecodeError,message);}
        };
    }


    INLINE Json::Json() noexcept:_valueType(ValueType::Null){}
    INLINE Json::Json(const Null     value) noexcept:_null(nullptr),_valueType(ValueType::Null  ){}
    INLINE Json::Json(const char*    value) noexcept:_string(new String(value)),_valueType(ValueType::String){}
    INLINE Json::Json(const String  &value) noexcept:_string(new String(value)),_valueType(ValueType::String){}
    INLINE Json::Json(const Array   &value) noexcept:_array (new Array (value)),_valueType(ValueType::Array ){}
    INLINE Json::Json(const Object  &value) noexcept:_object(new Object(value)),_valueType(ValueType::Object){}

    template<typename NumberT,typename std::enable_if<
        std::is_integral<NumberT>::value||std::is_floating_point<NumberT>::value,std::nullptr_t
    >::type>
    INLINE Json::Json(const NumberT &value) noexcept{
        if(std::is_same<NumberT,bool>::value){
            _bool=value;
            _valueType=ValueType::Bool;
        }else{
            _number=value;
            _valueType=ValueType::Number;
        }
    }
    NOINLINE Json::Json(const Json &value) noexcept{
        _valueType=value._valueType;
        switch(value._valueType){
            case ValueType::Null    : _null   =nullptr;       break;
            case ValueType::Bool    : _bool   =value._bool  ; break;
            case ValueType::Number  : _number =value._number; break;
            case ValueType::String  : _string=new String(*(value._string)); break;
            case ValueType::Array   : _array =new Array (*(value._array )); break;
            case ValueType::Object  : _object=new Object(*(value._object)); break;
        }
    }
    NOINLINE Json::Json(Json &&value) noexcept{
        _valueType=value._valueType;
        switch(value._valueType){
            case ValueType::Null    : _null=nullptr;break;
            case ValueType::Bool    : _bool  =std::move(value._bool  );break;
            case ValueType::Number  : _number=std::move(value._number);break;
            case ValueType::String  : _string=value._string;value._string=nullptr;break;
            case ValueType::Array   : _array =value._array ;value._array =nullptr;break;
            case ValueType::Object  : _object=value._object;value._object=nullptr;break;
        }
    }
    NOINLINE Json::Json(const std::initializer_list<Json> &list) noexcept{
        _valueType=ValueType::Array;
        _array=new Array();
        for(std::initializer_list<Json>::iterator it=list.begin();it!=list.end();++it){
            _array->push_back(*it);
        }
    }
    INLINE Json::Json(const ValueType type) noexcept{
        _valueType=type;
        switch(type){
            case ValueType::Null   : _null  =Null  ();break;
            case ValueType::Bool   : _bool  =Bool  ();break;
            case ValueType::Number : _number=Number();break;
            case ValueType::String : _string=new String();break;
            case ValueType::Array  : _array =new Array ();break;
            case ValueType::Object : _object=new Object();break;
            default: break;
        }
    }

    INLINE Json::~Json() noexcept{
        clear();
    }

    NOINLINE Json::Null Json::getNull() const{
        if(isNull()) return _null;
        else throw Exception::typeError(("type must be Null, but "+toString(_valueType)).c_str());
    }
    NOINLINE Json::Bool Json::getBool() const{
        if(isBool()) return _bool;
        else throw Exception::typeError(("type must be Bool, but "+toString(_valueType)).c_str());
    }
    NOINLINE Json::Number Json::getNumber() const{
        if(isNumber()) return _number;
        else throw Exception::typeError(("type must be Number, but "+toString(_valueType)).c_str());
    }
    template<typename NumberT>
    NOINLINE NumberT Json::getNumber() const{
        static_assert(std::is_floating_point<NumberT>::value || std::is_integral<NumberT>::value,"NumberT must be numeric, but is not.");
        if(isNumber()) return _number.get<NumberT>();
        else throw Exception::typeError(("type must be Number, but "+toString(_valueType)).c_str());
    }
    NOINLINE Json::String Json::getString() const{
        if(isString()) return *_string;
        else throw Exception::typeError(("type must be String, but "+toString(_valueType)).c_str());
    }
    NOINLINE Json::Array Json::getArray() const{
        if(isArray()) return *_array;
        else throw Exception::typeError(("type must be Array, but "+toString(_valueType)).c_str());
    }
    NOINLINE Json::Object Json::getObject() const{
        if(isObject()) return *_object;
        else throw Exception::typeError(("type must be Object, but "+toString(_valueType)).c_str());
    }

    INLINE Json::ValueType Json::valueType() const noexcept{
        return _valueType;
    }

    INLINE bool Json::isNull()   const noexcept{ return _valueType==ValueType::Null  ;}
    INLINE bool Json::isBool()   const noexcept{ return _valueType==ValueType::Bool  ;}
    INLINE bool Json::isNumber() const noexcept{ return _valueType==ValueType::Number;}
    INLINE bool Json::isString() const noexcept{ return _valueType==ValueType::String;}
    INLINE bool Json::isArray()  const noexcept{ return _valueType==ValueType::Array ;}
    INLINE bool Json::isObject() const noexcept{ return _valueType==ValueType::Object;}

    NOINLINE void Json::clear() noexcept{
        switch(_valueType){
            case ValueType::Null   : break;
            case ValueType::Bool   : break;
            case ValueType::Number : break;
            case ValueType::String : delete _string;break;
            case ValueType::Array  : delete _array ;break;
            case ValueType::Object : delete _object;break;
            default: break;
        }
        _valueType=ValueType::Null;
    }
    NOINLINE void Json::reset(const Json::ValueType type) noexcept{
        clear();
        _valueType=type;
        switch(type){
            case ValueType::Null   : _null  =Null  ();break;
            case ValueType::Bool   : _bool  =Bool  ();break;
            case ValueType::Number : _number=Number();break;
            case ValueType::String : _string=new String();break;
            case ValueType::Array  : _array =new Array ();break;
            case ValueType::Object : _object=new Object();break;
            default: break;
        }
    }

    NOINLINE Json Json::load(const std::string& path){
        Json res;
        std::string jsonStr;
        std::ifstream ifs(path,std::ios::in);
        if(!ifs.is_open()){
            throw Exception::otherError(("failed to open the file : "+path).c_str());
        }
        jsonStr=std::string(std::istreambuf_iterator<char>(ifs),std::istreambuf_iterator<char>());
        detail::JsonDecoder::decode(jsonStr.begin(),jsonStr.end(),res);
        ifs.close();
        return res;
    }
    NOINLINE void Json::save(const Json &json,const std::string& path,const bool indent){
        std::ofstream ofs(path);
        if(!ofs.is_open()){
            throw Exception::otherError(("failed to open the file : "+path).c_str());
            return;
        }
        ofs << toString(json,indent);
        ofs.close();
    }
    NOINLINE Json Json::fromString(const std::string& jsonStr){
        Json res;
        detail::JsonDecoder::decode(jsonStr.begin(),jsonStr.end(),res);
        return res;
    }

    #define INDENT for(unsigned int i=0; i<indentSize; ++i){oss<<"    ";}
    NOINLINE std::string Json::Json::toString(const Json &value,const bool indent) noexcept{
        static unsigned int indentSize=0;
        std::ostringstream oss;
        switch(value._valueType){
            case Json::ValueType::Null    : oss << "null"; break;
            case Json::ValueType::Bool    : oss << (value._bool?"true":"false"); break;
            case Json::ValueType::Number  : oss << value._number; break;
            case Json::ValueType::String  : oss << "\"" << detail::JsonDecoder::escapeStr(*(value._string)) << "\""; break;
            case Json::ValueType::Array   :
            {
                bool d=false;
                oss << "[";
                ++indentSize;
                for(const Json &val : *(value._array)){
                    if(d){
                        oss << ",";
                    }else d=true;
                    if(indent){oss<<"\n";INDENT}
                    oss << toString(val,indent);
                }
                indentSize--;
                if(indent){oss<<"\n";INDENT}
                oss << "]";
                break;
            }
            case Json::ValueType::Object     :
            {
                bool d=false;
                oss << "{";
                ++indentSize;
                for(const auto &item : *(value._object)){
                    if(d){
                        oss << ",";
                    }else d=true;
                    if(indent){oss<<"\n";INDENT}
                    oss << "\"" << item.first << "\":" << toString(item.second,indent);
                }
                indentSize--;
                if(indent){oss<<"\n";INDENT}
                oss << "}";
                break;
            }
        }
        return oss.str();
    }
    #undef INDENT
    NOINLINE std::string Json::toString(const ValueType type) noexcept{
        switch(type){
            case ValueType::Null   : return "Null"  ; break;
            case ValueType::Bool   : return "Bool"  ; break;
            case ValueType::Number : return "Number"; break;
            case ValueType::String : return "String"; break;
            case ValueType::Array  : return "Array" ; break;
            case ValueType::Object : return "Object"; break;
        }
        return "";
    }
    NOINLINE Json& Json::operator= (const Json &value){
        if(this->_valueType==value._valueType){
            switch(value._valueType){
                case ValueType::Null    : _null  =nullptr;          break;
                case ValueType::Bool    : _bool  =value._bool  ;    break;
                case ValueType::Number  : _number=value._number;    break;
                case ValueType::String  :*_string=*(value._string); break;
                case ValueType::Array   :*_array =*(value._array ); break;
                case ValueType::Object  :*_object=*(value._object); break;
            }
        }else{
            this->reset(value._valueType);
            switch(value._valueType){
                case ValueType::Null    : _null  =nullptr;       break;
                case ValueType::Bool    : _bool  =value._bool  ; break;
                case ValueType::Number  : _number=value._number; break;
                case ValueType::String  : _string=new String(*(value._string)); break;
                case ValueType::Array   : _array =new Array (*(value._array )); break;
                case ValueType::Object  : _object=new Object(*(value._object)); break;
            }
        }
        return *this;
    }
    NOINLINE Json& Json::operator= (Json &&value){
        if(this->_valueType!=value._valueType){
            this->reset(value._valueType);
        }
        switch(value._valueType){
            case ValueType::Null    : _null=nullptr        ;    break;
            case ValueType::Bool    : _bool  =value._bool  ;    break;
            case ValueType::Number  : _number=value._number;    break;
            case ValueType::String  : _string=value._string;value._string=nullptr;  break;
            case ValueType::Array   : _array =value._array ;value._array =nullptr;  break;
            case ValueType::Object  : _object=value._object;value._object=nullptr;  break;
        }
        return *this;
    }
    NOINLINE bool Json::operator==(const Json& other) const{
        if(_valueType!=other._valueType){
            return false;
        }else{
            switch(_valueType){
                case ValueType::Null    : return true;  break;
                case ValueType::Bool    : return  _bool  == other._bool  ; break;
                case ValueType::Number  : return  _number== other._number; break;
                case ValueType::String  : return *_string==*other._string; break;
                case ValueType::Array   : return *_array ==*other._array ; break;
                case ValueType::Object  : return *_object==*other._object; break;
            }
        }
        return false;
    }
    INLINE bool Json::operator!=(const Json& other) const{
        return !(operator==(other));
    }

    NOINLINE Json::Iterator Json::begin(){
        if(isArray()){
            return Iterator(_array->begin(),*this);
        }else if(isObject()){
            return Iterator(_object->begin(),*this);
        }else{
            throw Exception::typeError("function begin() is only for Array/Object-type value");
        }
    }
    NOINLINE Json::ConstIterator Json::begin() const{
        if(isArray()){
            return ConstIterator(_array->cbegin(),*this);
        }else if(isObject()){
            return ConstIterator(_object->cbegin(),*this);
        }else{
            throw Exception::typeError("function begin() is only for Array/Object-type value");
        }
    }
    NOINLINE Json::ConstIterator Json::cbegin() const{
        if(isArray()){
            return ConstIterator(_array->cbegin(),*this);
        }else if(isObject()){
            return ConstIterator(_object->cbegin(),*this);
        }else{
            throw Exception::typeError("function cbegin() is only for Array/Object-type value");
        }
    }
    NOINLINE Json::Iterator Json::end(){
        if(isArray()){
            return Iterator(_array->end(),*this);
        }else if(isObject()){
            return Iterator(_object->end(),*this);
        }else{
            throw Exception::typeError("function end() is only for Array/Object-type value");
        }
    }
    NOINLINE Json::ConstIterator Json::end() const{
        if(isArray()){
            return ConstIterator(_array->cend(),*this);
        }else if(isObject()){
            return ConstIterator(_object->cend(),*this);
        }else{
            throw Exception::typeError("function end() is only for Array/Object-type value");
        }
    }
    NOINLINE Json::ConstIterator Json::cend() const{
        if(isArray()){
            return ConstIterator(_array->cend(),*this);
        }else if(isObject()){
            return ConstIterator(_object->cend(),*this);
        }else{
            throw Exception::typeError("function cend() is only for Array/Object-type value");
        }
    }
    NOINLINE size_t Json::size() const{
        if(isArray()){
            return _array->size();
        }else if(isObject()){
            return _object->size();
        }else throw Exception::typeError("function size() is only for Array/Object-type value");
    }
    NOINLINE Json::Iterator Json::find(const Json& value){
        if(isArray()){
            return Iterator(std::find(_array->begin(),_array->end(),value),*this);
        }else throw Exception::typeError("function find(value) is only for Array-type value");
    }
    NOINLINE Json::ConstIterator Json::find(const Json& value) const{
        if(isArray()){
            return ConstIterator(std::find(_array->cbegin(),_array->cend(),value),*this);
        }else throw Exception::typeError("function find(value) is only for Array-type value");
    }
    NOINLINE Json::Iterator Json::find(const std::string &key){
        if(isObject()){
            return Iterator(_object->find(key),*this);
        }else throw Exception::typeError("function find(key) is only for Object-type value");
    }
    NOINLINE Json::ConstIterator Json::find(const std::string &key) const{
        if(isObject()){
            return ConstIterator(_object->find(key),*this);
        }else throw Exception::typeError("function find(key) is only for Object-type value");
    }

    NOINLINE void Json::append(const Json& value){
        if(this->isNull()){
            this->reset(ValueType::Array);
        }
        if(isArray()){
            _array->push_back(value);
        }else{
            throw Exception::typeError("function append(value) is only for Array-type value");
        }
    }
    NOINLINE void Json::insert(const Json::Array::size_type index, const Json& value){
        if(isArray()){
            _array->insert(_array->begin()+index,value);
        }else{
            throw Exception::typeError("function insert(index,value) is only for Array-type value");
        }
    }
    NOINLINE Json& Json::at(const Array::size_type index){
        if(this->isArray()){
            if( index<_array->size() ) return _array->operator[](index);
            else throw Exception::outOfRange();
        }else{
            throw Exception::typeError("operator[index] is only for Array-type value");
        }
    }
    NOINLINE const Json& Json::at(const Array::size_type index) const{
        if(this->isArray()){
            if( index < _array->size() ) return _array->operator[](index);
            else throw Exception::outOfRange();
        }else{
            throw Exception::typeError("operator[index] is only for Array-type value");
        }
    }
    NOINLINE Json& Json::operator [](const Array::size_type index){
        if(this->isNull()){
            this->reset(ValueType::Array);
        }
        if(this->isArray()){
            if( index < _array->size() ) return _array->operator[](index);
            else throw Exception::outOfRange();
        }else{
            throw Exception::typeError("operator[index] is only for Array-type value");
        }
    }
    NOINLINE const Json& Json::operator [](const Array::size_type index) const{
        if(this->isArray()){
            if( index < _array->size() ) return _array->operator[](index);
            else throw Exception::outOfRange();
        }else{
            throw Exception::typeError("operator[index] is only for Array-type value");
        }
    }

    NOINLINE void Json::append(const std::string &key,const Json &value){
        if(this->isNull()){
            this->reset(ValueType::Object);
        }
        if(!isObject()){
            throw Exception::typeError("function append(key,value) is only for Object-type value");
        }else{
            _object->operator[]("key")=value;
        }
    }
    NOINLINE void Json::insert(const std::string &key,const Json &value){
        if(!isObject()){
            throw Exception::typeError("function insert(key,value) is only for Object-type value");
        }else{
            _object->operator[]("key")=value;
        }
    }
    NOINLINE bool Json::contains(const std::string &key) const{
        if(_valueType!=ValueType::Object) throw Exception::typeError("function contains(key) is only for Object-type value");
        return _object->count(key)>0;
    }
    NOINLINE std::vector<std::string> Json::keys() const{
        if(_valueType!=ValueType::Object) throw Exception::typeError("function keys() is only for Object-type value");
        std::vector<std::string> k;
        for(auto &item : *_object){
            k.push_back(item.first);
        }
        return k;
    }
    NOINLINE void Json::erase(const std::string &key){
        if(_valueType!=ValueType::Object) throw Exception::typeError("function erase(key) is only for Object-type value");
        _object->erase(key);
    }
    NOINLINE Json& Json::at(const std::string &key){
        if(this->isObject()){
            return _object->at(key);
        }else{
            throw Exception::typeError("operator[](key) is only for Object-type value");
        }
    }
    NOINLINE const Json& Json::at(const std::string &key) const{
        if(this->isObject()){
            return _object->at(key);
        }else{
            throw Exception::typeError("operator[](key) is only for Object-type value");
        }
    }
    NOINLINE Json& Json::operator [] (const std::string &key){
        if(this->isNull()){
            this->reset(ValueType::Object);
        }
        if(this->isObject()){
            return _object->operator[](key);
        }else{
            throw Exception::typeError("operator[](key) is only for Object-type value");
        }
    }
    NOINLINE const Json& Json::operator [] (const std::string &key) const{
        if(this->isObject()){
            return _object->at(key);
        }else{
            throw Exception::typeError("operator[](key) is only for Object-type value");
        }
    }

    INLINE std::ostream& operator<<(std::ostream &os,const dfe::Json &value) noexcept{
        os << dfe::Json::toString(value);
        return os;
    }
    NOINLINE std::ostream& operator<<(std::ostream &os,const dfe::Json::ValueType &valueType) noexcept{
        switch(valueType){
            case Json::ValueType::Null:   os << "Json::Null"  ; break;
            case Json::ValueType::Number: os << "Json::Number"; break;
            case Json::ValueType::String: os << "Json::String"; break;
            case Json::ValueType::Bool:   os << "Json::Bool"  ; break;
            case Json::ValueType::Array:  os << "Json::Array" ; break;
            case Json::ValueType::Object: os << "Json::Object"; break;
        }
        return os;
    }
    
    namespace detail{
        
        /********************************/
        /*      JsonDecoder                 */
        /********************************/
        #define THROW throw Json::Exception::decodeError()
        NOINLINE bool JsonDecoder::decode(const StrIter &start,const StrIter &end,Json& out){
            out.clear();
            auto r=getParsedValue(start,end,out);
            if(r.first){
                StrIter p=r.second;
                if(*p=='\0'||isValidEmptyString(p,end)){
                    return true;
                }else{
                    out.clear();
                    THROW;
                    return false;
                }
            }else{
                THROW;
                return false;
            }
        }
        #undef THROW
        NOINLINE
        std::pair<bool,JsonDecoder::StrIter> JsonDecoder::getParsedValue(const StrIter &start,const StrIter &end,::dfe::Json &out) noexcept{
            ValueType type;
            StrIter p;
            auto r=detectValueType(start,end);
            p=std::get<1>(r);
            type=std::get<2>(r);
            if(std::get<0>(r)==true){
                if(p==end){
                    out._valueType=ValueType::Null;
                    return {true,p};
                }
            }else{
                return {false,p};
            }
            switch(type){
                case ValueType::Null:
                {
                    auto r=tryParseToNull(p,end);
                    if(std::get<0>(r)){
                        p=std::get<1>(r);
                        out._valueType=ValueType::Null;
                        return {true,p};
                    }
                } break;
                case ValueType::Bool:
                {
                    auto r=tryParseToBool(p,end);
                    if(std::get<0>(r)){
                        p=std::get<1>(r);
                        out._bool=std::get<2>(r);
                        out._valueType=ValueType::Bool;
                        return {true,p};
                    }
                } break;
                case ValueType::Number:
                {
                    auto r=tryParseToNumber(p,end);
                    if(std::get<0>(r)){
                        p=std::get<1>(r);
                        out._number=std::get<2>(r);
                        out._valueType=ValueType::Number;
                        return {true,p};
                    }
                } break;
                case ValueType::String:
                {
                    auto r=tryParseToString(p,end);
                    if(std::get<0>(r)){
                        p=std::get<1>(r);
                        out._string=new Json::String(std::get<2>(r));
                        out._valueType=ValueType::String;
                        return {true,p};
                    }
                } break;
                case ValueType::Array:
                {
                    auto r=tryParseToArray(p,end);
                    if(std::get<0>(r)){
                        p=std::get<1>(r);
                        out._array=new Json::Array(std::get<2>(r));
                        out._valueType=ValueType::Array;
                        return {true,p};
                    }
                } break;
                case ValueType::Object:
                {
                    auto r=tryParseToObject(p,end);
                    if(std::get<0>(r)){
                        p=std::get<1>(r);
                        out._object=new Json::Object(std::get<2>(r));
                        out._valueType=ValueType::Object;
                        return {true,p};
                    }
                } break;
            }
            return {false,end};
        }
        
        NOINLINE
        std::tuple<bool,JsonDecoder::StrIter,JsonDecoder::ValueType>
        JsonDecoder::detectValueType(const StrIter &start,const StrIter &end) noexcept{
            StrIter p;
            for(p=start;p<end;++p){
                if((*p>='0' && *p<='9')||*p=='-'){
                    return {true,p,ValueType::Number};
                }else if(*p=='"'){
                    return {true,p,ValueType::String};
                }else if(*p=='['){
                    return {true,p,ValueType::Array};
                }else if(*p=='{'){
                    return {true,p,ValueType::Object};
                }else if(*p=='t' || *p=='f'){
                    return {true,p,ValueType::Bool};
                }else if(*p=='n'){
                    return {true,p,ValueType::Null};
                }else if(isValidEmptyChar(*p)){
                    //pass
                }else{
                    return {false,p,ValueType::Null};
                }
            }
            return {true,end,ValueType::Null};
        }
        NOINLINE
        std::tuple<bool,JsonDecoder::StrIter,Json::Null>
        JsonDecoder::tryParseToNull(const StrIter &start,const StrIter &end) noexcept{
            StrIter p=start;
            if(p+4>end || *p!='n'){
                return {false,end,nullptr};
            }else if(std::string(p,p+4)=="null"){
                return {true,p+4,nullptr};
            }else{
                return {false,end,nullptr};
            }
        }
        #ifdef FAILED
        #   undef FAILED
        #endif
        #define FAILED return {false,end,false};
        NOINLINE
        std::tuple<bool,JsonDecoder::StrIter,Json::Bool>
        JsonDecoder::tryParseToBool(const StrIter &start,const StrIter &end) noexcept{
            size_t l;
            std::string b;
            Json::Bool res=false;
            if(*start=='t'){
                l=4;
                b="true";
                res=true;
            }else if(*start=='f'){
                l=5;
                b="false";
                res=false;
            }else{
                FAILED
            }
            if(start+l>end){
                FAILED
            }else if(std::string(start,start+l)==b){
                return {true,start+l,res};
            }else{
                FAILED
            }
        }
        #undef FAILED
        #define FAILED return {false,end,0};
        NOINLINE
        std::tuple<bool,JsonDecoder::StrIter,Json::Number>
        JsonDecoder::tryParseToNumber(const StrIter &start,const StrIter &end) noexcept{
            StrIter p=start;
            Json::Number res=0;
            bool expFlag=false;
            bool dicimFlag=false;
            bool minusFlag=false;
            bool expMinusFlag=false;
            bool floatFlag=false;
            Json::Number::Int integ=0;
            Json::Number::Float dicim=0;
            Json::Number::Float dicimDivNum=1;
            Json::Number::Int exp=0;

            if(p>=end){
                FAILED
            }else if((*p>='0' && *p<='9')||*p=='-'){
                if(*p=='-'){
                    minusFlag=true;
                }else{
                    integ=*p-'0';
                }
            }else{
                FAILED
            }
            while(true){
                ++p;
                if(p>=end){
                    break;
                }else if(*p=='e' || *p=='E'){
                    if(expFlag){
                        FAILED
                    }else{
                        floatFlag=true;
                        expFlag=true;
                    }
                }else if(*p=='.'){
                    if(expFlag || dicimFlag){
                        FAILED
                    }else{
                        floatFlag=true;
                        dicimFlag=true;
                    }
                }else if(*p=='-' || *p=='+'){
                    if(*(p-1)=='e' || *(p-1)=='E'){
                        if(*p=='-') expMinusFlag=true;
                    }else{
                        FAILED
                    }
                }else if(*p>='0' && *p<='9'){
                    int n=*p-'0';
                    if(expFlag){
                        exp=exp*10+n;
                    }else if(dicimFlag){
                        dicim=dicim*10+n;
                        dicimDivNum*=10;
                    }else{
                        integ=integ*10+n;
                    }
                }else{
                    break;
                }
            }
            if(expMinusFlag) exp *= -1;
            if(floatFlag){
                Json::Number::Float tmp=(integ+(dicim/dicimDivNum))*std::pow(10,exp);
                if(minusFlag) tmp*=-1;
                res=tmp;
            }else{
                Json::Number::Int tmp=integ;
                if(minusFlag) tmp*=-1;
                res=tmp;
            }
            return {true,p,res};
        }
        #undef FAILED
        #define FAILED return {false,end,""};
        NOINLINE
        std::tuple<bool,JsonDecoder::StrIter,Json::String>
        JsonDecoder::tryParseToString(const StrIter &start,const StrIter &end) noexcept{
            StrIter p=start;
            Json::String res="";
            bool escape=false;
            bool flag=false;
            if(p>=end || *p!='"'){
                FAILED
            }
            ++p;
            for(/**/;p<end;++p){
                if(escape){
                    bool flag=false;
                    switch(*p){
                        case 'u' : 
                        {
                            if(end-p<5){
                                flag=true;
                            }else{
                                uint16_t c=0x0000;
                                for(size_t i=1;i<5; ++i){
                                    char ch=*(p+i);
                                    if(i>0) c<<=4;
                                    if(ch>='0' && ch<='9'){
                                        c+=(ch-'0');
                                    }else if(ch>='a' && ch<='f'){
                                        c+=(ch-'a'+10);
                                    }else if(ch>='A' && ch<='F'){
                                        c+=(ch-'A'+10);
                                    }else{
                                        flag=true;
                                        break;
                                    }
                                }
                                res+=unicodeToUtf8(c);
                                p+=4;
                            }
                            escape=false;
                        } break;
                        case '\\':res+='\\';escape=false;break;
                        case '/' :res+='/' ;escape=false;break;
                        case '"' :res+='\\';escape=false;break;
                        case 'b' :res+='\b';escape=false;break;
                        case 'f' :res+='\f';escape=false;break;
                        case 'n' :res+='\n';escape=false;break;
                        case 'r' :res+='\r';escape=false;break;
                        case 't' :res+='\t';escape=false;break;
                        default  :flag=true;break;
                    }
                    if(flag){
                        FAILED
                    }
                }else{
                    if(*p=='"'){
                        flag=true;
                        break;
                    }else if(*p=='\\'){
                        escape=true;
                    }else{
                        res+=(*p);
                    }
                }
            }
            if(flag){
                ++p;
                return {true,p,res};
            }else{
                FAILED
            }
        }
        #undef FAILED
        #define FAILED return {false,end,{}};
        NOINLINE
        std::tuple<bool,JsonDecoder::StrIter,Json::Array>
        JsonDecoder::tryParseToArray(const StrIter &start,const StrIter &end) noexcept{
            bool endFlag=false;
            Json::Array res;
            if(start>=end || *start!='['){
                FAILED
            }
            StrIter p=start;
            while(1){
                ++p;
                Json val;
                // get value
                auto r=getParsedValue(p,end,val);
                p=r.second;
                if(r.first){
                    res.push_back(val);
                }else if(*p==']' && res.empty()){
                    endFlag=true;
                    break;
                }else{
                    FAILED
                }
                // find ',' to next value or '}' to end decode
                for(/**/;p<end;++p){
                    if(*p==','){
                        break;
                    }else if(*p==']'){
                        endFlag=true;
                        break;
                    }else if(!isValidEmptyChar(*p)){
                        FAILED
                    }
                }
                if(endFlag || p>=end){
                    break;
                }
            }
            if(endFlag){
                ++p;
                return {true,p,res};
            }else{
                FAILED
            }
        }
        #undef FAILED
        #define FAILED return {false,end,{}};
        NOINLINE
        std::tuple<bool,JsonDecoder::StrIter,Json::Object>
        JsonDecoder::tryParseToObject(const StrIter &start,const StrIter &end) noexcept{
            bool endFlag=false;
            Json::Object res;
            if(start>=end || *start!='{'){
                FAILED
            }
            StrIter p=start;
            while(1){
                ++p;
                std::string key;
                Json val;
                // detect key string
                for(/**/;p<end;++p){
                    if(*p=='"'){
                        break;
                    }else if(*p=='}' && res.empty()){
                        break;
                    }else if(!isValidEmptyChar(*p)){
                        FAILED
                    }
                }
                if(p>=end){
                    FAILED
                }else if(*p=='}' && res.empty()){
                    endFlag=true;
                    break;
                }else{
                    auto r=tryParseToString(p,end);
                    if(std::get<0>(r)){
                        p=std::get<1>(r);
                        key=std::get<2>(r);
                    }else{
                        FAILED
                    }
                    if(res.count(key)>0){
                        FAILED
                    }
                }
                // find ':' and get value
                for(/**/;p<end;++p){
                    if(*p==':'){
                        ++p;
                        break;
                    }else if(!isValidEmptyChar(*p)){
                        FAILED
                    }
                }
                if(p>=end){
                    FAILED
                }else{
                    auto r=getParsedValue(p,end,val);
                    if(r.first){
                        p=r.second;
                        res[key]=val;
                    }else{
                        FAILED
                    }
                }
                // find ',' to next pair or '}' to end decoding
                for(/**/;p<end;++p){
                    if(*p==','){
                        break;
                    }else if(*p=='}'){
                        endFlag=true;
                        break;
                    }else if(!isValidEmptyChar(*p)){
                        FAILED
                    }else{
                        //pass
                    }
                }
                if(endFlag || p>=end){
                    break;
                }
            }
            if(endFlag){
                ++p;
                return {true,p,res};
            }else{
                FAILED
            }
        }
        #undef FAILED
        INLINE bool JsonDecoder::isValidEmptyChar(const char ch) noexcept{
            return (ch==' '||ch=='\n'||ch=='\r'||ch=='\t'||ch=='\b'||ch=='\f'||ch=='\0');
        }
        INLINE bool JsonDecoder::isValidEmptyString(const StrIter& start,const StrIter& end) noexcept{
            return start>=end?false:std::all_of(start,end,isValidEmptyChar);
        }
        
        NOINLINE std::string JsonDecoder::escapeStr(const std::string &str) noexcept{
            std::string res;
            for(char ch : str){
                switch(ch){
                    case '\\':res+="\\\\";break;
                    case '\b':res+="\\b" ;break;
                    case '\f':res+="\\f" ;break;
                    case '\n':res+="\\n" ;break;
                    case '\r':res+="\\r" ;break;
                    case '\t':res+="\\t" ;break;
                    default  :res+=ch    ;break;
                }
            }
            return res;
        }
        NOINLINE std::string JsonDecoder::unicodeToUtf8(const uint16_t b) noexcept{
            std::string res;
            if( b < 0x0080){
                res+= char( b & 0b01111111 );
            }else if( b < 0x0800 ){
                res+= char((0b1100|((b>>10)&0b0001))<<4 | ( b>>6 & 0b1111 ));
                res+= char((0b1000|((b>>4 )&0b0011))<<4 | ( b    & 0b1111 ));
            }else{
                res+= char((0b1110)<<4 | ( b>>12 & 0b1111 ));
                res+= char((0b1000|((b>>10)&0b0011))<<4 | ( b>>6 & 0b1111 ));
                res+= char((0b1000|((b>>4 )&0b0011))<<4 | ( b    & 0b1111 ));
            }
            return res;
        }

        /********************************/
        /*      JsonIterator            */
        /********************************/
        #define THROW throw Json::Exception::typeError("iterator is only for Array/Object-type value")
        INLINE JsonIterator::JsonIterator(const Json::Array::iterator  &iter, Json& json)
        :   _json(json),_arrayIter(iter),_valueType(Json::ValueType::Array )
        {}
        INLINE JsonIterator::JsonIterator(const Json::Object::iterator &iter, Json& json)
        :   _json(json),_objectIter(iter),_valueType(Json::ValueType::Object)
        {}
        NOINLINE JsonIterator::JsonIterator(const JsonIterator& iterator)
        :   _json(iterator._json),_valueType(iterator._valueType)
        {
            if(_valueType==Json::ValueType::Array){
                this->_arrayIter=iterator._arrayIter;
            }else if(_valueType==Json::ValueType::Object){
                this->_objectIter=iterator._objectIter;
            }
        }
        NOINLINE size_t JsonIterator::index() const{
            if(_valueType==Json::ValueType::Array){
                return _arrayIter-(_json.begin()._arrayIter);
            }else THROW;
        }
        NOINLINE std::string JsonIterator::key() const{
            if(_valueType==Json::ValueType::Object){
                return _objectIter->first;
            }else THROW;
        }
        NOINLINE Json JsonIterator::value() const{
            if(_valueType==Json::ValueType::Array){
                return *_arrayIter;
            }else if(_valueType==Json::ValueType::Object){
                return _objectIter->second;
            }else THROW;
        }
        NOINLINE Json& JsonIterator::operator*(){
            if(_valueType==Json::ValueType::Array){
                return *_arrayIter;
            }else if(_valueType==Json::ValueType::Object){
                return _json[_objectIter->first];
            }else THROW;
        }
        NOINLINE const Json& JsonIterator::operator*() const{
            if(_valueType==Json::ValueType::Array){
                return *_arrayIter;
            }else if(_valueType==Json::ValueType::Object){
                return _json[_objectIter->first];
            }else THROW;
        }
        NOINLINE JsonIterator& JsonIterator::operator++(){
            if(_valueType==Json::ValueType::Array){
                ++_arrayIter;
                return *this;
            }else if(_valueType==Json::ValueType::Object){
                ++_objectIter;
                return *this;
            }else THROW;
        }
        NOINLINE JsonIterator JsonIterator::operator++(int i){
            if(_valueType==Json::ValueType::Array){
                Json::Array::iterator iter=_arrayIter;
                ++_arrayIter;
                return JsonIterator(iter,_json);
            }else if(_valueType==Json::ValueType::Object){
                Json::Object::iterator iter=_objectIter;
                ++_objectIter;
                return JsonIterator(iter,_json);
            }else THROW;
        }
        NOINLINE bool JsonIterator::operator==(const JsonIterator &iter) const{
            if(_valueType!=iter._valueType){
                THROW;
            }else if(_valueType==Json::ValueType::Array){
                return iter._arrayIter  == _arrayIter;
            }else if(_valueType==Json::ValueType::Object){
                return iter._objectIter == _objectIter;
            }else THROW;
        }
        INLINE bool JsonIterator::operator!=(const JsonIterator &iter) const{
            return !(*this==iter);
        }
        #undef THROW
        
        /********************************/
        /*      Const JsonIterator      */
        /********************************/
        #define THROW throw Json::Exception::typeError("ConstIterator is only for Array/Object-type value")
        INLINE JsonConstIterator::JsonConstIterator(const Json::Array::const_iterator  &iter,const Json& json)
        :   _json(json),_arrayIter(iter),_valueType(Json::ValueType::Array )
        {}
        INLINE JsonConstIterator::JsonConstIterator(const Json::Object::const_iterator &iter,const Json& json)
        :   _json(json),_objectIter(iter),_valueType(Json::ValueType::Object)
        {}
        NOINLINE JsonConstIterator::JsonConstIterator(const JsonConstIterator& iterator)
        :_json(iterator._json),_valueType(iterator._valueType)
        {
            if(_valueType==Json::ValueType::Array){
                this->_arrayIter=iterator._arrayIter;
            }else if(_valueType==Json::ValueType::Object){
                this->_objectIter=iterator._objectIter;
            }
        }
        NOINLINE size_t JsonConstIterator::index() const{
            if(_valueType==Json::ValueType::Array){
                return _arrayIter-(_json.begin()._arrayIter);
            }else throw Json::Exception::typeError("type must be Array");
        }
        NOINLINE std::string JsonConstIterator::key() const{
            if(_valueType==Json::ValueType::Object){
                return _objectIter->first;
            }else throw Json::Exception::typeError("type must be Object");
        }
        NOINLINE Json JsonConstIterator::value() const{
            if(_valueType==Json::ValueType::Array){
                return *_arrayIter;
            }else if(_valueType==Json::ValueType::Object){
                return _objectIter->second;
            }else THROW;
        }
        NOINLINE const Json& JsonConstIterator::operator*() const{
            if(_valueType==Json::ValueType::Array){
                return *_arrayIter;
            }else if(_valueType==Json::ValueType::Object){
                return _json[_objectIter->first];
            }else THROW;
        }
        NOINLINE JsonConstIterator& JsonConstIterator::operator++(){
            if(_valueType==Json::ValueType::Array){
                ++_arrayIter;
                return *this;
            }else if(_valueType==Json::ValueType::Object){
                ++_objectIter;
                return *this;
            }else THROW;
        }
        NOINLINE JsonConstIterator JsonConstIterator::operator++(int i){
            if(_valueType==Json::ValueType::Array){
                Json::Array::const_iterator iter=_arrayIter;
                ++_arrayIter;
                return JsonConstIterator(iter,_json);
            }else if(_valueType==Json::ValueType::Object){
                Json::Object::const_iterator iter=_objectIter;
                ++_objectIter;
                return JsonConstIterator(iter,_json);
            }else THROW;
        }
        NOINLINE bool JsonConstIterator::operator==(const JsonConstIterator &iter) const{
            if(_valueType!=iter._valueType){
                THROW;
            }else if(_valueType==Json::ValueType::Array){
                return iter._arrayIter  == _arrayIter;
            }else if(_valueType==Json::ValueType::Object){
                return iter._objectIter == _objectIter;
            }else THROW;
        }
        INLINE bool JsonConstIterator::operator!=(const JsonConstIterator &iter) const{
            return !(*this==iter);
        }
        #undef THROW

        INLINE JsonNumber::JsonNumber():_int(0),_isFloat(false){}
        template<typename T,typename JsonNumber::enable_if_float<T>::type>
        INLINE JsonNumber::JsonNumber(const T value):_float(static_cast<Float>(value)),_isFloat(true){}
        template<typename T,typename JsonNumber::enable_if_int<T>::type>
        INLINE JsonNumber::JsonNumber(const T value):_int(static_cast<Int>(value)),_isFloat(false){}

        template<typename T>
        INLINE T JsonNumber::get() const{
            static_assert(std::is_floating_point<T>::value||std::is_integral<T>::value,"T must be integral or floating point, but is not.");
            return _isFloat?static_cast<T>(_float):static_cast<T>(_int);
        }
        template<typename T,typename JsonNumber::enable_if_float<T>::type>
        INLINE void JsonNumber::set(const T value){
            _isFloat=true;
            _float=static_cast<Float>(value);
        }
        template<typename T,typename JsonNumber::enable_if_int<T>::type>
        INLINE void JsonNumber::set(const T value){
            _isFloat=false;
            _int=static_cast<Int>(value);
        }
        INLINE bool JsonNumber::isFloat() const{
            return _isFloat;
        }
        INLINE bool JsonNumber::isInt() const{
            return !_isFloat;
        }
        NOINLINE bool JsonNumber::operator==(const JsonNumber& other) const{
            if(_isFloat&&other._isFloat){
                return _float==other._float;
            }else if(_isFloat&&!other._isFloat){
                return _float==other._int;
            }else if((!_isFloat)&&other._isFloat){
                return _int==other._float;
            }else{
                return _int==other._int;
            }
        }
        INLINE bool JsonNumber::operator!=(const JsonNumber& other) const{
            return !this->operator==(other);
        }
        NOINLINE std::ostream& operator<<(std::ostream& os,const JsonNumber& value){
            if(value._isFloat){
                os << value._float;
            }else{
                os << value._int;
            }
            return os;
        }
    }
}

#endif