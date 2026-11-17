"""Kymopsis Radar Base - application-mode views for the PC GUI.

These widgets render the *results* that a radar scenario firmware reports
(people tracks, zones, fall state, vital signs). They consume messages in the
Radar Base JSON schema v0 (see SCHEMA.md next to this file), so the same views
work whether the data comes from the ESP32 over USB, from a recorded .jsonl
file, or from the built-in simulator.

Two renderers share one targets[] list:
  * ceiling mount -> square top-down view (x/y, +-3 m by default)
  * side mount    -> sector view (azimuth +-55 deg, range 0-6 m by default)

Run standalone:
  python app_views.py --demo ceiling        # simulated walkers, square view
  python app_views.py --demo side           # simulated walkers, sector view
  python app_views.py --replay log.jsonl    # play back a recorded session
  python app_views.py --serial COM5         # live from the ESP32 (sends "jsonout on")
  python app_views.py --serial COM5 --send "tf 0201 14000000" --log run.jsonl   # side mount, record
  python app_views.py --demo side --shot out.png   # render one frame to PNG

No vendor or chip names appear in the UI: labels and logo are Kymopsis only.

The ESP32 converts the radar app's frames to schema v0; the same JSON drives this
PC view, the ESP32 web page and the smart-home entities.
"""
from __future__ import annotations

import argparse
import json
import math
import os
import sys
import time
from collections import deque

import numpy as np
import pyqtgraph as pg
from PySide6 import QtCore, QtGui, QtWidgets

try:  # optional, only for the logo
    from PySide6 import QtSvgWidgets
except Exception:  # pragma: no cover
    QtSvgWidgets = None

SCHEMA_VERSION = 0
HERE = os.path.dirname(os.path.abspath(__file__))
LOGO_CANDIDATES = [
    os.path.join(HERE, "assets", "kymopsis-logo-artistic-v4-light.svg"),
    os.path.normpath(os.path.join(HERE, "..", "..", "logo", "kymopsis-mark-artistic-v4-light.svg")),
]

# Kymopsis palette (light UI, matches the site)
C_BG = "#ffffff"
C_GRID = "#d5dbe3"
C_AXIS = "#5b6675"
C_ZONE = (37, 99, 235, 30)
C_ZONE_ON = (37, 99, 235, 90)
TRACK_COLORS = ["#2563eb", "#ea580c", "#16a34a", "#9333ea", "#db2777", "#0891b2", "#ca8a04", "#4b5563"]


def track_color(tid: int) -> str:
    return TRACK_COLORS[int(tid) % len(TRACK_COLORS)]


# ---------------------------------------------------------------------------
# Branding
# ---------------------------------------------------------------------------
class BrandHeader(QtWidgets.QWidget):
    """Kymopsis logo + product name + status text."""

    def __init__(self, subtitle: str = "Radar Base", parent=None):
        super().__init__(parent)
        lay = QtWidgets.QHBoxLayout(self)
        lay.setContentsMargins(8, 6, 8, 6)
        logo = next((p for p in LOGO_CANDIDATES if os.path.exists(p)), None)
        if logo and QtSvgWidgets is not None:
            w = QtSvgWidgets.QSvgWidget(logo)
            w.setFixedSize(36, 36)
            lay.addWidget(w)
        title = QtWidgets.QLabel(f"<b>Kymopsis</b> {subtitle}")
        title.setStyleSheet("font-size:16px; color:#111827;")
        lay.addWidget(title)
        lay.addStretch(1)
        self.status = QtWidgets.QLabel("")
        self.status.setStyleSheet("color:#5b6675;")
        lay.addWidget(self.status)

    def set_status(self, text: str):
        self.status.setText(text)


