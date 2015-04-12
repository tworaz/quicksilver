// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/common/cursors/webcursor.h"

#include <QCursor>

#include "third_party/WebKit/public/platform/WebCursorInfo.h"

namespace content {

gfx::NativeCursor WebCursor::GetNativeCursor() const {
  switch (type_) {
  case blink::WebCursorInfo::TypePointer:
    return Qt::ArrowCursor;
  case blink::WebCursorInfo::TypeCross:
    return Qt::CrossCursor;
  case blink::WebCursorInfo::TypeHand:
    return Qt::PointingHandCursor;
  case blink::WebCursorInfo::TypeIBeam:
    return Qt::IBeamCursor;
  case blink::WebCursorInfo::TypeWait:
    return Qt::WaitCursor;
  case blink::WebCursorInfo::TypeHelp:
    return Qt::WhatsThisCursor;
  case blink::WebCursorInfo::TypeEastResize:
  case blink::WebCursorInfo::TypeWestResize:
  case blink::WebCursorInfo::TypeEastWestResize:
  case blink::WebCursorInfo::TypeEastPanning:
  case blink::WebCursorInfo::TypeWestPanning:
    return Qt::SizeHorCursor;
  case blink::WebCursorInfo::TypeNorthResize:
  case blink::WebCursorInfo::TypeSouthResize:
  case blink::WebCursorInfo::TypeNorthSouthResize:
  case blink::WebCursorInfo::TypeNorthPanning:
  case blink::WebCursorInfo::TypeSouthPanning:
    return Qt::SizeVerCursor;
  case blink::WebCursorInfo::TypeNorthEastResize:
  case blink::WebCursorInfo::TypeSouthWestResize:
  case blink::WebCursorInfo::TypeNorthEastSouthWestResize:
  case blink::WebCursorInfo::TypeNorthEastPanning:
  case blink::WebCursorInfo::TypeSouthWestPanning:
    return Qt::SizeBDiagCursor;
  case blink::WebCursorInfo::TypeNorthWestResize:
  case blink::WebCursorInfo::TypeSouthEastResize:
  case blink::WebCursorInfo::TypeNorthWestSouthEastResize:
  case blink::WebCursorInfo::TypeNorthWestPanning:
  case blink::WebCursorInfo::TypeSouthEastPanning:
    return Qt::SizeFDiagCursor;
  case blink::WebCursorInfo::TypeColumnResize:
    return Qt::SplitHCursor;
  case blink::WebCursorInfo::TypeRowResize:
    return Qt::SplitVCursor;
  case blink::WebCursorInfo::TypeMiddlePanning:
  case blink::WebCursorInfo::TypeMove:
    return Qt::SizeAllCursor;
  case blink::WebCursorInfo::TypeVerticalText:
  case blink::WebCursorInfo::TypeCell:
  case blink::WebCursorInfo::TypeContextMenu:
  case blink::WebCursorInfo::TypeAlias:
  case blink::WebCursorInfo::TypeProgress:
  case blink::WebCursorInfo::TypeCopy:
  case blink::WebCursorInfo::TypeZoomIn:
  case blink::WebCursorInfo::TypeZoomOut:
    // FIXME: Load from the resource bundle.
    return Qt::ArrowCursor;
  case blink::WebCursorInfo::TypeNoDrop:
  case blink::WebCursorInfo::TypeNotAllowed:
    return Qt::ForbiddenCursor;
  case blink::WebCursorInfo::TypeNone:
    return Qt::BlankCursor;
  case blink::WebCursorInfo::TypeGrab:
    return Qt::OpenHandCursor;
  case blink::WebCursorInfo::TypeGrabbing:
    return Qt::ClosedHandCursor;
  case blink::WebCursorInfo::TypeCustom:
    // FIXME: Extract from the CursorInfo.
    return Qt::ArrowCursor;
  default:
    NOTREACHED();
    return Qt::ArrowCursor;
  }
}

void WebCursor::InitPlatformData() {
}

bool WebCursor::SerializePlatformData(Pickle* pickle) const {
  return true;
}

bool WebCursor::DeserializePlatformData(PickleIterator* iter) {
  return true;
}

bool WebCursor::IsPlatformDataEqual(const WebCursor& other) const {
  return true;
}

void WebCursor::CleanupPlatformData() {
}

void WebCursor::CopyPlatformData(const WebCursor& other) {
}

}  // namespace content

