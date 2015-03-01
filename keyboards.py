from dodona import core
from random import shuffle, choice

import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches
import collections
import matplotlib.cm as cm
from matplotlib.colors import LinearSegmentedColormap
from matplotlib.pylab import savefig

from math import log, floor, sqrt, exp
from copy import deepcopy

from itertools import cycle

#need to add this in notebooks:
#%matplotlib inline

#for adding text later: http://matplotlib.org/1.3.1/users/annotations_intro.html

def DrawPolygon(p):
    DrawPolygons(p)

def DrawPolygons(polylist, facecolor = 'lightblue', figsize = None):
    if not isinstance(polylist, collections.Iterable):
        polylist = [polylist]
    if figsize != None:
        fig = plt.figure(figsize=figsize)
    else:
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

        patch = patches.PathPatch(path, facecolor=facecolor, lw=2)
        ax.add_patch(patch)

    xpad = (r-l)*0.1
    ypad = (t-b)*0.1
    ax.set_xlim(l-xpad, r+xpad)
    ax.set_ylim(b-ypad, t+ypad)
    plt.show()

def DrawKeyboard(k, wordlist = None, logarithmic = False, pmin = None, pmax = None, inputvector = None,
                t9 = False, letters = True, frequencymap = None, oneletter = None, colormap = mpl.cm.cool, figsize = None, saveas = None,
                nopalette = False, perfectvector = None, axis = 'off', facecolor = 'lightblue', vectorcolormap = mpl.cm.autumn_r):
    fig = None
    if figsize != None:
        fig = plt.figure(figsize=figsize)
    else:
        fig = plt.figure()
    colored = False
    ax = None
    gs = None

    #if frequencymap is a word then build a new frequencymap highlighting the included letters
    if isinstance(frequencymap, str):
        word = frequencymap
        frequencymap = []
        for letter in k.OrderedKeyList():
            if letter in word.lower():
                frequencymap.append(1.0)
            else:
                frequencymap.append(0.2)
    if (wordlist != None or frequencymap != None):
        colored = True
        if not nopalette:
            ax = fig.add_axes([0.04, 0.04, 0.85, 0.92])
        else:
            ax = fig.add_subplot(111)
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
        if not nopalette:
            ax2 = fig.add_axes([0.91, 0.04, 0.05, 0.92])
            cb1 = mpl.colorbar.ColorbarBase(ax2, cmap=colormap, norm=norm, format = format, orientation='vertical')

    ordered_key_list = k.OrderedKeyList()
    for i, c in enumerate(ordered_key_list):
        p = d[c]
        verts = p.VertexList()
        codes = [Path.MOVETO] + [Path.LINETO for i in range(len(verts)-2)] + [Path.CLOSEPOLY]
        path = Path(verts, codes)
        (newl, newr, newb, newt) = (p.LeftExtreme(), p.RightExtreme(), p.BottomExtreme(), p.TopExtreme())
        l = min(l, newl)
        r = max(r, newr)
        b = min(b, newb)
        t = max(t, newt)

        if colored:
            frequency = None
            if wordlist != None:
                frequency = wordlist.LetterOccurances(c)/wordlist.TotalLetterOccurances()
            elif frequencymap != None:
                frequency = frequencymap[i]

            facecolor = scalarmap.to_rgba(frequency)

        patch = patches.PathPatch(path, facecolor=facecolor, lw=2)
        ax.add_patch(patch)

        if t9:
            if i in [0, 3, 6, 9, 12, 15, 19, 22]:
                extras = 4 if i in [15, 22] else 3
                for j in range(i+1, i + extras):
                    c += ' ' + ordered_key_list[j]
            else:
                c = ''

        if letters:
            if oneletter != None:
                c = oneletter
            patheffects = None
            tracecolor = 'ghostwhite' if colored else facecolor
            from matplotlib import patheffects
            patheffects = [patheffects.withStroke(linewidth=3,foreground=tracecolor)]
            ax.text(newl + 0.5*(newr-newl),newb + 0.5*(newt-newb), c, fontsize=18, path_effects=patheffects, horizontalalignment='center', verticalalignment='center')
    xpad = (r-l)*0.1
    ypad = (t-b)*0.1
    ax.set_xlim(l-xpad, r+xpad)
    ax.set_ylim(b-ypad, t+ypad)

    if perfectvector != None:
        points = perfectvector.PointList()
        x = [x for x,y,t in points]
        y = [y for x,y,t in points]
        from matplotlib import patheffects
        patheffects = [patheffects.withStroke(linewidth=5,foreground=(53*0.5/255,120*0.5/255,255*0.5/255))]
        ax.plot(x, y, path_effects=patheffects, lw=3, color=(53/255,120/255,255/255))

    if inputvector != None:
        #Test if inputvector is a list or not
        if not isinstance(inputvector,collections.Iterable):
            inputvector = [inputvector]
            vectorcolormap = [vectorcolormap]
        elif not isinstance(vectorcolormap,collections.Iterable) or not len(inputvector) == len(vectorcolormap):
            lightscale = 0.5
            darkscale = 0.5
            vectorcolormap = []
            for i, vector in enumerate(inputvector):
                color = next(ax._get_lines.color_cycle)
                color = mpl.colors.colorConverter.to_rgba(color)
                cdict = {'alpha': ((0.0,color[3],color[3]),(1.0,color[3],color[3]))}
                for j, channel in enumerate(['red', 'green', 'blue']):
                    light = 1.0 - (1.0 - color[j])*lightscale
                    dark = color[j]*darkscale
                    cdict[channel] = ((0.0,light,light),(1.0,dark,dark))
                vectorcolormap.append(LinearSegmentedColormap('vectorcolormap' + str(i), cdict))

        for vectorindex, vector in enumerate(inputvector):
            points = vector.PointList()
            times = [t[2] for t in points]
            tmin = min(times)
            tmax = max(times)

            norm = mpl.colors.Normalize(vmin=0, vmax=1)
            scalarmap = mpl.cm.ScalarMappable(norm, cmap=vectorcolormap[vectorindex])
            for i, p in enumerate(points):
                scaledtime = 0.5
                scaledtime = (p[2]-tmin)/(tmax-tmin) if tmax > tmin else 0.5
                color = scalarmap.to_rgba(scaledtime)

                markersize = 15 if t9 else 10
                ax.plot(p[0], p[1],color=color, markersize=markersize, marker='o')

    plt.axis(axis)
    if saveas:
        savefig(saveas)
    plt.show()

