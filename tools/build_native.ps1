# RAPZZ native build — com.dts.freefireth (arm64-v8a)
# Android NDK r27 clang. C and C++ compiled separately, then linked into one .so.
$ErrorActionPreference = "Stop"
$ndk = "C:\Android\android-sdk\ndk\27.3.13750724"
$tc  = "$ndk\toolchains\llvm\prebuilt\windows-x86_64"
$clangxx = "$tc\bin\clang++.exe"
$clang   = "$tc\bin\clang.exe"
$root = Split-Path -Parent $PSScriptRoot
$jni = "$root\jni"
$out = "$root\build"
$obj = "$out\obj"
New-Item -ItemType Directory -Force -Path $out, $obj | Out-Null

$cppSrcs = @()
$cppSrcs += Get-ChildItem "$jni\KittyMemory\*.cpp" | ForEach-Object { $_.FullName }
$cppSrcs += Get-ChildItem "$jni\Misc\imgui\*.cpp" | ForEach-Object { $_.FullName }
$cppSrcs += Get-ChildItem "$jni\Misc\update\*.cpp" | ForEach-Object { $_.FullName }
$cppSrcs += Get-ChildItem "$jni\ZygModule\*.cpp" | ForEach-Object { $_.FullName }
$cSrcs = Get-ChildItem "$jni\Misc\update\xdl\*.c" | ForEach-Object { $_.FullName }

$incs = @(
  $jni, "$jni\Hush", "$jni\ZygModule", "$jni\Misc", "$jni\Misc\Unity",
  "$jni\Misc\update\xdl\include", "$jni\Misc\update", "$jni\Misc\update\xdl",
  "$jni\Misc\imgui", "$jni\Misc\imgui\fonts"
) | ForEach-Object { "-I$_" }

$common = @(
  "--target=aarch64-linux-android21", "--sysroot=$tc\sysroot",
  "-fPIC", "-O2", "-fvisibility=hidden", "-ffunction-sections", "-fdata-sections",
  "-w", "-DNDEBUG"
)
$cxxFlags = $common + @("-std=c++17", "-fno-rtti", "-fno-exceptions", "-fpermissive", "-fms-extensions")
$cFlags   = $common + @("-std=c11")

$objs = @()
$i = 0
foreach ($s in $cSrcs) {
  $o = "$obj\c_$i.o"; $i++
  & $clang @cFlags @incs -c $s -o $o
  if ($LASTEXITCODE -ne 0) { throw "C compile failed: $s" }
  $objs += $o
}
$i = 0
foreach ($s in $cppSrcs) {
  $o = "$obj\cxx_$i.o"; $i++
  & $clangxx @cxxFlags @incs -c $s -o $o
  if ($LASTEXITCODE -ne 0) { throw "C++ compile failed: $s" }
  $objs += $o
}

$ldflags = @(
  "--target=aarch64-linux-android21", "--sysroot=$tc\sysroot",
  "-shared", "-static-libstdc++",
  "-Wl,--gc-sections", "-Wl,--strip-all", "-Wl,-z,max-page-size=16384",
  "-llog", "-landroid", "-lEGL", "-lGLESv3", "-lGLESv2", "-lGLESv1_CM", "-lz"
)
Write-Host "Linking $($objs.Count) objects -> $out\librapzz.so"
& $clangxx @ldflags @objs -o "$out\librapzz.so"
if ($LASTEXITCODE -ne 0) { throw "link failed ($LASTEXITCODE)" }
Write-Host ("OK  librapzz.so  {0:N0} bytes" -f (Get-Item "$out\librapzz.so").Length)