# ---------------------------------------------------------------------------
# Targets view: square (ceiling) or sector (side)
# ---------------------------------------------------------------------------
class TargetsView(pg.PlotWidget):
    """Top-down plot of tracked targets. x = lateral (m), y = depth (m).

    Ceiling mount: radar at the origin looking down, x/y are floor coordinates.
    Side mount: radar at the origin looking along +y, azimuth = atan2(x, y).
    """

    TRAIL = 25

    def __init__(self, mount: str = "ceiling", half_width: float = 3.0,
                 max_range: float = 6.0, fov_deg: float = 55.0, parent=None):
        super().__init__(parent=parent, background=C_BG)
        self.half_width = half_width
        self.max_range = max_range
        self.fov_deg = fov_deg
        self.mount = None
        self._bg_items: list = []
        self._zone_items: dict = {}
        self._zone_defs: list = []
        self._trails: dict[int, deque] = {}
        self._trail_items: dict[int, pg.PlotDataItem] = {}
        self._labels: dict[int, pg.TextItem] = {}
        self.setAspectLocked(True)
        self.hideButtons()
        self.setMenuEnabled(False)
        for ax in ("left", "bottom"):
            self.getAxis(ax).setPen(C_AXIS)
            self.getAxis(ax).setTextPen(C_AXIS)
        self.setLabel("bottom", "x", units="m")
        self.setLabel("left", "y", units="m")
        self.scatter = pg.ScatterPlotItem(size=16, pen=pg.mkPen("w", width=1.5))
        self.scatter.setZValue(10)
        self.addItem(self.scatter)
        self.set_mount(mount)

    # -- background -------------------------------------------------------
    def _clear_bg(self):
        for it in self._bg_items:
            self.removeItem(it)
        self._bg_items = []

    def _add_bg(self, item):
        item.setZValue(-10)
        self.addItem(item)
        self._bg_items.append(item)

    def set_mount(self, mount: str):
        if mount not in ("ceiling", "side"):
            raise ValueError("mount must be 'ceiling' or 'side'")
        if mount == self.mount:
            return
        self.mount = mount
        self._clear_bg()
        grid_pen = pg.mkPen(C_GRID, width=1)
        if mount == "ceiling":
            h = self.half_width
            for v in np.arange(-math.floor(h), math.floor(h) + 1, 1.0):
                self._add_bg(pg.PlotDataItem([v, v], [-h, h], pen=grid_pen))
                self._add_bg(pg.PlotDataItem([-h, h], [v, v], pen=grid_pen))
            box = np.array([[-h, -h], [h, -h], [h, h], [-h, h], [-h, -h]])
            self._add_bg(pg.PlotDataItem(box[:, 0], box[:, 1], pen=pg.mkPen(C_AXIS, width=2)))
            self._add_bg(pg.ScatterPlotItem([0], [0], symbol="s", size=12,
                                            brush=pg.mkBrush("#111827")))
            self.setRange(xRange=(-h, h), yRange=(-h, h), padding=0.04)
            self.setTitle("Ceiling mount - top view", color="#111827")
        else:
            r, fov = self.max_range, math.radians(self.fov_deg)
            for rr in np.arange(1.0, r + 0.01, 1.0):
                a = np.linspace(-fov, fov, 90)
                self._add_bg(pg.PlotDataItem(rr * np.sin(a), rr * np.cos(a), pen=grid_pen))
                t = pg.TextItem(f"{rr:.0f} m", color=C_AXIS, anchor=(0, 1))
                t.setPos(rr * math.sin(fov), rr * math.cos(fov))
                self._add_bg(t)
            for deg in range(-int(self.fov_deg), int(self.fov_deg) + 1, 15):
                a = math.radians(deg)
                self._add_bg(pg.PlotDataItem([0, r * math.sin(a)], [0, r * math.cos(a)], pen=grid_pen))
            a = np.concatenate([[0], np.linspace(-fov, fov, 120), [0]])
            rad = np.concatenate([[0], np.full(120, r), [0]])
            self._add_bg(pg.PlotDataItem(rad * np.sin(a), rad * np.cos(a), pen=pg.mkPen(C_AXIS, width=2)))
            self._add_bg(pg.ScatterPlotItem([0], [0], symbol="t1", size=14,
                                            brush=pg.mkBrush("#111827")))
            xr = r * math.sin(fov)
            self.setRange(xRange=(-xr, xr), yRange=(0, r), padding=0.04)
            self.setTitle("Side mount - sector view", color="#111827")
        self.set_zones(self._zone_defs)
        self.clear_targets()

    # -- zones ------------------------------------------------------------
    def set_zones(self, zones: list[dict]):
        """zones: [{"id":1,"x0":..,"y0":..,"x1":..,"y1":..,"occupied":bool}]"""
        for it in self._zone_items.values():
            self.removeItem(it)
        self._zone_items = {}
        self._zone_defs = list(zones or [])
        for z in self._zone_defs:
            if not all(k in z for k in ("x0", "y0", "x1", "y1")):
                continue
            rect = QtWidgets.QGraphicsRectItem(z["x0"], z["y0"], z["x1"] - z["x0"], z["y1"] - z["y0"])
            rect.setPen(pg.mkPen((37, 99, 235), width=1, style=QtCore.Qt.DashLine))
            rect.setBrush(pg.mkBrush(C_ZONE_ON if z.get("occupied") else C_ZONE))
            rect.setZValue(-5)
            self.addItem(rect)
            self._zone_items[z.get("id", len(self._zone_items))] = rect

    def set_zone_state(self, states: dict):
        for zid, occ in states.items():
            rect = self._zone_items.get(zid)
            if rect is not None:
                rect.setBrush(pg.mkBrush(C_ZONE_ON if occ else C_ZONE))

    # -- targets ----------------------------------------------------------
    def clear_targets(self):
        self.scatter.setData([], [])
        for d in (self._trail_items, self._labels):
            for it in d.values():
                self.removeItem(it)
            d.clear()
        self._trails.clear()

    def update_targets(self, targets: list[dict]):
        seen = set()
        spots = []
        for t in targets:
            tid = int(t.get("id", 0))
            x, y = float(t.get("x", 0.0)), float(t.get("y", 0.0))
            seen.add(tid)
            col = track_color(tid)
            spots.append({"pos": (x, y), "brush": pg.mkBrush(col), "data": tid})
            trail = self._trails.setdefault(tid, deque(maxlen=self.TRAIL))
            trail.append((x, y))
            if tid not in self._trail_items:
                item = pg.PlotDataItem(pen=pg.mkPen(col, width=2))
                item.setOpacity(0.5)
                self.addItem(item)
                self._trail_items[tid] = item
                lab = pg.TextItem(color="#111827", anchor=(-0.3, 1.2))
                self.addItem(lab)
                self._labels[tid] = lab
            arr = np.array(trail)
            self._trail_items[tid].setData(arr[:, 0], arr[:, 1])
            extra = ""
            if "z" in t:
                extra = f"  h {t['z']:.1f} m" if self.mount == "ceiling" else f"  z {t['z']:.1f} m"
            if self.mount == "side":
                rng = math.hypot(x, y)
                az = math.degrees(math.atan2(x, y))
                extra = f"  {rng:.1f} m {az:+.0f}°" + extra
            self._labels[tid].setText(f"#{tid}{extra}")
            self._labels[tid].setPos(x, y)
        self.scatter.setData(spots)
        for tid in list(self._trail_items):
            if tid not in seen:
                self.removeItem(self._trail_items.pop(tid))
                self.removeItem(self._labels.pop(tid))
                self._trails.pop(tid, None)


