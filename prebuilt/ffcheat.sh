#!/system/bin/sh
# RAPZZ root injector — com.dts.freefireth
# Push librapzz.so + rapzz_inject + ffcheat.sh to /data/local/tmp, then:
#   su -c sh /data/local/tmp/ffcheat.sh
# Credit: RAPZZ

PKG=com.dts.freefireth
TMP=/data/local/tmp
SRC_LIB=$TMP/librapzz.so
INJ=$TMP/rapzz_inject
DST_DIR=/data/data/$PKG/rapzz
DST_LIB=$DST_DIR/librapzz.so

# self-elevate
if [ "$(id -u)" != "0" ]; then
  exec su -c "sh $0 $*"
fi

echo "[RAPZZ] root ok"

if [ ! -f "$SRC_LIB" ]; then echo "[-] missing $SRC_LIB"; exit 1; fi
if [ ! -f "$INJ" ]; then echo "[-] missing $INJ"; exit 1; fi
chmod 755 "$INJ"

# put the .so where the app process can read it (selinux-safe)
UID_APP=$(stat -c %u /data/data/$PKG 2>/dev/null)
[ -z "$UID_APP" ] && UID_APP=$(dumpsys package $PKG | grep userId= | head -n1 | sed 's/.*userId=//;s/ .*//')
mkdir -p "$DST_DIR"
cp -f "$SRC_LIB" "$DST_LIB"
chown -R "$UID_APP:$UID_APP" "$DST_DIR" 2>/dev/null
chmod 755 "$DST_DIR"
chmod 755 "$DST_LIB"
restorecon -R "$DST_DIR" 2>/dev/null
echo "[RAPZZ] lib staged -> $DST_LIB (uid $UID_APP)"

# make sure the game is running
PID=$(pidof $PKG)
if [ -z "$PID" ]; then
  echo "[RAPZZ] launching $PKG ..."
  monkey -p $PKG -c android.intent.category.LAUNCHER 1 >/dev/null 2>&1
  i=0
  while [ -z "$PID" ] && [ $i -lt 60 ]; do
    sleep 1
    PID=$(pidof $PKG)
    i=$((i+1))
  done
fi
if [ -z "$PID" ]; then echo "[-] game not running"; exit 1; fi
echo "[RAPZZ] pid = $PID"

# wait for the process to settle (il2cpp loads shortly after)
sleep 3

"$INJ" "$PID" "$DST_LIB" > $TMP/rapzz.log 2>&1
RC=$?
cat $TMP/rapzz.log
if [ $RC -eq 0 ]; then
  echo "[RAPZZ] done. menu auto-opens; 5-finger tap toggles it."
else
  echo "[-] inject failed (rc=$RC). try again once the game is at the lobby."
fi
exit $RC
