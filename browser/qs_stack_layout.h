// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_BROWSER_QS_STACK_LAYOUT_H_
#define QUICKSILVER_BROWSER_QS_STACK_LAYOUT_H_

#include <list>

#include "ui/aura/layout_manager.h"
#include "ui/gfx/geometry/insets.h"

namespace aura {
class Window;
}

namespace QuickSilverImpl {

class QSStackLayout : public aura::LayoutManager {
 public:
  explicit QSStackLayout(aura::Window* root);
  ~QSStackLayout() override;

  aura::Window* top_window() const { return visible_child_; }
  void SetInsets(gfx::Insets& insets);

 private:
  // aura::LayoutManager:
  void OnWindowResized() override;
  void OnWindowAddedToLayout(aura::Window* child) override;
  void OnWillRemoveWindowFromLayout(aura::Window* child) override {};
  void OnWindowRemovedFromLayout(aura::Window* child) override;
  void OnChildWindowVisibilityChanged(aura::Window* child,
                                      bool visible) override;
  void SetChildBounds(aura::Window* child,
                      const gfx::Rect& requested_bounds) override;

  aura::Window* root_;
  aura::Window* visible_child_;
  std::list<aura::Window*> children_;
  gfx::Insets insets_;

  DISALLOW_COPY_AND_ASSIGN(QSStackLayout);
};

} // namespace QuickSilverImpl

#endif // !QUICKSILVER_BROWSER_QS_STACK_LAYOUT_H_


