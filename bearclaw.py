from donut import cruller
from random import shuffle

import matplotlib.pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches
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


def DrawKeyboard(k):
    fig = plt.figure()
    ax = fig.add_subplot(111)
    (l, r, b, t) = (0, 0, 0, 0)

    d = k.PolygonDict()

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

        patch = patches.PathPatch(path, facecolor='orange', lw=2)
        ax.add_patch(patch)

        ax.text(newl + 0.2*(newr-newl),newb + 0.2*(newt-newb), c, fontsize=18)

    xpad = (r-l)*0.1
    ypad = (t-b)*0.1
    ax.set_xlim(l-xpad, r+xpad)
    ax.set_ylim(b-ypad, t+ypad)
    plt.show()


def MakeKeyboard(alphabetStr='qwertyuiopasdfghjklzxcvbnm.'):
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

    k = cruller.Keyboard()
    for i in range(27):
        k.AddKey(alphabetStr[i], l[i])
    return k


def RandomKeyboard():
    l = list('qwertyuiopasdfghjklzxcvbnm.')
    shuffle(l)
    rstr = ''.join(l)
    return MakeKeyboard(rstr)


def StandardQwerty():
    return MakeKeyboard()



def GetNRandomKeyboards(n):
    kList = []

    for i in range(0,n):
        kList.append(RandomKeyboard())

    return kList





