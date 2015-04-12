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
      'target_name': 'ozone_qt',
      'type': 'static_library',
      'dependencies': [
        '<(DEPTH)/qt_port/build/system.gyp:qtcore',
        '<(DEPTH)/qt_port/build/system.gyp:qtgui',
        '<(DEPTH)/third_party/khronos/khronos.gyp:khronos_headers',
      ],
      'sources': [
        'media_ozone_platform_qt.cc',
        'ozone_platform_qt.h',
        'ozone_platform_qt.cc',
        'surface_factory_qt.h',
        'surface_factory_qt.cc',
      ],
    }, # ui_base_qt
  ],
}
