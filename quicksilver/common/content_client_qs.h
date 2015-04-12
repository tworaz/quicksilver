// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_COMMON_CONTENT_CLIENT_QS_H_
#define QUICKSILVER_COMMON_CONTENT_CLIENT_QS_H_

#include "content/public/common/content_client.h"

namespace QuickSilverImpl {

class ContentClientQS : public content::ContentClient {
 public:
  ContentClientQS();

  // ContentClient overrides.
  std::string GetUserAgent() const override;
  base::string16 GetLocalizedString(int message_id) const override;
  base::StringPiece GetDataResource(
      int resource_id,
      ui::ScaleFactor scale_factor) const override;
  base::RefCountedStaticMemory* GetDataResourceBytes(
      int resource_id) const override;
  gfx::Image& GetNativeImageNamed(int resource_id) const override;
};

} // namespace QuickSilverImpl

#endif  // !QUICKSILVER_COMMON_CONTENT_CLIENT_QS_H_
