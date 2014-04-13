from donut import cruller

import matplotlib.pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches

from math import log, floor


#need to add this in notebooks:
#%matplotlib inline

#for adding text later: http://matplotlib.org/1.3.1/users/annotations_intro.html

def DrawPolygon(p):
    verts = p.VertexList()
    codes = [Path.MOVETO] + [Path.LINETO for i in range(len(verts)-2)] + [Path.CLOSEPOLY]
    path = Path(verts, codes)

    fig = plt.figure()
    ax = fig.add_subplot(111)
    patch = patches.PathPatch(path, facecolor='orange', lw=2)
    ax.add_patch(patch)
    (l, r, b, t) = (p.LeftExtreme(), p.RightExtreme(), p.BottomExtreme(), p.TopExtreme())
    xpad = (r-l)*0.1
    ypad = (t-b)*0.1
    ax.set_xlim(l-xpad, r+xpad)
    ax.set_ylim(b-ypad, t+ypad)
    plt.show()

def DrawPolygons(polylist):
    fig = plt.figure()
    ax = fig.add_subplot(111)
    (l, r, b, t) = (0, 0, 0, 0)
    for p in polylist:
        verts = p.VertexList()
        codes = [Path.MOVETO] + [Path.LINETO for i in range(len(verts)-2)] + [Path.CLOSEPOLY]
        path = Path(verts, codes)
        (newl, newr, newb, newt) = (p.LeftExtreme(), p.RightExtreme(), p.BottomExtreme(), p.TopExtreme())
        l = min(l, newl)
        r = max(r, newr)
        b = min(b, newb)
        t = max(t, newt)

        patch = patches.PathPatch(path, facecolor='orange', lw=2)
        ax.add_patch(patch)

    xpad = (r-l)*0.1
    ypad = (t-b)*0.1
    ax.set_xlim(l-xpad, r+xpad)
    ax.set_ylim(b-ypad, t+ypad)
    plt.show()

def HeatMap(v):
    r = [0, 1, 1]
    g = [0, 1, 0]
    b = [1, 1, 0]

    l = len(r) - 1.0
    width = 1.0/l
    closest = floor(v*l)
    if closest == l:
        return (r[-1], g[-1], b[-1])
    extra = (v*l - closest)/l
    rn = (r[closest+1]*extra + r[closest]*(width-extra))/width
    gn = (g[closest+1]*extra + g[closest]*(width-extra))/width
    bn = (b[closest+1]*extra + b[closest]*(width-extra))/width
    if rn > 1: rn = 1
    if gn > 1: gn = 1
    if bn > 1: bn = 1
    if rn < 0: rn = 0
    if gn < 0: gn = 0
    if bn < 0: bn = 0
    return (rn,gn,bn)

def DrawKeyboard(k, wordlist = None, logarithmic = False, pmin = None, pmax = None, inputvector = None):
    fig = plt.figure()
    ax = fig.add_subplot(111)
    (l, r, b, t) = (0, 0, 0, 0)

    d = k.PolygonDict()

    frequencies = None
    if wordlist != None:
        rawfrequencies = [wordlist.LetterOccurances(c)/wordlist.TotalLetterOccurances() for c in d.keys()]
        frequencies = [f for f in rawfrequencies if f > 0]
        if pmin == None:
            pmin = min(frequencies)
        if pmax == None:
            pmax = max(frequencies)
    for c in d.keys():
        p = d[c]
        verts = p.VertexList()
        codes = [Path.MOVETO] + [Path.LINETO for i in range(len(verts)-2)] + [Path.CLOSEPOLY]
        path = Path(verts, codes)
        (newl, newr, newb, newt) = (p.LeftExtreme(), p.RightExtreme(), p.BottomExtreme(), p.TopExtreme())
        l = min(l, newl)
        r = max(r, newr)
        b = min(b, newb)
        t = max(t, newt)

        facecolor = 'orange'
        if wordlist != None:
            frequency = wordlist.LetterOccurances(c)/wordlist.TotalLetterOccurances()
            value = 0
            if frequency >= pmin:
                if logarithmic:
                    value = (log(frequency) - log(pmin))/(log(pmax) - log(pmin))
                else:
                    value = (frequency - pmin)/(pmax - pmin)
            facecolor = HeatMap(value)

        patch = patches.PathPatch(path, facecolor=facecolor, lw=2)
        ax.add_patch(patch)

        ax.text(newl + 0.2*(newr-newl),newb + 0.2*(newt-newb), c, fontsize=18)
    xpad = (r-l)*0.1
    ypad = (t-b)*0.1
    ax.set_xlim(l-xpad, r+xpad)
    ax.set_ylim(b-ypad, t+ypad)

    if inputvector != None:
        points = inputvector.PointList()
        times = [t[2] for t in points]
        tmin = min(times)
        tmax = max(times)
        for i, p in enumerate(points):
            color = (0.8, 0.5, 0.5)
            if tmax > tmin:
                color = (0.8, 1.0 - ((p[2]-tmin)/(tmax-tmin)), 0.5)
            ax.plot(p[0], p[1], color=color, markersize=10, marker='o')

    plt.show()

def StandardQwerty():
    width = 1.0
    height = 1.5
    gap = 0.2
    startx = 0.0
    starty = 0.0
    l = []
    for i in range(10):
        p = cruller.Polygon()
        p.AddVertex(startx + float(i)*(width+gap), starty)
        p.AddVertex(startx + float(i)*(width+gap) + width, starty)
        p.AddVertex(startx + float(i)*(width+gap) + width, starty-height)
        p.AddVertex(startx + float(i)*(width+gap), starty-height)
        l.append(p)
    startx += 0.5*width
    starty -= height + gap
    for i in range(9):
        p = cruller.Polygon()
        p.AddVertex(startx + float(i)*(width+gap), starty)
        p.AddVertex(startx + float(i)*(width+gap) + width, starty)
        p.AddVertex(startx + float(i)*(width+gap) + width, starty-height)
        p.AddVertex(startx + float(i)*(width+gap), starty-height)
        l.append(p)
    startx += width + gap
    starty -= height + gap
    for i in range(7):
        p = cruller.Polygon()
        p.AddVertex(startx + float(i)*(width+gap), starty)
        p.AddVertex(startx + float(i)*(width+gap) + width, starty)
        p.AddVertex(startx + float(i)*(width+gap) + width, starty-height)
        p.AddVertex(startx + float(i)*(width+gap), starty-height)
        l.append(p)
    starty -= height + gap
    p = cruller.Polygon()
    p.AddVertex(startx + float(6)*(width+gap), starty)
    p.AddVertex(startx + float(6)*(width+gap) + width, starty)
    p.AddVertex(startx + float(6)*(width+gap) + width, starty-height)
    p.AddVertex(startx + float(6)*(width+gap), starty-height)
    l.append(p)

    qwerty = 'qwertyuiopasdfghjklzxcvbnm.'
    k = cruller.Keyboard()
    for i in range(27):
        k.AddKey(qwerty[i], l[i])
    return k
