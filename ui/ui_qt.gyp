# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'target_defaults': {
    'include_dirs': [
      '<(DEPTH)',
    ],
  },

  'targets': [
    {
      'target_name': 'ui_base_qt',
      'type': 'static_library',
      'dependencies': [
        '<(DEPTH)/skia/skia.gyp:skia',
        '<(DEPTH)/qt_port/build/system.gyp:qtgui',
        'ui_base_qt_moc',
      ],
      'sources': [
        'base/clipboard/clipboard_qt.h',
        'base/clipboard/clipboard_qt.cpp',
        '<(moc_gen_dir)/moc_clipboard_qt.cc',
        'base/cursor/cursor_loader_qt.cc',
        'gfx/screen_qt.cc',
        'snapshot/snapshot_qt.cc',

        # Reusable aura implementations
        '<(DEPTH)/ui/base/resource/resource_bundle_auralinux.cc',
        '<(DEPTH)/ui/base/nine_image_painter_factory.cc',
        '<(DEPTH)/ui/base/nine_image_painter_factory.h',
        '<(DEPTH)/ui/base/cursor/cursor.h',
        '<(DEPTH)/ui/base/cursor/cursor.cc',
        '<(DEPTH)/ui/base/cursor/cursors_aura.h',
        '<(DEPTH)/ui/base/cursor/cursors_aura.cc',
        '<(DEPTH)/ui/native_theme/native_theme_aura.cc',
        '<(DEPTH)/ui/native_theme/native_theme_aura.h',
        '<(DEPTH)/ui/gfx/nine_image_painter.cc',
        '<(DEPTH)/ui/gfx/nine_image_painter.h',
      ],
    }, # ui_base_qt
    {
      'target_name': 'ui_base_qt_moc',
      'type': 'static_library',
      'dependencies': [
        '<(DEPTH)/qt_port/build/system.gyp:qtgui',
      ],
      'sources': [
        'base/clipboard/clipboard_qt.h',
      ],
      'rules': [{
          'rule_name': 'generate_ui_moc',
          'includes': [ '../build/moc_rule.gypi' ],
      }],
    }, # ui_base_qt_moc
    {
      'target_name': 'qt',
      'type': 'static_library',
      'sources': [
        'qt/env.cc',
        'qt/env.h',
        'qt/type_conversion.h',
      ],
    }, # ui_qt
  ],
}
