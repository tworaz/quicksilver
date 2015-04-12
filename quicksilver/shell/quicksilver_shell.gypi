# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'targets': [{
    'target_name': 'quicksilver_shell',
    'type': 'executable',
    'dependencies': [
      '<(DEPTH)/qt_port/build/system.gyp:qtquick',
      '<(DEPTH)/qt_port/build/system.gyp:sailfishapp',
      'quicksilver',
      'quicksilver_renderer',
    ],
    'include_dirs': [
      '../browser/public',
    ],
    'sources': [
      'main.cc',
      '<(rcc_gen_dir)/rcc_quicksilver_shell.cc',
    ],
    'actions': [{
      'action_name': 'genrate_quicksilver_shell_rcc',
      'variables': {
        'rcc_file': 'quicksilver_shell.qrc',
        'resources': [
          'shell/icons/back.png',
          'shell/icons/forward.png',
          'shell/icons/stop.png',
          'shell/icons/reload.png',
          'shell/icons/window-new.png',
          'shell/icons/window-close.png',
          'shell/quicksilver_shell.qml',
          'shell/quicksilver_shell_sf.qml',
        ],
        'rcc_output': 'rcc_quicksilver_shell.cc',
        'rcc_name': 'quicksilver_shell',
      },
      'includes': [ '../../build/rcc_action.gypi' ],
    }],
  }],
}

