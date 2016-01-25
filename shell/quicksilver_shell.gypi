# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'targets': [
    {
      'target_name': 'quicksilver_shell',
      'type': 'executable',
      'dependencies': [
        '<(DEPTH)/ozone_qt/build/system.gyp:qtcore',
        '<(DEPTH)/ozone_qt/build/system.gyp:qtgui',
        '<(DEPTH)/ozone_qt/build/system.gyp:qtqml',
        '<(DEPTH)/ozone_qt/build/system.gyp:qtquick',
        '<(quicksilver_target)',
        'quicksilver_worker',
        'shell_moc',
      ],
      'sources': [
        'main.cc',
        'shell_browser_process.cc',
        'shell_browser_process.h',
        'shell_chrome.cc',
        'shell_chrome.h',
        'shell_tab_model.cc',
        'shell_tab_model.h',

        # Generated sources
        '<(moc_gen_dir)/moc_shell_chrome.cc',
        '<(moc_gen_dir)/moc_shell_tab_model.cc',
        '<(rcc_gen_dir)/rcc_quicksilver_shell.cc',
      ],
      'actions': [{
        'action_name': 'genrate_quicksilver_shell_rcc',
        'variables': {
          'rcc_file': 'quicksilver_shell.qrc',
          'resources': [
            'shell/icons/add.png',
            'shell/icons/add_black.png',
            'shell/icons/back.png',
            'shell/icons/back_black.png',
            'shell/icons/close.png',
            'shell/icons/close_black.png',
            'shell/icons/forward.png',
            'shell/icons/reload.png',
            'shell/icons/stop.png',
            'shell/icons/tabs.png',
            'shell/ui/common/BackgroundItem.qml',
            'shell/ui/common/ContextMenu.qml',
            'shell/ui/common/DialogButton.qml',
            'shell/ui/common/JavaScriptDialog.qml',
            'shell/ui/common/PermissionDialog.qml',
            'shell/ui/common/TopIndicator.qml',
            'shell/ui/common/UIState.qml',
            'shell/ui/desktop/Chrome.qml',
            'shell/ui/desktop/TabsView.qml',
            'shell/ui/sailfish/Chrome.qml',
            'shell/ui/sailfish/TabsPage.qml',
          ],
          'rcc_output': 'rcc_quicksilver_shell.cc',
          'rcc_name': 'quicksilver_shell',
        },
        'includes': [ '../../ozone_qt/build/rcc_action.gypi' ],
      }],
    }, # quicksilver_shell
    {
      'target_name': 'shell_moc',
      'type': 'none',
      'sources': [
        'shell_chrome.h',
        'shell_tab_model.h',
      ],
      'rules': [{
        'rule_name': 'generate_quicksilver_moc',
        'includes': [ '../../ozone_qt/build/moc_rule.gypi' ],
      }],
    }, # shell_moc
  ],
}

