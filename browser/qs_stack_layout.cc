// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/browser/qs_stack_layout.h"

#include <algorithm>

#include "base/logging.h"
#include "ui/aura/window.h"

namespace QuickSilverImpl {

QSStackLayout::QSStackLayout(aura::Window* root)
    : root_(root)
    , visible_child_(nullptr) {
}

QSStackLayout::~QSStackLayout() {
  children_.clear();
  visible_child_ = nullptr;
}

void QSStackLayout::SetInsets(gfx::Insets& insets) {
  if (insets_ != insets) {
    insets_ = insets;
    OnWindowResized();
  }
}

void QSStackLayout::OnWindowResized() {
  if (visible_child_) {
    gfx::Rect bounds = root_->bounds();
    bounds.Inset(insets_);
    visible_child_->SetBounds(bounds);
  }
}

void QSStackLayout::OnWindowAddedToLayout(aura::Window* child) {
  DVLOG(1) << "Window added to layout:" << child;
  DCHECK(std::find(children_.begin(), children_.end(), child) == children_.end());
  children_.push_front(child);
}

void QSStackLayout::OnWindowRemovedFromLayout(aura::Window* child) {
  DVLOG(1) << "Window removed from layout:" << child;
  std::list<aura::Window*>::iterator it =
      std::find(children_.begin(), children_.end(), child);
  DCHECK(it != children_.end());
  children_.erase(it);
  if (child == visible_child_) {
    if (children_.size()) {
      (*children_.begin())->Show();
    } else {
      visible_child_ = nullptr;
    }
  }
}

void QSStackLayout::OnChildWindowVisibilityChanged(
    aura::Window* child, bool visible) {
  if (visible_child_ && visible_child_ != child && visible) {
    visible_child_->Hide();
    visible_child_ = nullptr;
  }

  DVLOG(1) << "Window visibility changed:" << child << ", visible: " << visible;

  if (visible) {
    visible_child_ = child;
    OnWindowResized();

    if (children_.size() > 1 && *children_.begin() != child) {
      std::list<aura::Window*>::iterator it =
          std::find(children_.begin(), children_.end(), child);
      aura::Window* temp = *it;
      children_.erase(it);
      children_.push_front(temp);
    }
  }
}

void QSStackLayout::SetChildBounds(
    aura::Window* child, const gfx::Rect& requested_bounds) {
  SetChildBoundsDirect(child, requested_bounds);
}

} // namespace QuickSilverImpl
