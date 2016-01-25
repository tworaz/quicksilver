// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/common/qs_profile.h"

#include "base/command_line.h"
#include "base/logging.h"
#include "quicksilver/common/qs_switches.h"

namespace QuickSilverImpl {

ProfileType CurrentProfile() {
  base::CommandLine* cmdline = base::CommandLine::ForCurrentProcess();
  if (cmdline->HasSwitch(switches::kQuickSilverProfile)) {
    std::string profile_string = cmdline->GetSwitchValueASCII(
        switches::kQuickSilverProfile);
    if (profile_string == switches::kQuickSilverProfileDesktop) {
      return ProfileType::DESKTOP;
    } else if (profile_string == switches::kQuickSilverProfileMobile) {
      return ProfileType::MOBILE;
    } else if (profile_string == switches::kQuickSilverProfileTablet) {
      return ProfileType::TABLET;
    } else {
      LOG(WARNING) << "Unrecognized QuickSilver profile: " << profile_string;
    }
  }

#if defined(QUICKSILVER_PROFILE_MOBILE)
  return ProfileType::MOBILE;
#elif defined(QUICKSILVER_PROFILE_TABLET)
  return ProfileType::TABLET;
#else
  return ProfileType::DESKTOP;
#endif
}

} // namespace QuickSilverImpl