# ---------------------------------------------------------------------------
# Fall and vital-sign panels
# ---------------------------------------------------------------------------
class FallPanel(QtWidgets.QGroupBox):
    STATES = {"none": ("No person", "#5b6675"), "normal": ("Normal", "#16a34a"),
              "fall": ("FALL DETECTED", "#dc2626"), "lying": ("Lying still", "#ea580c")}

    def __init__(self, parent=None):
        super().__init__("Fall detection", parent)
        lay = QtWidgets.QVBoxLayout(self)
        self.state = QtWidgets.QLabel("-")
        self.state.setAlignment(QtCore.Qt.AlignCenter)
        self.state.setStyleSheet("font-size:22px; font-weight:600;")
        lay.addWidget(self.state)
        self.detail = QtWidgets.QLabel("")
        self.detail.setStyleSheet("color:#5b6675;")
        lay.addWidget(self.detail)

    def update_state(self, msg: dict):
        text, col = self.STATES.get(msg.get("state", ""), (str(msg.get("state", "-")), "#111827"))
        self.state.setText(text)
        self.state.setStyleSheet(f"font-size:22px; font-weight:600; color:{col};")
        bits = []
        if "height" in msg:
            bits.append(f"height {msg['height']:.2f} m")
        if "count" in msg:
            bits.append(f"people {msg['count']}")
        self.detail.setText("  ·  ".join(bits))


