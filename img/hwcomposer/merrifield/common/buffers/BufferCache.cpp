/*
// Copyright (c) 2014 Intel Corporation 
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/
#include <HwcTrace.h>
#include <BufferCache.h>

namespace android {
namespace intel {

BufferCache::BufferCache(int size)
{
    mBufferPool.setCapacity(size);
}

BufferCache::~BufferCache()
{
    if (mBufferPool.size() != 0) {
        ETRACE("buffer cache is not empty");
    }
    mBufferPool.clear();
}

bool BufferCache::addMapper(uint64_t handle, BufferMapper* mapper)
{
    ssize_t index = mBufferPool.indexOfKey(handle);
    if (index >= 0) {
        ETRACE("buffer %#llx exists", handle);
        return false;
    }

    // add mapper
    index = mBufferPool.add(handle, mapper);
    if (index < 0) {
        ETRACE("failed to add mapper. err = %d", (int)index);
        return false;
    }

    return true;
}

bool BufferCache::removeMapper(BufferMapper* mapper)
{
    ssize_t index;

    if (!mapper) {
        ETRACE("invalid mapper");
        return false;
    }

    index = mBufferPool.removeItem(mapper->getKey());
    if (index < 0) {
        WTRACE("failed to remove mapper. err = %d", (int)index);
        return false;
    }

    return true;
}

BufferMapper* BufferCache::getMapper(uint64_t handle)
{
    ssize_t index = mBufferPool.indexOfKey(handle);
    if (index < 0) {
        // don't add ETRACE here as this condition will happen frequently
        return 0;
    }
    return mBufferPool.valueAt(index);
}

size_t BufferCache::getCacheSize() const
{
    return mBufferPool.size();
}

BufferMapper* BufferCache::getMapper(uint32_t index)
{
    if (index >= mBufferPool.size()) {
        ETRACE("invalid index");
        return 0;
    }
    BufferMapper* mapper = mBufferPool.valueAt(index);
    return mapper;
}

} // namespace intel
} // namespace android
