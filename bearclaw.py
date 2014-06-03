from donut import cruller
from random import shuffle, choice

import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches
import collections
import matplotlib.cm as cm
from matplotlib.pylab import savefig

from math import log, floor, sqrt, exp
from copy import deepcopy

from itertools import cycle

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

def DrawKeyboard(k, wordlist = None, logarithmic = False, pmin = None, pmax = None, inputvector = None,
                t9 = False, letters = True, frequencymap = None, oneletter = None, colormap = mpl.cm.cool, figsize = None, saveas = None):
    fig = None
    if figsize != None:
        fig = plt.figure(figsize=figsize)
    else:
        fig = plt.figure()
    colored = False
    ax = None
    gs = None
    if wordlist != None or frequencymap != None:
        colored = True
        ax = fig.add_axes([0.0, 0.0, 0.93, 1.0])
    else:
        ax = fig.add_subplot(111)

    (l, r, b, t) = (0, 0, 0, 0)

    d = k.PolygonDict()

    frequencies, norm, scalarmap = None, None, None
    if colored:
        rawfrequencies = None
        if wordlist != None:
            rawfrequencies = [wordlist.LetterOccurances(c)/wordlist.TotalLetterOccurances() for c in d.keys()]
        if frequencymap != None:
            rawfrequencies = frequencymap
        frequencies = [f for f in rawfrequencies if f > 0]
        if pmin == None:
            pmin = min(frequencies)
        if pmax == None:
            pmax = max(frequencies)
        format = None
        if logarithmic:
            norm = mpl.colors.LogNorm(vmin=pmin, vmax=pmax)
            format = mpl.ticker.LogFormatter(10, labelOnlyBase=False)
        else:
            norm = mpl.colors.Normalize(vmin=pmin, vmax=pmax)
        scalarmap = mpl.cm.ScalarMappable(norm=norm, cmap=colormap)
        ax2 = fig.add_axes([0.95, 0.0, 0.05, 1.0])
        cb1 = mpl.colorbar.ColorbarBase(ax2, cmap=colormap, norm=norm, format = format, orientation='vertical')

    for i, c in enumerate(k.OrderedKeyList()):
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
        if colored:
            frequency = None
            if wordlist != None:
                frequency = wordlist.LetterOccurances(c)/wordlist.TotalLetterOccurances()
            elif frequencymap != None:
                frequency = frequencymap[i]

            facecolor = scalarmap.to_rgba(frequency)

        patch = patches.PathPatch(path, facecolor=facecolor, lw=2)
        ax.add_patch(patch)

        t9shift = 0.0
        if t9:
            idx = i % 3
            t9shift = (newr-newl)*0.25
            if i == 25 or i == 18:
                t9shift *= 3
            else:
                if i > 18:
                    idx = (i-1) % 3
                t9shift *= idx
            t9shift -= (newr-newl)*0.15

        if letters:
            if oneletter != None:
                c = oneletter
            patheffects = None
            if colored:
                from matplotlib import patheffects
                patheffects = [patheffects.withStroke(linewidth=3,foreground="w")]
            ax.text(newl + 0.2*(newr-newl) + t9shift,newb + 0.2*(newt-newb), c, fontsize=18, path_effects=patheffects)
    xpad = (r-l)*0.1
    ypad = (t-b)*0.1
    ax.set_xlim(l-xpad, r+xpad)
    ax.set_ylim(b-ypad, t+ypad)

    if inputvector != None:
        color = (0.0,0.0,0.0)
        colorlist = [(0.0,0.0,0.0)]
        finalcolor = [(1.0,1.0,1.0)]

        #Test if inputvector is a list or not
        if not isinstance(inputvector,collections.Iterable):
            inputvector = [inputvector]
            color = [(.8, 0.5, 0.5)]
        else:
            colorlist = [(1.0,0.0,0.0),(0.0,100/255.,0.0),(0.0,0.0,1.0),
                         (1.0,1.0,0.0),(1.0,0.0,1.0),(0.0,1.0,1.0)]
            finalcolor =[(1.0,128/255.,128/255.),(89/255.,189/255.,89/255.),(148/255.,148/255.,1.0),
                         (1.0,1.0,175/255.),(1.0,171/255.,1.0),(179/255.,1.0,1.0)]
            color = colorlist

        iEntry = 0
        for entry in inputvector:

            points = entry.PointList()
            times = [t[2] for t in points]
            tmin = min(times)
            tmax = max(times)
            for i, p in enumerate(points):
                if tmax > tmin:
                    colortmp = (0.0,0.0,0.0)
                    if len(colorlist) > 1:
                        colortmp = (color[iEntry][0]+((finalcolor[iEntry][0]-color[iEntry][0])/(tmax-tmin))*p[2],
                                    color[iEntry][1]+((finalcolor[iEntry][1]-color[iEntry][1])/(tmax-tmin))*p[2],
                                    color[iEntry][2]+((finalcolor[iEntry][2]-color[iEntry][2])/(tmax-tmin))*p[2])
                    else :
                        colortmp = (color[iEntry][0],1.0 - ((p[2]-tmin)/(tmax-tmin)), color[iEntry][2])

                    ax.plot(p[0], p[1],color=colortmp, markersize=10, marker='o')

            iEntry = iEntry+1
            if iEntry >= len(colorlist):
                iEntry =0;

    if saveas:
        savefig(saveas)
    plt.show()

  #          for i, p in enumerate(points):
  #              color = (0.8, 0.5, 0.5)
  #              if tmax > tmin:
  #                  color = (0.8, 1.0 - ((p[2]-tmin)/(tmax-tmin)), 0.5)
  #                  ax.plot(p[0], p[1], color=color, markersize=10, marker='o')

