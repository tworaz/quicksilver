# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'external_content_browser_deps': [
      '<(DEPTH)/qt_port/ui/ui_qt.gyp:qt',
      '<(DEPTH)/qt_port/content/content_qt.gyp:content_browser_qt',
    ],
    'external_content_common_deps': [
      '<(DEPTH)/qt_port/build/system.gyp:qtgui',
      '<(DEPTH)/qt_port/content/content_qt.gyp:content_common_qt',
    ],
    'external_content_shell_deps': [
      '<(DEPTH)/qt_port/content/content_qt.gyp:shell_lib_qt',
      '<(DEPTH)/qt_port/build/system.gyp:qtgui',
      '<(DEPTH)/qt_port/build/system.gyp:qtquick',
    ],
  },

  'target_defaults': {
    'target_conditions': [

      ['_target_name=="content_common"', {
        'sources': [
          'common/cursors/webcursor_qt.cc',
          'common/cursors/webcursor_qt.h',
        ],
      }], # content_common

      ['_target_name=="content_common"', {
        'sources/': [
          [ 'exclude', 'common/cursors/webcursor_ozone\\.cc$' ],
        ],
      }], # content_gpu

      ['_target_name=="content_child"', {
        'conditions': [
          ['enable_plugins==1', {
            'sources/': [
              [ 'include', 'child/npapi/webplugin_delegate_impl_aura\\.cc$' ],
            ],
          }], # enable_plugins==1
        ]
      }], # content_child

      ['_target_name=="content_renderer"', {
        'sources': [
          '<(DEPTH)/content/renderer/external_popup_menu.cc',
          '<(DEPTH)/content/renderer/external_popup_menu.h',
        ],
      }], # content_renderer

      ['_target_name=="content_shell"', {
        'sources': [
          'shell/app/shell_main_qt.cc'
        ],
        'sources/': [
          [ 'exclude', 'shell/app/shell_main\\.cc$' ],
        ],
      }], # content_shell
    ],
  },
}

