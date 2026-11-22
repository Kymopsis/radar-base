"""Build radar.kymopsis.com (static, multilingual) into site/public/.

Sources: site/src/<lang>/<page>.md (English is the reference; a page missing in a language falls back to English
with a note). Also copies the web app (../esp32-flasher-test/main/app.html -> public/app/), its language packs and
the logo, so the hosted app works over USB (Web Serial). No vendor or chip names belong in these sources.

usage: python site/build.py
"""
import html
import json
import re
import shutil
from pathlib import Path

HERE = Path(__file__).resolve().parent
SRC, OUT = HERE / "src", HERE / "public"
APP = next(p for p in (HERE.parent / "esp32-flasher-test" / "main", HERE.parent / "firmware" / "main") if p.exists())
LOGO = HERE.parent / "pc-gui" / "assets" / "kymopsis-logo-artistic-v4-light.svg"
LANGS = {"en": "English", "zh-CN": "简体中文", "zh-TW": "繁體中文", "ja": "日本語", "ko": "한국어", "de": "Deutsch",
         "fr": "Français", "es": "Español", "pt": "Português", "it": "Italiano", "ru": "Русский"}
ORDER = ["index", "getting-started", "usb", "wifi", "hotspot", "home-assistant", "esphome", "raw-data", "json-api",
         "devkit-flashing", "downloads", "faq"]
UI = json.loads((HERE / "ui.json").read_text(encoding="utf-8"))   # site chrome strings (en, zh-CN)
for _p in SRC.glob("*/_ui.json"):                                  # per-language overrides from translators
    UI.setdefault(_p.parent.name, {}).update(json.loads(_p.read_text(encoding="utf-8")))


def inline(s):
    s = html.escape(s, quote=False)
    s = re.sub(r"`([^`]+)`", r"<code>\1</code>", s)
    s = re.sub(r"\*\*([^*]+)\*\*", r"<strong>\1</strong>", s)

    def link(m):
        t, u = m.group(1), m.group(2)
        if not re.match(r"^[a-z]+:|^/|^#", u):
            u = "../" if u.startswith("app") else u + ".html"
        return f'<a href="{u}">{t}</a>'
    return re.sub(r"\[([^\]]+)\]\(([^)]+)\)", link, s)


def cards(lines):
    out = ['<div class="cards">']
    for l in lines:
        t, d, u = [x.strip() for x in l.lstrip("- ").split("|")]
        out.append(f'<a class="card" href="{u}.html"><b>{inline(t.strip("*"))}</b><span>{inline(d)}</span></a>')
    return "\n".join(out + ["</div>"])


def md(text):
    """Small Markdown subset: headings, paragraphs, lists, tables, code, quotes, [[cards]], [[diagram]]."""
    lines, out, i = text.split("\n"), [], 0
    while i < len(lines):
        l = lines[i]
        if not l.strip():
            i += 1
        elif l.startswith("```"):
            j = i + 1
            while not lines[j].startswith("```"):
                j += 1
            out.append("<pre><code>" + html.escape("\n".join(lines[i + 1:j])) + "</code></pre>")
            i = j + 1
        elif l.strip() == "[[cards]]":
            j = lines.index("[[/cards]]", i)
            out.append(cards([x for x in lines[i + 1:j] if x.strip()]))
            i = j + 1
        elif l.strip() in ("[[diagram]]", "[[wiring]]"):
            out.append((HERE / ("diagram.svg" if "diagram" in l else "wiring.svg")).read_text(encoding="utf-8"))
            i += 1
        elif m := re.match(r"^(#{1,3}) (.*)", l):
            n = len(m.group(1))
            slug = re.sub(r"[^\w]+", "-", m.group(2).lower()).strip("-")
            out.append(f'<h{n} id="{slug}">{inline(m.group(2))}</h{n}>')
            i += 1
        elif l.startswith("|"):
            rows = []
            while i < len(lines) and lines[i].startswith("|"):
                rows.append([c.strip() for c in lines[i].strip("|").split("|")])
                i += 1
            h, body = rows[0], [r for r in rows[2:]]
            out.append('<div class="tw"><table><thead><tr>' + "".join(f"<th>{inline(c)}</th>" for c in h) + "</tr></thead><tbody>" +
                       "".join("<tr>" + "".join(f"<td>{inline(c)}</td>" for c in r) + "</tr>" for r in body) + "</tbody></table></div>")
        elif l.startswith("> "):
            out.append(f'<p class="note">{inline(l[2:])}</p>')
            i += 1
        elif re.match(r"^(- |\d+\. )", l):
            tag = "ol" if l[0].isdigit() else "ul"
            items = []
            while i < len(lines) and (re.match(r"^(- |\d+\. )", lines[i]) or (lines[i].startswith("   ") and items)):
                if re.match(r"^(- |\d+\. )", lines[i]):
                    items.append(re.sub(r"^(- |\d+\. )", "", lines[i]))
                else:
                    items[-1] += " " + lines[i].strip()
                i += 1
            out.append(f"<{tag}>" + "".join(f"<li>{inline(x)}</li>" for x in items) + f"</{tag}>")
        else:
            para = []
            while i < len(lines) and lines[i].strip() and not re.match(r"^(#|\||- |\d+\. |```|> |\[\[)", lines[i]):
                para.append(lines[i].strip())
                i += 1
            out.append(f"<p>{inline(' '.join(para))}</p>")
    return "\n".join(out)


def front(text):
    meta = {}
    if text.startswith("---"):
        head, text = text[3:].split("\n---", 1)
        for l in head.strip().splitlines():
            k, v = l.split(":", 1)
            meta[k.strip()] = v.strip()
    return meta, text.lstrip("\n")