def MakeStandardKeyboard(alphabetStr='qwertyuiopasdfghjklzxcvbnm.'):
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
    for i in range(len(alphabetStr)):
        k.AddKey(alphabetStr[i], l[i])
    return k

def MakeHexagonalKeyboard(alphabetStr='qwertyuiopasdfghjklzxcvbnm.', scale=0.9):
    shift = (sqrt(3), -1.5)
    a, b, c = scale, scale/2.0, sqrt(3)*scale/2
    h = cruller.Polygon()
    h.AddVertex(0,-a)
    h.AddVertex(c,-b)
    h.AddVertex(c,b)
    h.AddVertex(0,a)
    h.AddVertex(-c,b)
    h.AddVertex(-c,-b)

    l = []
    for j in range(3):
        oldp = h
        for i in range(10-j):
            p = deepcopy(oldp)
            p.Translate(shift[0],0)
            l.append(p)
            oldp = p
        h.Translate(0.5*shift[0], shift[1])

    k = cruller.Keyboard()
    for i in range(27):
        k.AddKey(alphabetStr[i], l[i])
    return k

#Left off the '.' because I think that was it's own key
def MakeT9Keyboard(alphabetStr='abcdefghijklmnopqrstuvwxyz', scale=0.9):
    shift = (1, -1)
    a, b, c = scale, scale/2.0, sqrt(3)*scale/2
    h = cruller.Polygon()
    h.AddVertex(0,0)
    h.AddVertex(1,0)
    h.AddVertex(1,-1)
    h.AddVertex(0,-1)

    l = []
    for j in range(3):
        oldp = h
        for i in range(3):
            p = deepcopy(oldp)
            p.Translate(shift[0],0)
            if not (i==0 and j==0):
                for k in range(3):
                    l.append(p)
                if j==2 and (i!=1):
                    l.append(p)
            oldp = p
        h.Translate(0, shift[1])

    k = cruller.Keyboard()
    for i in range(26):
        k.AddKey(alphabetStr[i], l[i])

    return k

def RandomKeyboard(keyboard = None):
    if keyboard == None:
        keyboard = MakeStandardKeyboard()
    d = keyboard.PolygonDict()
    letters = keyboard.OrderedKeyList()
    polygons = [keyboard.GetKey(k) for k in letters]

    indices = list(range(len(letters)))
    shuffle(indices)
    newk = cruller.Keyboard()
    for i, idx in enumerate(indices):
        newk.AddKey(letters[idx], polygons[i])

    return newk

def StandardQwerty():
    return MakeStandardKeyboard()

def GetNRandomKeyboards(n, keyboard = None):
    kList = []

    for i in range(0,n):
        kList.append(RandomKeyboard(keyboard))

    return kList

def RandomSwap(keyboard, Nswaps = 1):
    d = keyboard.PolygonDict()
    letters = keyboard.OrderedKeyList()
    polygons = [keyboard.GetKey(k) for k in letters]

    indices = list(range(len(letters)))

    for swap in range(Nswaps):
        c = (choice(indices), choice(indices))
#        print("choices:", c)
        indices[c[0]], indices[c[1]] = indices[c[1]], indices[c[0]]

    newk = cruller.Keyboard()
    for i, idx in enumerate(indices):
        newk.AddKey(letters[idx], polygons[i])

    return newk

