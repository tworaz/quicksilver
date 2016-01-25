// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_COMMON_QS_PROFILE_H_
#define QUICKSILVER_COMMON_QS_PROFILE_H_

namespace QuickSilverImpl {

enum ProfileType {
  DESKTOP,
  MOBILE,
  TABLET,
};

ProfileType CurrentProfile();

} // namespace QuickSilverImpl

#endif // !QUICKSILVER_COMMON_QS_PROFILE_H_

