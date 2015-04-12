// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "media/ozone/media_ozone_platform.h"

namespace media {

namespace {

class MediaOzonePlatformQt : public MediaOzonePlatform {
 public:
  MediaOzonePlatformQt() {}
  ~MediaOzonePlatformQt() override {}

 private:
  DISALLOW_COPY_AND_ASSIGN(MediaOzonePlatformQt);
};

} // namespace

MediaOzonePlatform* CreateMediaOzonePlatformQt() {
  return new MediaOzonePlatformQt;
}

} // namespace media
