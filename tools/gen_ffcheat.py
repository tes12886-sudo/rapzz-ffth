import base64, textwrap, os
here = os.path.dirname(os.path.abspath(__file__))
build = os.path.join(here, "..", "build")
d = open(os.path.join(build, "rapzz_inject"), "rb").read()
b64 = textwrap.fill(base64.b64encode(d).decode(), 76)

script = r'''#!/system/bin/sh
# RAPZZ root injector v3 (self-extracting, libdl fix) - com.dts.freefireth
# Only this file needs updating; the correct injector binary is embedded below.
PKG=com.dts.freefireth
TMP=/data/local/tmp
SRC_LIB=$TMP/librapzz.so
INJ=$TMP/rapzz_inject
DST_DIR=/data/data/$PKG/rapzz
DST_LIB=$DST_DIR/librapzz.so

if [ "$(id -u)" != "0" ]; then exec su -c "sh $0 $*"; fi
echo "[RAPZZ] root ok (injector v3/libdl)"

if [ ! -f "$SRC_LIB" ]; then echo "[-] missing $SRC_LIB"; exit 1; fi

base64 -d > "$INJ" <<'RAPZZ_B64_EOF'
__B64__
RAPZZ_B64_EOF
chmod 755 "$INJ"
echo "[RAPZZ] injector extracted ($(wc -c < "$INJ") bytes)"

UID_APP=$(stat -c %u /data/data/$PKG 2>/dev/null)
[ -z "$UID_APP" ] && UID_APP=$(dumpsys package $PKG | grep userId= | head -n1 | sed 's/.*userId=//;s/ .*//')
mkdir -p "$DST_DIR"
cp -f "$SRC_LIB" "$DST_LIB"
chown -R "$UID_APP:$UID_APP" "$DST_DIR" 2>/dev/null
chmod 755 "$DST_DIR"
chmod 755 "$DST_LIB"
restorecon -R "$DST_DIR" 2>/dev/null
echo "[RAPZZ] lib staged -> $DST_LIB (uid $UID_APP)"

PID=$(pidof $PKG)
if [ -z "$PID" ]; then
  echo "[RAPZZ] launching $PKG ..."
  monkey -p $PKG -c android.intent.category.LAUNCHER 1 >/dev/null 2>&1
  i=0
  while [ -z "$PID" ] && [ $i -lt 60 ]; do sleep 1; PID=$(pidof $PKG); i=$((i+1)); done
fi
if [ -z "$PID" ]; then echo "[-] game not running"; exit 1; fi
echo "[RAPZZ] pid = $PID"
sleep 3
"$INJ" "$PID" "$DST_LIB" > $TMP/rapzz.log 2>&1
RC=$?
cat $TMP/rapzz.log
if [ $RC -eq 0 ]; then echo "[RAPZZ] done. 5-finger tap toggles menu."; else echo "[-] inject failed rc=$RC"; fi
exit $RC
'''
script = script.replace("__B64__", b64)
out = os.path.join(build, "ffcheat.sh")
with open(out, "w", newline="\n") as f:
    f.write(script)
print("ffcheat.sh written:", len(script), "bytes; injector", len(d), "bytes")
