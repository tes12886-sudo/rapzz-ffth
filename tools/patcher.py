#!/usr/bin/env python3
# RAPZZ APK patcher — com.dts.freefireth
# Replaces classes.dex with the RAPZZ-loaded dex and adds lib/arm64-v8a/librapzz.so.
# Usage: python patcher.py <base.apk> <classes_patched.dex> <librapzz.so> <out.apk>
import sys, zipfile, shutil, os

def patch(base, dex, so, out):
    with open(dex, "rb") as f: dex_data = f.read()
    with open(so, "rb") as f: so_data = f.read()
    zin = zipfile.ZipFile(base, "r")
    if os.path.exists(out): os.remove(out)
    zout = zipfile.ZipFile(out, "w")
    replaced = False
    for item in zin.infolist():
        data = zin.read(item.filename)
        if item.filename == "classes.dex":
            data = dex_data
            replaced = True
        # preserve original compression method per entry
        zi = zipfile.ZipInfo(item.filename, date_time=item.date_time)
        zi.compress_type = item.compress_type
        zi.external_attr = item.external_attr
        zi.internal_attr = item.internal_attr
        zi.create_system = item.create_system
        zout.writestr(zi, data)
    if not replaced:
        zout.writestr("classes.dex", dex_data)
    zout.writestr("lib/arm64-v8a/librapzz.so", so_data, compress_type=zipfile.ZIP_STORED)
    zout.close(); zin.close()
    print(f"[RAPZZ] patched -> {out} ({os.path.getsize(out):,} bytes)")

if __name__ == "__main__":
    if len(sys.argv) != 5:
        print(__doc__); sys.exit(1)
    patch(*sys.argv[1:])
