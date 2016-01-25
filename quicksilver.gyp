# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'quicksilver_data_dir%': '',
    'quicksilver_profile%': 'mobile',
  },

  'targets': [
    {
      'target_name': '<(quicksilver_target)',
      'type': 'shared_library',
      'dependencies': [
        '<(DEPTH)/components/components.gyp:content_settings_core_browser',
        '<(DEPTH)/components/components.gyp:keyed_service_core',
        '<(DEPTH)/content/content.gyp:content',
        '<(DEPTH)/content/content.gyp:content_app_both',
        '<(DEPTH)/ozone_qt/build/system.gyp:qtcore',
        '<(DEPTH)/ozone_qt/build/system.gyp:qtgui',
        '<(DEPTH)/ozone_qt/build/system.gyp:qtquick',
        '<(DEPTH)/ozone_qt/components/components.gyp:aura_client_qt',
        '<(DEPTH)/ozone_qt/components/components.gyp:ime_qt',
        '<(DEPTH)/ozone_qt/components/components.gyp:mce',
        '<(DEPTH)/ozone_qt/components/components.gyp:screen_qt',
        '<(DEPTH)/quicksilver/chrome/chrome_reuse.gyp:chrome_error_pages',
        '<(DEPTH)/third_party/WebKit/public/blink.gyp:blink',
        '<(DEPTH)/ui/aura/aura.gyp:aura',
        '<(DEPTH)/ui/wm/wm.gyp:wm',
        'quicksilver_moc',
        'quicksilver_pak',
      ],
      'defines': [
        'CONTENT_IMPLEMENTATION=1',
        'CHROMIUM_VERSION=\"<!(<(version_cmd) -t "<(version_pattern)")\"',
      ],
      'cflags!': [
        # Symbol visibility controlled by version script
        '-fvisibility=hidden',
      ],
      'product_extension': 'so.<(quicksilver_soname)',
      'link_settings': {
        'ldflags': [
          '-Wl,--no-undefined',
          '-Wl,--version-script=../../quicksilver/quicksilver.filter',
        ],
      },
      'direct_dependent_settings': {
        'include_dirs': [
          '<(DEPTH)/quicksilver/public/common',
          '<(DEPTH)/quicksilver/public/browser',
          '<(DEPTH)/quicksilver/public/renderer',
        ],
      },
      'conditions': [
        ['quicksilver_data_dir!=""', {
          'defines': [
            'QUICKSILVER_DATA_DIR="<(quicksilver_data_dir)"',
          ],
        }],
        ['quicksilver_profile=="mobile"', {
          'defines': [
            'QUICKSILVER_PROFILE_MOBILE',
          ],
        }],
        ['quicksilver_profile=="tablet"', {
          'defines': [
            'QUICKSILVER_PROFILE_TABLET',
          ],
        }],
      ],
      'variables' : {
        'public_api_headers': [
          'public/browser/qs_browser_process.h',
          'public/browser/qs_context_menu_params.h',
          'public/browser/qs_javascript_dialog.h',
          'public/browser/qs_permission_request.h',
          'public/browser/qs_web_area.h',
          'public/browser/qs_web_context.h',
          'public/browser/qs_web_view.h',
          'public/browser/qs_web_view_delegate.h',
          'public/browser/qs_web_window.h',
          'public/common/qs_process.h',
        ],
        'public_api_sources': [
          'public/browser/qs_browser_process.cc',
          'public/browser/qs_context_menu_params.cc',
          'public/browser/qs_javascript_dialog.cc',
          'public/browser/qs_permission_request.cc',
          'public/browser/qs_web_area.cc',
          'public/browser/qs_web_context.cc',
          'public/browser/qs_web_view.cc',
          'public/browser/qs_web_window.cc',
          'public/common/qs_process.cc',
        ],
      },
      'sources': [
        '<@(public_api_headers)',
        '<@(public_api_sources)',

        # Code running in browser/UI process.
        'browser/qs_browser_context.cc',
        'browser/qs_browser_context.h',
        'browser/qs_browser_main_parts.cc',
        'browser/qs_browser_main_parts.h',
        'browser/qs_content_browser_client.cc',
        'browser/qs_content_browser_client.h',
        'browser/qs_javascript_dialog_manager.cc',
        'browser/qs_javascript_dialog_manager.h',
        'browser/qs_network_delegate.cc',
        'browser/qs_network_delegate.h',
        'browser/qs_oom_manager.cc',
        'browser/qs_oom_manager.h',
        'browser/qs_permission_manager.cc',
        'browser/qs_permission_manager.h',
        'browser/qs_pref_service_factory.cc',
        'browser/qs_pref_service_factory.h',
        'browser/qs_stack_layout.cc',
        'browser/qs_stack_layout.h',
        'browser/qs_url_request_context_getter.cc',
        'browser/qs_url_request_context_getter.h',
        'browser/qs_web_contents_view_delegate.cc',
        'browser/qs_web_contents_view_delegate.h',
        'browser/qs_web_view_impl.cc',
        'browser/qs_web_view_impl.h',
        'browser/qs_web_window_impl.cc',
        'browser/qs_web_window_impl.h',

        # Code running browser and renderer processes.
        'common/qs_message_generator.cc',
        'common/qs_message_generator.h',
        'common/qs_content_client.cc',
        'common/qs_content_client.h',
        'common/qs_main_delegate.cc',
        'common/qs_main_delegate.h',
        'common/qs_messages.h',
        'common/qs_profile.cc',
        'common/qs_profile.h',
        'common/qs_switches.cc',
        'common/qs_switches.h',

        # Code running in renderer process.
        'renderer/qs_render_view_observer.cc',
        'renderer/qs_render_view_observer.h',
        'renderer/qs_content_renderer_client.cc',
        'renderer/qs_content_renderer_client.h',

        # Sources generated quicksilver_moc.
        '<(moc_gen_dir)/moc_qs_context_menu_params.cc',
        '<(moc_gen_dir)/moc_qs_javascript_dialog.cc',
        '<(moc_gen_dir)/moc_qs_permission_request.cc',
        '<(moc_gen_dir)/moc_qs_web_area.cc',
        '<(moc_gen_dir)/moc_qs_web_context.cc',
        '<(moc_gen_dir)/moc_qs_web_view.cc',
        '<(moc_gen_dir)/moc_qs_web_window.cc',

        # Reused upstream sources.
        '<(DEPTH)/ui/aura/test/test_focus_client.cc',
        '<(DEPTH)/ui/aura/test/test_focus_client.h',
      ],
      'actions': [{
        'action_name': 'prepare_api_headers',
        'inputs': [
          '<@(public_api_headers)',
        ],
        'outputs': [
          '<(PRODUCT_DIR)/QuickSilver.stamp',
        ],
        'action': [
          'python', 'build/prepare_api_headers.py',
          '--stamp', '<(PRODUCT_DIR)/QuickSilver.stamp',
          '<@(public_api_headers)', '<(PRODUCT_DIR)/QuickSilver/'
        ],
      }],
    }, # quicksilver
    {
      'target_name': 'quicksilver_moc',
      'type': 'none',
      'sources': [
        'public/browser/qs_context_menu_params.h',
        'public/browser/qs_javascript_dialog.h',
        'public/browser/qs_permission_request.h',
        'public/browser/qs_web_area.h',
        'public/browser/qs_web_context.h',
        'public/browser/qs_web_view.h',
        'public/browser/qs_web_window.h',
      ],
      'rules': [{
        'rule_name': 'generate_quicksilver_moc',
        'includes': [ '../ozone_qt/build/moc_rule.gypi' ],
      }],
    }, # quicksilver_moc
    {
      'target_name': 'quicksilver_worker',
      'type': 'executable',
      'dependencies': [
        '<(DEPTH)/ozone_qt/build/system.gyp:qtcore',
        '<(quicksilver_target)',
      ],
      'sources': [
        'renderer/renderer_main.cc',
      ],
    }, # quicksilver_worker
    {
      'target_name': 'quicksilver_pak',
      'type': 'none',
      'dependencies': [
        '<(DEPTH)/components/components_strings.gyp:components_strings',
        '<(DEPTH)/chrome/chrome_resources.gyp:chrome_resources',
        '<(DEPTH)/components/components.gyp:devtools_http_handler',
        '<(DEPTH)/content/content.gyp:content_resources',
        '<(DEPTH)/net/net.gyp:net_resources',
        '<(DEPTH)/third_party/WebKit/public/blink_resources.gyp:blink_image_resources',
        '<(DEPTH)/third_party/WebKit/public/blink_resources.gyp:blink_resources',
        '<(DEPTH)/ui/resources/ui_resources.gyp:ui_resources',
        '<(DEPTH)/ui/strings/ui_strings.gyp:ui_strings',
      ],
      'actions': [
        {
          'action_name': 'repack_quicksilver_pack',
          'variables': {
            'pak_inputs': [
              '<(SHARED_INTERMEDIATE_DIR)/blink/public/resources/blink_image_resources_100_percent.pak',
              '<(SHARED_INTERMEDIATE_DIR)/blink/public/resources/blink_resources.pak',
              '<(SHARED_INTERMEDIATE_DIR)/chrome/renderer_resources_100_percent.pak',
              '<(SHARED_INTERMEDIATE_DIR)/content/app/resources/content_resources_100_percent.pak',
              '<(SHARED_INTERMEDIATE_DIR)/content/content_resources.pak',
              '<(SHARED_INTERMEDIATE_DIR)/net/net_resources.pak',
              '<(SHARED_INTERMEDIATE_DIR)/ui/resources/ui_resources_100_percent.pak',
              '<(SHARED_INTERMEDIATE_DIR)/ui/resources/webui_resources.pak',

              # TODO: Store strings in the localization paks
              '<(SHARED_INTERMEDIATE_DIR)/chrome/chromium_strings_en-US.pak',
              '<(SHARED_INTERMEDIATE_DIR)/chrome/generated_resources_en-US.pak',
              '<(SHARED_INTERMEDIATE_DIR)/content/app/strings/content_strings_en-US.pak',
              '<(SHARED_INTERMEDIATE_DIR)/ui/strings/app_locale_settings_en-US.pak',
            ],
            'pak_output': '<(PRODUCT_DIR)/quicksilver.pak',
          },
          'includes': [ '../build/repack_action.gypi' ],
        },
      ],
    }, # quicksilver_pak
  ],

  'includes': [
    'shell/quicksilver_shell.gypi',
  ],
}