class VitalPanel(QtWidgets.QGroupBox):
    def __init__(self, seconds: int = 60, parent=None):
        super().__init__("Vital signs", parent)
        lay = QtWidgets.QVBoxLayout(self)
        row = QtWidgets.QHBoxLayout()
        self.hr = QtWidgets.QLabel("HR -")
        self.br = QtWidgets.QLabel("BR -")
        self.dist = QtWidgets.QLabel("")
        for w in (self.hr, self.br):
            w.setStyleSheet("font-size:20px; font-weight:600;")
            row.addWidget(w)
        row.addStretch(1)
        row.addWidget(self.dist)
        lay.addLayout(row)
        self.plot = pg.PlotWidget(background=C_BG)
        self.plot.addLegend(offset=(5, 5))
        self.plot.setLabel("bottom", "time", units="s")
        self.plot.setLabel("left", "per minute")
        self._t0 = None
        self._t, self._hr, self._br = (deque(maxlen=seconds * 4) for _ in range(3))
        self.c_hr = self.plot.plot(pen=pg.mkPen("#dc2626", width=2), name="heart")
        self.c_br = self.plot.plot(pen=pg.mkPen("#2563eb", width=2), name="breath")
        lay.addWidget(self.plot)

    def update_vital(self, msg: dict):
        now = msg.get("ts", time.time() * 1000) / 1000.0
        if self._t0 is None:
            self._t0 = now
        hr, br = msg.get("hr"), msg.get("br")
        self.hr.setText(f"HR {hr:.0f}" if hr is not None else "HR -")
        self.br.setText(f"BR {br:.0f}" if br is not None else "BR -")
        if "dist" in msg:
            self.dist.setText(f"distance {msg['dist']:.2f} m")
        self._t.append(now - self._t0)
        self._hr.append(np.nan if hr is None else hr)
        self._br.append(np.nan if br is None else br)
        t = np.array(self._t)
        self.c_hr.setData(t, np.array(self._hr), connect="finite")
        self.c_br.setData(t, np.array(self._br), connect="finite")


# ---------------------------------------------------------------------------
# App-mode page: routes schema v0 messages to the right view
# ---------------------------------------------------------------------------
class AppModePage(QtWidgets.QWidget):
    """Feed it schema v0 dicts with feed(msg)."""

    def __init__(self, mount: str = "ceiling", parent=None):
        super().__init__(parent)
        lay = QtWidgets.QVBoxLayout(self)
        lay.setContentsMargins(0, 0, 0, 0)
        self.header = BrandHeader("Radar Base")
        lay.addWidget(self.header)
        bar = QtWidgets.QHBoxLayout()
        bar.addWidget(QtWidgets.QLabel("Mount:"))
        self.mount_box = QtWidgets.QComboBox()
        self.mount_box.addItems(["ceiling", "side"])
        self.mount_box.setCurrentText(mount)
        self.mount_box.currentTextChanged.connect(self._on_mount)
        bar.addWidget(self.mount_box)
        bar.addStretch(1)
        self.info = QtWidgets.QLabel("")
        bar.addWidget(self.info)
        lay.addLayout(bar)
        split = QtWidgets.QSplitter(QtCore.Qt.Horizontal)
        self.view = TargetsView(mount)
        split.addWidget(self.view)
        side = QtWidgets.QWidget()
        sl = QtWidgets.QVBoxLayout(side)
        sl.setContentsMargins(0, 0, 0, 0)
        self.fall = FallPanel()
        self.vital = VitalPanel()
        sl.addWidget(self.fall)
        sl.addWidget(self.vital, 1)
        split.addWidget(side)
        split.setSizes([700, 360])
        lay.addWidget(split, 1)
        self._app = None
        self._n = 0
        self._t_rate = time.time()
        self._rate = 0.0
        self.fall.hide()
        self.vital.hide()

    def _on_mount(self, mount: str):
        self.view.set_mount(mount)

    def _set_app(self, app: str):
        if app == self._app:
            return
        self._app = app
        self.fall.setVisible(app == "fall")
        self.vital.setVisible(app == "vital")
        self.header.set_status(f"scenario: {app}")

    def feed(self, msg: dict):
        if msg.get("v", SCHEMA_VERSION) != SCHEMA_VERSION:
            return
        kind = msg.get("t")
        if "app" in msg:
            self._set_app(msg["app"])
        if "mount" in msg and msg["mount"] != self.mount_box.currentText():
            self.mount_box.setCurrentText(msg["mount"])
        if kind == "cap":
            self._set_app(msg.get("app", self._app or "?"))
            for m in msg.get("mounts", []):
                if m == self.mount_box.currentText():
                    break
            else:
                if msg.get("mounts"):
                    self.mount_box.setCurrentText(msg["mounts"][0])
        elif kind == "targets":
            self.view.update_targets(msg.get("targets", []))
            self._n += 1
            now = time.time()
            if now - self._t_rate >= 1.0:
                self._rate = self._n / (now - self._t_rate)
                self._n, self._t_rate = 0, now
            self.info.setText(f"{len(msg.get('targets', []))} target(s)  ·  {self._rate:.0f} fps")
        elif kind == "zones":
            if any("x0" in z for z in msg.get("zones", [])):
                self.view.set_zones(msg["zones"])
            else:
                self.view.set_zone_state({z["id"]: bool(z.get("occupied")) for z in msg.get("zones", [])})
                if not self.view._zone_defs:   # firmware areas without geometry: list them in the status
                    occ = [str(z["id"]) for z in msg.get("zones", []) if z.get("occupied")]
                    self.header.set_status(f"scenario: {self._app}  ·  areas occupied: {', '.join(occ) or 'none'}")
        elif kind == "fall":
            self.fall.update_state(msg)
        elif kind == "vital":
            self.vital.update_vital(msg)


