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
