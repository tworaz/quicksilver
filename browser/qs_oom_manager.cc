// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/browser/qs_oom_manager.h"

#include "base/bind.h"
#include "quicksilver/browser/qs_web_view_impl.h"

namespace QuickSilverImpl {

namespace {
const int kViewRendererDiscardIntervalSec = 5;
}

// static
QSOomManager* QSOomManager::GetInstance() {
  return base::Singleton<QSOomManager>::get();
}

QSOomManager::QSOomManager() {
  memory_pressure_listener_.reset(new base::MemoryPressureListener(
      base::Bind(&QSOomManager::OnMemoryPressure, base::Unretained(this))));
}

QSOomManager::~QSOomManager() {
}

void QSOomManager::AddWebView(QSWebViewImpl* wv) {
  DVLOG(1) << "QS OOM Manager, view added: " << wv;
  auto it = std::find(managed_views_.begin(), managed_views_.end(), wv);
  DCHECK(it == managed_views_.end());
  managed_views_.push_back(wv);
}

void QSOomManager::RemoveWebView(QSWebViewImpl* wv) {
  DVLOG(1) << "QS OOM Manager, view removed: " << wv;
  auto it = std::find(managed_views_.begin(), managed_views_.end(), wv);
  DCHECK(it != managed_views_.end());
  managed_views_.erase(it);
}

void QSOomManager::WebViewActivated(QSWebViewImpl* wv) {
  DVLOG(1) << "QS OOM Manager, view activated: " << wv;
  auto it = std::find(managed_views_.begin(), managed_views_.end(), wv);
  DCHECK(it != managed_views_.end());
  if (managed_views_.size() > 1) {
    QSWebViewImpl* wv = *it;
    managed_views_.erase(it);
    managed_views_.push_back(wv);
  }
}

void QSOomManager::OnMemoryPressure(
    base::MemoryPressureListener::MemoryPressureLevel level) {
  DVLOG(1) << "Memory pressure level changed to: " << level;
  switch (level) {
    case base::MemoryPressureListener::MEMORY_PRESSURE_LEVEL_NONE:
      timer_.Stop();
      break;
    case base::MemoryPressureListener::MEMORY_PRESSURE_LEVEL_MODERATE:
      OnModerateMemoryPressure();
      break;
    case base::MemoryPressureListener::MEMORY_PRESSURE_LEVEL_CRITICAL:
      OnCriticalMemoryPressure();
      break;
  }
}

void QSOomManager::OnModerateMemoryPressure() {
  // Keep discarding inactive renderer processes every
  // kViewRendererDiscardIntervalSec until the memory usage does not
  // drop to acceptablet levels.
  if (!timer_.IsRunning()) {
    timer_.Start(FROM_HERE,
                 base::TimeDelta::FromSeconds(kViewRendererDiscardIntervalSec),
                 this, &QSOomManager::DiscardNextViewRenderer);
    DiscardNextViewRenderer();
  }
}

void QSOomManager::OnCriticalMemoryPressure() {
  // Discard renderer processes for all inactive views.
  for (QSWebViewImpl* wv : managed_views_) {
    if (!wv->IsVisible()) {
      wv->DiscardRenderer();
    }
  }
}

void QSOomManager::DiscardNextViewRenderer() {
  for (QSWebViewImpl* wv : managed_views_) {
    if (!wv->IsVisible() && wv->HasRenderView()) {
      wv->DiscardRenderer();
      return;
    }
  }
}

} // namespace QuickSilverImpl