# ---------------------------------------------------------------------------
# Sources: simulator and .jsonl replay
# ---------------------------------------------------------------------------
class Simulator:
    """Generates plausible schema v0 messages for UI work without hardware."""

    def __init__(self, mount: str = "ceiling", app: str = "hd3d"):
        self.mount, self.app, self.t0 = mount, app, time.time()

    def messages(self):
        t = time.time() - self.t0
        ts = int(time.time() * 1000)
        out = []
        if t < 0.06:
            out.append({"v": 0, "t": "cap", "app": self.app, "mounts": [self.mount]})
            if self.mount == "ceiling":
                out.append({"v": 0, "t": "zones", "zones": [
                    {"id": 1, "x0": -2.5, "y0": 0.5, "x1": -0.5, "y1": 2.5},
                    {"id": 2, "x0": 0.5, "y0": -2.5, "x1": 2.5, "y1": -0.5}]})
        tg = []
        if self.mount == "ceiling":
            tg.append({"id": 1, "x": 1.8 * math.cos(0.4 * t), "y": 1.8 * math.sin(0.4 * t), "z": 1.7})
            tg.append({"id": 2, "x": -1.5 + 0.6 * math.sin(0.25 * t), "y": 1.5 + 0.4 * math.cos(0.3 * t), "z": 1.1})
        else:
            tg.append({"id": 1, "x": 2.2 * math.sin(0.3 * t), "y": 3.0 + 1.2 * math.cos(0.2 * t), "z": 0.2})
            tg.append({"id": 3, "x": -1.0, "y": 1.6 + 0.2 * math.sin(t), "z": -0.3})
        out.append({"v": 0, "t": "targets", "ts": ts, "targets": tg})
        if self.mount == "ceiling":
            occ1 = -2.5 < tg[1]["x"] < -0.5 and 0.5 < tg[1]["y"] < 2.5
            occ2 = 0.5 < tg[0]["x"] < 2.5 and -2.5 < tg[0]["y"] < -0.5
            out.append({"v": 0, "t": "zones", "zones": [{"id": 1, "occupied": occ1}, {"id": 2, "occupied": occ2}]})
        if self.app == "fall":
            out.append({"v": 0, "t": "fall", "state": "fall" if int(t) % 20 > 15 else "normal",
                        "height": 1.6, "count": len(tg)})
        if self.app == "vital":
            out.append({"v": 0, "t": "vital", "ts": ts, "hr": 68 + 4 * math.sin(0.1 * t),
                        "br": 15 + 2 * math.sin(0.05 * t), "dist": 1.2})
        return out


