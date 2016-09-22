/***************************************************************************
   Copyright 2016 Ufora Inc.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
****************************************************************************/
#pragma once

#include <stdint.h>
#include <string>

#include "StringBuilder.hpp"


class BinaryObjectRegistry {
public:
    enum Code {
        CODE_NONE=1,
        CODE_INT=2,
        CODE_LONG=3,
        CODE_FLOAT=4,
        CODE_BOOL=5,
        CODE_STR=6,
        CODE_LIST_OF_PRIMITIVES=7,
        CODE_TUPLE=8,
        CODE_PACKED_HOMOGENOUS_DATA=9,
        CODE_LIST=10,
        CODE_FILE=11,
        CODE_DICT=12,
        CODE_REMOTE_PY_OBJECT=13,
        CODE_BUILTIN_EXCEPTION_INSTANCE=14,
        CODE_NAMED_SINGLETON=15,
        CODE_FUNCTION=16,
        CODE_CLASS=17,
        CODE_UNCONVERTIBLE=18,
        CODE_CLASS_INSTANCE=19,
        CODE_INSTANCE_METHOD=20,
        CODE_WITH_BLOCK=21,
        CODE_PY_ABORT_EXCEPTION=22,
        CODE_STACKTRACE_AS_JSON=23
        };        

public:
    BinaryObjectRegistry();

    uint64_t bytecount() const {
        return mStringBuilder.bytecount();
        }

    std::string str() const {
        return mStringBuilder.str();
        }

    void clear() {
        mStringBuilder.clear();
        }
        
    inline uint64_t allocateObject();

    template<class T>
    void definePrimitive(uint64_t objectId, const T& t) {
        mStringBuilder.addInt64(objectId);
        _writePrimitive(t);
        }

    inline void defineEndOfStream();

    inline void defineTuple(uint64_t objectId,
                            const std::vector<uint64_t>& memberIds);
    
    inline void defineTuple(uint64_t objectId,
                            const uint64_t* memberIds,
                            uint64_t nMemberIds);

    template<class T>
    void definePackedHomogeneousData(uint64_t objectId,
        const T& dtype,
        const std::string& dataAsBytes);

    template<class T>
    void definePackedHomogeneousData(uint64_t objectId,
        const T& dtype,
        const char* dataAsBytes,
        uint64_t bytes);


private:
    StringBuilder mStringBuilder;
    uint64_t mNextObjectId;

    inline void _writePrimitive(bool b);
    inline void _writePrimitive(int32_t i);
    inline void _writePrimitive(int64_t l);
    inline void _writePrimitive(double d);
    inline void _writePrimitive(const std::string& s);

    // DON'T KNOW HOW TO DEAL WITH PYTHON LONGS!

    template<typename T>
    void _writePrimitive(const std::vector<T>& primitives) {
        mStringBuilder.addByte(CODE_LIST_OF_PRIMITIVES);
        mStringBuilder.addInt64(primitives.size());
        for (typename std::vector<T>::const_iterator it = primitives.begin();
             it != primitives.end();
             ++it)
            _writePrimitive(*it);
        }

    };
