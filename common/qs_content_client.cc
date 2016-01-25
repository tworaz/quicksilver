// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/common/qs_content_client.h"

#include "base/command_line.h"
#include "content/public/common/content_switches.h"
#include "content/public/common/user_agent.h"
#include "quicksilver/common/qs_profile.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/resource/resource_bundle.h"

namespace QuickSilverImpl {

QSContentClient::QSContentClient()
    : content::ContentClient() {
}

std::string QSContentClient::GetUserAgent() const {
  std::string product = "Chrome/" CHROMIUM_VERSION;
#if defined(OS_SAILFISH)
  std::string os_info = "Linux; Jolla; Sailfish";
#else
  std::string os_info = "Linux";
#endif
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();

  switch (CurrentProfile()) {
    case ProfileType::MOBILE:
      os_info += "; Android 4.4; Mobile";
      product += " Mobile";
      break;
    case ProfileType::TABLET:
      os_info += "; Android 4.4; Tablet";
      break;
    default:
      break;
  }

  return content::BuildUserAgentFromOSAndProduct(os_info, product);
}

base::string16 QSContentClient::GetLocalizedString(int message_id) const {
  return l10n_util::GetStringUTF16(message_id);
}

base::StringPiece QSContentClient::GetDataResource(
    int resource_id, ui::ScaleFactor scale_factor) const {
  return ResourceBundle::GetSharedInstance().GetRawDataResourceForScale(
      resource_id, scale_factor);
}

base::RefCountedStaticMemory* QSContentClient::GetDataResourceBytes(
    int resource_id) const {
  return ResourceBundle::GetSharedInstance().LoadDataResourceBytes(resource_id);
}

gfx::Image& QSContentClient::GetNativeImageNamed(int resource_id) const {
  return ResourceBundle::GetSharedInstance().GetNativeImageNamed(resource_id);
}

}