def iter_jsonl(path: str):
    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if line.startswith("{"):
                try:
                    yield json.loads(line)
                except json.JSONDecodeError:
                    continue


class SerialSource:
    """Schema v0 lines live from the ESP32 USB console (binary packets and log lines are skipped)."""

    def __init__(self, port: str, send=(), log_path: str | None = None):
        import queue
        import threading
        import serial
        self.q = queue.Queue()
        self.ser = serial.Serial()
        self.ser.port, self.ser.baudrate, self.ser.timeout = port, 115200, 0.05
        self.ser.dtr = False   # do not reset the ESP32 on open
        self.ser.rts = False
        self.ser.open()
        self.log = open(log_path, "a", encoding="utf-8") if log_path else None
        self.lines = 0
        self.stop = threading.Event()
        threading.Thread(target=self._run, args=(list(send),), daemon=True).start()

    def _run(self, send):
        time.sleep(0.3)
        for cmd in ["jsonout on"] + send:
            self.ser.write((cmd + "\r\n").encode())
            time.sleep(0.2)
        buf = b""
        while not self.stop.is_set():
            buf += self.ser.read(65536)
            *done, buf = buf.split(b"\n")
            for raw in done:
                i = raw.find(b'{"v":')
                if i < 0:
                    continue
                try:
                    msg = json.loads(raw[i:].decode("utf-8", "replace"))
                except json.JSONDecodeError:
                    continue
                self.lines += 1
                if self.log:
                    self.log.write(json.dumps(msg) + "\n")
                self.q.put(msg)

    def messages(self):
        out = []
        while not self.q.empty():
            out.append(self.q.get_nowait())
        return out

    def close(self):
        self.stop.set()
        try:
            self.ser.write(b"jsonout off\r\n")
            time.sleep(0.1)
            self.ser.close()
        except Exception:
            pass
        if self.log:
            self.log.close()


def main(argv=None):
    ap = argparse.ArgumentParser(description="Kymopsis Radar Base app-mode views")
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument("--demo", choices=["ceiling", "side"])
    g.add_argument("--replay", metavar="FILE.jsonl")
    g.add_argument("--serial", metavar="COMx", help="live schema v0 lines from the ESP32 USB console")
    ap.add_argument("--send", action="append", default=[], help="console command after connecting (repeatable)")
    ap.add_argument("--log", metavar="FILE.jsonl", help="with --serial: append every received message")
    ap.add_argument("--app", default="hd3d", choices=["hd3d", "fall", "vital"])
    ap.add_argument("--fps", type=float, default=20.0)
    ap.add_argument("--shot", metavar="PNG", help="render, save a screenshot after --shot-after s and exit")
    ap.add_argument("--shot-after", type=float, default=2.5, metavar="S")
    a = ap.parse_args(argv)

    pg.setConfigOptions(antialias=True, foreground=C_AXIS)
    qapp = QtWidgets.QApplication.instance() or QtWidgets.QApplication(sys.argv)
    page = AppModePage(a.demo or "ceiling")
    page.setWindowTitle("Kymopsis Radar Base")
    page.resize(1100, 700)
    page.show()

    if a.demo:
        sim = Simulator(a.demo, a.app)
        src = lambda: sim.messages()
    elif a.serial:
        ser = SerialSource(a.serial, a.send, a.log)
        src = ser.messages
        qapp.aboutToQuit.connect(ser.close)
    else:
        it = iter_jsonl(a.replay)
        def src():
            try:
                return [next(it)]
            except StopIteration:
                timer.stop()
                return []

    def tick():
        for m in src():
            page.feed(m)

    timer = QtCore.QTimer()
    timer.timeout.connect(tick)
    timer.start(int(1000 / a.fps))
    if a.shot:
        def snap():
            page.grab().save(a.shot)
            qapp.quit()
        QtCore.QTimer.singleShot(int(a.shot_after * 1000), snap)
    return qapp.exec()


if __name__ == "__main__":
    sys.exit(main())
