# Results viewer (PC)

```
pip install pyside6 pyqtgraph numpy pyserial
python app_views.py --serial COM5          # live JSON results from Radar Base over USB
python app_views.py --demo ceiling         # without hardware
```

Shows people positions (ceiling or wall view), areas, fall state, breathing and heart rate from the JSON results
(format in `SCHEMA.md`). It is also a small example of reading Radar Base results from your own code.