def MakeStandardKeyboard(alphabetStr='qwertyuiopasdfghjklzxcvbnm.'):
    width = 1.0
    height = 1.5
    gap = 0.2
    startx = 0.0
    starty = 0.0
    l = []
    for i in range(10):
        p = core.Polygon()
        p.AddVertex(startx + float(i)*(width+gap), starty)
        p.AddVertex(startx + float(i)*(width+gap) + width, starty)
        p.AddVertex(startx + float(i)*(width+gap) + width, starty-height)
        p.AddVertex(startx + float(i)*(width+gap), starty-height)
        l.append(p)
    startx += 0.5*width
    starty -= height + gap
    for i in range(9):
        p = core.Polygon()
        p.AddVertex(startx + float(i)*(width+gap), starty)
        p.AddVertex(startx + float(i)*(width+gap) + width, starty)
        p.AddVertex(startx + float(i)*(width+gap) + width, starty-height)
        p.AddVertex(startx + float(i)*(width+gap), starty-height)
        l.append(p)
    startx += width + gap
    starty -= height + gap
    for i in range(7):
        p = core.Polygon()
        p.AddVertex(startx + float(i)*(width+gap), starty)
        p.AddVertex(startx + float(i)*(width+gap) + width, starty)
        p.AddVertex(startx + float(i)*(width+gap) + width, starty-height)
        p.AddVertex(startx + float(i)*(width+gap), starty-height)
        l.append(p)
    starty -= height + gap
    p = core.Polygon()
    p.AddVertex(startx + float(6)*(width+gap), starty)
    p.AddVertex(startx + float(6)*(width+gap) + width, starty)
    p.AddVertex(startx + float(6)*(width+gap) + width, starty-height)
    p.AddVertex(startx + float(6)*(width+gap), starty-height)
    l.append(p)

    k = core.Keyboard()
    for i in range(len(alphabetStr)):
        k.AddKey(alphabetStr[i], l[i])
    return k