def load(lang, page):
    p = SRC / lang / f"{page}.md"
    fb = not p.exists()
    meta, body = front((SRC / "en" / f"{page}.md" if fb else p).read_text(encoding="utf-8"))
    return meta, body, fb


def page_html(lang, page, langs_built):
    u = {**UI["en"], **UI.get(lang, {})}
    meta, body, fb = load(lang, page)
    groups = {}
    for pg in ORDER[1:]:
        m, _, _ = load(lang, pg)
        groups.setdefault(m.get("group", ""), []).append((pg, m.get("nav", pg)))
    nav = "".join(f'<div class="ng"><div class="nh">{html.escape(u.get("group." + g, g))}</div>' +
                  "".join(f'<a href="{p}.html"{" class=on" if p == page else ""}>{html.escape(n)}</a>' for p, n in items) + "</div>"
                  for g, items in groups.items())
    opts = "".join(f'<option value="{c}"{" selected" if c == lang else ""}>{LANGS[c]}</option>' for c in langs_built)
    note = f'<p class="note">{html.escape(u["fallback"])}</p>' if fb and lang != "en" else ""
    return f"""<!doctype html><html lang="{lang}"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>{html.escape(meta.get('title', 'Radar Base'))} · Kymopsis Radar Base</title>
<link rel="icon" href="../favicon.png"><link rel="stylesheet" href="../site.css">
{''.join(f'<link rel="alternate" hreflang="{c}" href="../{c}/{page}.html">' for c in langs_built)}
</head><body>
<header class="top"><div class="in"><a class="brand" href="index.html"><img src="../logo.svg" alt="Kymopsis"><span>Radar Base</span></a>
<div class="sp"></div><a class="btn pri" href="../">{html.escape(u['open_app'])}</a>
<select class="lang" aria-label="Language" onchange="location.href='../'+this.value+'/{page}.html'">{opts}</select></div></header>
<div class="wrap"><nav class="side"><a href="index.html"{' class=on' if page == 'index' else ''}>{html.escape(u['overview'])}</a>{nav}</nav>
<main>{note}{md(body)}</main></div>
<footer><span>© Kymopsis</span><span>{html.escape(u['no_cloud'])}</span><a href="https://kymopsis.com">kymopsis.com</a></footer>
<script>try{{localStorage.setItem('kym.lang','{lang}')}}catch(e){{}}</script>
</body></html>"""


ROOT_INDEX = """<!doctype html><html><head><meta charset="utf-8"><title>Kymopsis Radar Base</title>
<meta name="viewport" content="width=device-width,initial-scale=1"><script>
var L=%s,s=null;try{s=localStorage.getItem('kym.lang')}catch(e){}
function pick(){if(s&&L.indexOf(s)>=0)return s;var ls=navigator.languages||[navigator.language];
for(var i=0;i<ls.length;i++){var l=ls[i];if(/^zh/i.test(l))return /^zh-(TW|HK|MO|Hant)/i.test(l)?'zh-TW':'zh-CN';
if(L.indexOf(l)>=0)return l;if(L.indexOf(l.split('-')[0])>=0)return l.split('-')[0]}return 'en'}
var t=pick();if(L.indexOf(t)<0)t='en';location.replace(t+'/index.html')</script></head>
<body><a href="en/index.html">Kymopsis Radar Base</a></body></html>"""


def main():
    if OUT.exists():   # delete old files but keep folders (Windows may hold an open folder)
        for f in sorted(OUT.rglob("*"), reverse=True):
            if f.is_file():
                f.unlink()
            else:
                try:
                    f.rmdir()
                except OSError:
                    pass
    OUT.mkdir(exist_ok=True)
    built = [c for c in LANGS if (SRC / c).exists()]
    for lang in built:
        (OUT / lang).mkdir(exist_ok=True)
        for page in ORDER:
            (OUT / lang / f"{page}.html").write_text(page_html(lang, page, built), encoding="utf-8")
    # the site's home page is the web app itself (hosted mode: USB, install, open on my network, guides)
    app = (APP / "app.html").read_text(encoding="utf-8")
    head = ('<link rel="icon" href="/favicon.png"><meta name="description" content="Kymopsis 60 GHz Radar Base: '
            'web app over USB, open firmware, Home Assistant, ESPHome and JSON guides. No cloud.">')
    app = app.replace("</title>", "</title>" + head, 1) if "</title>" in app else app.replace("<head>", "<head>" + head, 1)
    (OUT / "index.html").write_text(app, encoding="utf-8")
    shutil.copy(HERE / "site.css", OUT / "site.css")
    shutil.copy(LOGO, OUT / "logo.svg")
    shutil.copy(HERE.parent / "pc-gui" / "assets" / "kymopsis-mark-favicon-192.png", OUT / "favicon.png")
    (OUT / "app").mkdir(exist_ok=True)   # old links to /app/ go to the home page
    (OUT / "app" / "index.html").write_text('<!doctype html><meta charset="utf-8"><meta http-equiv="refresh" content="0; url=../">'
                                            '<link rel="canonical" href="https://radar.kymopsis.com/"><a href="../">Radar Base</a>',
                                            encoding="utf-8")
    shutil.copytree(APP / "i18n", OUT / "i18n", dirs_exist_ok=True)
    (OUT / "CNAME").write_text("radar.kymopsis.com\n", encoding="utf-8")
    (OUT / ".nojekyll").write_text("", encoding="utf-8")
    n = sum(1 for _ in OUT.rglob("*.html"))
    print(f"built {n} pages for {built} into {OUT}")


if __name__ == "__main__":
    main()
