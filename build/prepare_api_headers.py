#!/usr/bin/env python

# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

import os
import sys
import shutil

def get_cpp_name(c_name):
    name = os.path.splitext(c_name)[0]
    parts = name.split("_")
    cppname = ""
    for part in parts:
        if part == "qs":
            cppname += part.upper()
            continue
        cppname += part.title()
    return cppname

def prepare_headers(headers, destination):
    for header in headers:
        name = os.path.basename(header)
        cppname = get_cpp_name(name)
        shutil.copy2(header, destination)

        cpp_hdr_content = "#include \"%s\"\n" % name
        cpp_hdr_path = os.path.join(destination, cppname)
        f = open(cpp_hdr_path, "w")
        f.write(cpp_hdr_content)
        f.close()

def main(argv):
    stamp = None
    for idx in range(len(argv)):
        if argv[idx] == "--stamp":
            stamp = argv[idx+1]
            del argv[idx+1]
            del argv[idx]
            break

    dest = argv[-1]
    try:
        os.makedirs(dest)
    except:
        pass

    if os.path.exists(dest) and os.path.isdir(dest):
        prepare_headers(argv[:-1], dest)
        open(stamp, "w").close();

    else:
        sys.stderr.write("[ERROR] Invalid destination: " + dest)
        sys.exit(-1)

if __name__ == "__main__":
    main(sys.argv[1:])