def MakeDvorakKeyboard(alphabetStr='.pyfgcrlaoeuidhtnsqjkxbmwvz'):
    width = 1.0
    height = 1.5
    gap = 0.2
    starty = 0.0

    #Dvorak specific start points
    startx = 1.5*(width+gap)
    l = []
    for i in range(8):
        p = core.Polygon()
        p.AddVertex(startx + float(i)*(width+gap), starty)
        p.AddVertex(startx + float(i)*(width+gap) + width, starty)
        p.AddVertex(startx + float(i)*(width+gap) + width, starty-height)
        p.AddVertex(startx + float(i)*(width+gap), starty-height)
        l.append(p)
    startx = 0.0
    starty -= height + gap
    for i in range(10):
        p = core.Polygon()
        p.AddVertex(startx + float(i)*(width+gap), starty)
        p.AddVertex(startx + float(i)*(width+gap) + width, starty)
        p.AddVertex(startx + float(i)*(width+gap) + width, starty-height)
        p.AddVertex(startx + float(i)*(width+gap), starty-height)
        l.append(p)
    startx = 1.5*(width+gap)
    starty -= height + gap
    for i in range(9):
        p = core.Polygon()
        p.AddVertex(startx + float(i)*(width+gap), starty)
        p.AddVertex(startx + float(i)*(width+gap) + width, starty)
        p.AddVertex(startx + float(i)*(width+gap) + width, starty-height)
        p.AddVertex(startx + float(i)*(width+gap), starty-height)
        l.append(p)
    starty -= height + gap
    p = core.Polygon()
    p.AddVertex(startx + float(6)*(width+gap), starty)
    p.AddVertex(startx + float(6)*(width+gap) + width, starty)
    p.AddVertex(startx + float(6)*(width+gap) + width, starty-height)
    p.AddVertex(startx + float(6)*(width+gap), starty-height)
    l.append(p)

    k = core.Keyboard()
    for i in range(len(alphabetStr)):
        k.AddKey(alphabetStr[i], l[i])
    return k

def MakeHexagonalKeyboard(alphabetStr='qwertyuiopasdfghjklzxcvbnm.', scale=0.9):
    shift = (sqrt(3), -1.5)
    a, b, c = scale, scale/2.0, sqrt(3)*scale/2
    h = core.Polygon()
    h.AddVertex(0,-a)
    h.AddVertex(c,-b)
    h.AddVertex(c,b)
    h.AddVertex(0,a)
    h.AddVertex(-c,b)
    h.AddVertex(-c,-b)
    h.Translate(-shift[0]/2.0, 0)

    l = []
    for j in range(3):
        oldp = h
        for i in range(10-j):
            p = deepcopy(oldp)
            p.Translate(shift[0],0)
            l.append(p)
            oldp = p
        h.Translate(0.5*shift[0], shift[1])

    k = core.Keyboard()
    for i in range(27):
        k.AddKey(alphabetStr[i], l[i])
    return k

#Left off the '.' because I think that was it's own key
def MakeT9Keyboard(alphabetStr='abcdefghijklmnopqrstuvwxyz', scale=0.9):
    shift = (1, -1)
    h = core.Polygon()
    pad = (1.0-scale)/2.0
    h.AddVertex(pad,-pad)
    h.AddVertex(1-pad,-pad)
    h.AddVertex(1-pad,pad-1)
    h.AddVertex(pad,pad-1)
    h.Translate(-2.5, 1.5)

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

    k = core.Keyboard()
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
    newk = core.Keyboard()
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

    newk = core.Keyboard()
    for i, idx in enumerate(indices):
        newk.AddKey(letters[idx], polygons[i])

    return newk

