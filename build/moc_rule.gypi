# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'extension': 'h',
  'outputs': [ '<(moc_gen_dir)/moc_<(RULE_INPUT_ROOT).cc' ],
  'action':  [
    '<(qt_moc)',
    '<(RULE_INPUT_PATH)',
    '-o', '<(moc_gen_dir)/moc_<(RULE_INPUT_ROOT).cc',
  ],
  'message': 'Generating <(RULE_INPUT_ROOT).cc',
}
