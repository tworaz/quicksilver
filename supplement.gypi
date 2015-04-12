# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'use_qt%': 1,
    'sailfish%': 0,
    'moc_gen_dir': '<(SHARED_INTERMEDIATE_DIR)/qt_moc',
    'rcc_gen_dir': '<(SHARED_INTERMEDIATE_DIR)/qt_rcc',
    'qt_moc%': '/usr/lib/qt5/bin/moc',
    'qt_rcc%': '/usr/lib/qt5/bin/rcc',
  },

  'conditions': [
    ['use_qt==1', {
      'variables': {
        'grit_defines': [ '-D' 'use_aura' ],
      },
    }], # use_qt==1
    ['use_qt==1 and use_ozone==1', {
      'variables': {
        'external_ozone_platform_deps': [
          '<(DEPTH)/qt_port/ozone/ozone_qt.gyp:ozone_qt',
        ],
        'external_ozone_platforms': [
          'qt',
        ],
        'ozone_auto_platforms': 0,
      },
    }], # use_qt==1 and use_ozone==1
  ],

  'target_defaults': {
    'conditions': [
      ['use_qt==1', {
        'defines': [
          'COMPONENT_BUILD=1',
          'SKIA_DLL=1',
          'BUILDING_V8_SHARED=1',
          'V8_SHARED=1',
          'USE_QT',
        ],
        'include_dirs': [
          '<(DEPTH)/qt_port',
        ],
      }], # use_qt==1
      ['sailfish==1', {
        'defines': [
          'OS_SAILFISH=1',
        ],
      }], # sailfish==1
    ],
  },

  'includes': [
    'content/content_qt.gypi',
    'ui/ui_qt.gypi',
  ],
}
