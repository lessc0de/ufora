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

#include "BinaryObjectRegistry.hpp"


BinaryObjectRegistry::BinaryObjectRegistry() : mNextObjectId(0) 
    {
    }
    

uint64_t BinaryObjectRegistry::allocateObject() {
    uint64_t objectId = mNextObjectId;
    mNextObjectId++;
    return objectId;
    }


void BinaryObjectRegistry::_writePrimitive(bool b) {
    mStringBuilder.addByte(CODE_BOOL);
    mStringBuilder.addByte(b ? 1 : 0);
    }


void BinaryObjectRegistry::_writePrimitive(int32_t i) {
    mStringBuilder.addByte(CODE_INT);
    mStringBuilder.addInt64(i);
    }


void BinaryObjectRegistry::_writePrimitive(int64_t i) {
    mStringBuilder.addByte(CODE_INT);
    mStringBuilder.addInt64(i);
    }


void BinaryObjectRegistry::_writePrimitive(double d) {
    mStringBuilder.addByte(CODE_FLOAT);
    mStringBuilder.addFloat64(d);
    }


void BinaryObjectRegistry::_writePrimitive(const std::string& s) {
    mStringBuilder.addByte(CODE_STR);
    mStringBuilder.addString(s);
    }


void BinaryObjectRegistry::defineEndOfStream() {
    mStringBuilder.addInt64(-1);
    }


void BinaryObjectRegistry::defineTuple(uint64_t objectId,
                                       const std::vector<uint64_t>& memberIds)
    {
    defineTuple(objectId,
                &memberIds[0],
                memberIds.size());
    }

    
void BinaryObjectRegistry::defineTuple(uint64_t objectId,
                                       const uint64_t* memberIds,
                                       uint64_t nMemberIds)
    {
    mStringBuilder.addInt64(objectId);
    mStringBuilder.addByte(CODE_TUPLE);
    mStringBuilder.addInt64s(memberIds, nMemberIds);
    }
