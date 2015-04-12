# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'external_ui_base_deps': [
      '<(DEPTH)/qt_port/ui/ui_qt.gyp:ui_base_qt',
    ],
    'external_ui_gl_deps': [
      '<(DEPTH)/qt_port/build/system.gyp:qtgui',
    ],
    'external_ui_gfx_x11_deps': [
      '<(DEPTH)/qt_port/build/system.gyp:qtgui',
    ],
  },

  'target_defaults': {
    'target_conditions': [
      ['_target_name=="gl"', {
        'sources': [
          'gl/gl_share_context_qt.h',
          'gl/gl_share_context_qt.cc',
        ],
      }], # gl
      ['_target_name=="gfx_x11"', {
        'sources': [
          'gfx/x/x11_types_qt.cc',
        ],
      }], # gfx_x11
      ['_target_name=="ui_base"', {
        'sources/': [
          ['exclude', 'cursor/cursor_loader_ozone\\.(cc|h)$', ]
        ],
      }], # ui_base
    ],
  },
}


