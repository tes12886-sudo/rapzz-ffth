# RAPZZ — push root injector to device and run it.
# Requires: adb, rooted device (Magisk), game installed (com.dts.freefireth).
$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
$b = "$root\build"
$adb = "adb"

& $adb push "$b\librapzz.so"   /data/local/tmp/librapzz.so
& $adb push "$b\rapzz_inject"  /data/local/tmp/rapzz_inject
& $adb push "$b\ffcheat.sh"    /data/local/tmp/ffcheat.sh
& $adb shell chmod 755 /data/local/tmp/rapzz_inject /data/local/tmp/ffcheat.sh
Write-Host "Running injector as root..."
& $adb shell "su -c 'sh /data/local/tmp/ffcheat.sh'"
