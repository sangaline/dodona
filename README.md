
# Dodona

Dodona is a framework for constructing keyboards and evaluating their
performance with user interaction models. It makes it easy to define a model for
inputting text on a keyboard and then do things like optimize the layout of the
keyboard. It can be used to perform analyses like the one that led to the Dvorak
keyboard but it's also very applicable to modern mobile devices.

If you've ever used T9 word, Swype, or even a phone keyboard with autocorrect
then you're probably familiar with the frustrations that can arise when the
device misinterprets what you're trying to say. Dodona helps you experiment with
these existing input methods by rearranging letters or swapping keys in order to
see how that affects things like typing speed and the rate of interpreation
errors. It also lets you explore totally new mechanisms that couple any keyboard
arrangement you would like with a user interaction model including patterns of
touches and swipes.

As you continue on through the README we'll explore some of the basic elements
and structure of the library. We hope that you find it as much fun to play with
as we do!

## Table of Contents

* [Installation](#installation)
    * [Dependencies](#dependencies)
    * [Linux Installation](#linux-installation)
    * [OSX Installation](#osx-installation)
    * [Importing the Library](#importing-the-library)
* [Keyboards](#keyboards)
    * [Polygons](#polygons)
    * [Building a Keyboard](#building-a-keyboard)
    * [Included Keyboards](#included-keyboards)
    * [Randomizing Keyboards](#randomizing-keyboards)
* [Wordlists](#wordlists)
    * [Building Wordlists](#building-wordlists)
    * [Accessing Wordlists](#accessing-wordlists)
* [Input Models](#input-models)
    * [Input Vectors](#input-vectors)
    * [Building an Input Model](#building-an-input-model)
    * [Predefined Input Models](#predefined-input-models)
    * [Predefined Distance Measure](#predefined-distance-measure)

## Installation

### Dependencies

In order to install and use the Dodana library to fulfill your wildest keyboard
analysis fantasies, you'll first need the following dependencies installed.
 * CMake version 2.8 or later
 * Boost C++ Libraries
 * Python 3.2 or later
 * FANN: Fast Artificial Neural Network Library version 2.2 (Note: this isn't
necessary but is used by certain packages.  It is turned off by default, to use
it you will need to use a certain flag with cmake, as explained below)
 * iPython (Note: this is not necessary to use the library, but it's highly
recommended as a way to interact and explore the library)

*Note: it is recommended that a MacPorts or Homebrew be used to manage the
dependencies on OSX.

### Linux Installation

Clone the repository and compile by running
```
git clone https://github.com/sangaline/dodona.git

cd /PATHTO/dodona/core

cmake .

make
```
If you have FANN installed and plan on using it then you can compile the library
with it by replacing the `cmake .` command above with
```
cmake -Dfann=ON .
```

Now the library is compiled and the file `core.so` is located in the parent
directory, `/PATHTO/dodona/`

Lastly, to use with Python the path to the `/dodona` directory must be included
in your `PYTHONPATH`.  You can double check this by running
```
echo $PYTHONPATH
```

If you do not see the correct path there then add it with the following command
(in bash)
```
export PYTHONPATH=/PATHTO/:$PYTHONPATH
```

Where, as before, `/PATHTO/` is the path leading to the `/dodona/` directory.

#### Static Compiling

On Linux you also have the option to statically compile the library.  This might
be necessary if, for example, you need to create a statically linked binary that
you want to run on a distributed cluster.

To do this simply run the following instead of the `cmake .` command listed
above
```
cmake -Dstatic=ON .
```
This will create a `core.a` object that will need to linked at compile time for
your desired binary.

### OSX Installation

Clone the repository from github.
```
git clone https://github.com/sangaline/dodona.git
```
Open `CMakeLists.txt` and edit lines 51 and 52 to point to where the appropriate
python files are located on your computer.  Their location depends on what
package manager, if any, you used to install them.  It should look like this:
```
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET(PYTHON_LIBRARY /PATH/TO/PYTHON/LIBRARIES/libpython3.4.dylib)
  SET(PYTHON_INCLUDE_DIR /PATH/TO/PTYHON/INCLUDES/python3.4m)
ENDIF()
```
But make sure that `libpython3.4.dylib` and `python3.4m` actually correspond to
the version of python you have.  It must be version 3.2 or later.

Once this is properly configured, run the following commands.
```
cd /PATHTO/dodona/core

cmake .

make
```
If you have FANN installed and plan on using it then you can compile the library
with it by replacing the `cmake .` command above with:
```
cmake -Dfann=ON .
```
Now the library is compiled and the file `core.so` is located in the parent
directory, `/PATHTO/dodona/`.  However, OSX uses `.dylib` files instead of `.so`
so you need to link them by running the following command:
```
ln -s /PATHTO/dodona/core.so /PATHTO/dodona/core.dylib
```
Lastly, to use with Python the path to the `/dodona` directory must be included
in your `PYTHONPATH`.  You can double check this by running:
```
echo $PYTHONPATH
```
If you do not see the correct path there then add it with the following command
(in bash):
```
export PYTHONPATH=/PATHTO/:$PYTHONPATH
```
Where, as before, `/PATHTO/` is the path leading to the `/dodona/` directory.

*NOTE: This has not been tested on OSX 10.10 (Yosemite) yet.  It should work
fine assuming there weren't any major changes to the XCode developer tools.

#### Static Compiling

Static compiling is not possible on OSX.

### Importing the Library

After compiling dodona it is possible to interact with it using either python or
c++. If your're relatively new to the library then we recommend starting out
using python and in particular, ipython. The interactivity makes it easier to
explore the structure of the framework as we will be doing in this README. The
following commands will import everything you need to follow along.


```python
#a couple of ipython specific commands to setup matplotlib and numpy
%pylab
%matplotlib inline

#importing the dodona framework
from dodona import core, keyboards, wordlists
```

    Using matplotlib backend: Qt5Agg
    Populating the interactive namespace from numpy and matplotlib


## Keyboards

The dodona framework centers around interactions with keyboards. Keyboards are
simply a map of ascii characters to polygons in a 2D plane. Polygons themselves
are in turn a collection of 2D vertices. We'll show a simple example of
constructing a keyboard and then go into more practical examples.

### Polygons

Polygons can be constructed by adding the necessary vertices.


```python
square = core.Polygon()
for x, y in [(0,0), (0,1), (1,1), (1,0)]:
    square.AddVertex(x, y)
    
triangle = core.Polygon()
for x, y in [(2,0), (3,0), (2.5,1)]:
    triangle.AddVertex(x, y)
```

They can also be copied and translated, which is very useful when constructing a
keyboard where all of the keys share the same shape.


```python
from copy import deepcopy

translated_square = deepcopy(square)
translated_square.Translate(0, 2)
print('The y-coordinates have been shifted by 2:\n', translated_square.VertexList())
```

    The y-coordinates have been shifted by 2:
     [(0.0, 2.0), (0.0, 3.0), (1.0, 3.0), (1.0, 2.0), (0.0, 2.0)]


You can plot a collection of polygons to getan idea of how they would look as a
keyboard:


```python
keyboards.DrawPolygons([square, triangle, translated_square], figsize=(3,3))
```


![png](http://sangaline.com/github/dodona/README_files/README_26_0.png)


and you can quickly check whether a point falls within a polygon:


```python
square.IsInside(0.5, 0.5)
```




    True



as well as determine the extremes of each polygon


```python
(triangle.LeftExtreme(), triangle.RightExtreme(), 
 triangle.BottomExtreme(), triangle.TopExtreme())
```




    (2.0, 3.0, 0.0, 1.0)



which will both be used later in our input models when determining how a user
interacts with a key.

### Building a Keyboard

To construct a keyboard from a set of polygons we simply add each polygon and
assign it a corresponding ascii character.


```python
simple_keyboard = core.Keyboard()
simple_keyboard.AddKey("a", translated_square)
simple_keyboard.AddKey("b", square)
simple_keyboard.AddKey("c", triangle)
```

We can draw the result as a keyboard, rather than just a collection of polygons,
to see each key labeled according to it's corresponding character.


```python
keyboards.DrawKeyboard(simple_keyboard, figsize=(3,3), axis='on')
```


![png](http://sangaline.com/github/dodona/README_files/README_36_0.png)


### Included Keyboards

There are a handful of keyboards that you're more likely to be interested in
using than our simple abc keyboard that we just made. For convenience we've
included routines for constructing these common keyboards. All of these
keyboards can be passed a string like 'qwertyuiopasdfghiklzxcvbnm.' to be built
with a nonstandard letter arrangement, but we'll just make the defaults here.

#### Standard QWERTY

The commonest of common. Note that it's made with a '.' key but we choose to
remove this key before plotting.


```python
qwerty_keyboard = keyboards.MakeStandardKeyboard()
qwerty_keyboard.RemoveKey('.')
keyboards.DrawKeyboard(qwerty_keyboard, figsize=(8,4))
```


![png](http://sangaline.com/github/dodona/README_files/README_41_0.png)


#### Dvorak Keyboard

Probably the most common alternative keyboard layout.


```python
dvorak_keyboard = keyboards.MakeDvorakKeyboard()
keyboards.DrawKeyboard(dvorak_keyboard, figsize=(8,4), facecolor='lightpink')
```


![png](http://sangaline.com/github/dodona/README_files/README_44_0.png)


#### T9 Keyboard

The spiritual predecessor to swipe input methods was undoubtedly T9 word. With a
T9 keyboard there is inherent ambiguity due to the reduced number of keys in
addition to variations in how a user inputs words.


```python
t9_keyboard = keyboards.MakeT9Keyboard()
keyboards.DrawKeyboard(t9_keyboard, t9=True, figsize=(5,5))
```


![png](http://sangaline.com/github/dodona/README_files/README_47_0.png)


Note that each square here actuall corresponds to three or four separate
polygons, each with its own corresponding character label. We can check this by
doing a quick test.


```python
def print_matching_keys(keyboard, x, y):
    for character, polygon in keyboard.PolygonDict().items():
        if polygon.IsInside(x, y):
            print('This point is inside the key for:', character)
print_matching_keys(t9_keyboard, 0, 0)
```

    This point is inside the key for: k
    This point is inside the key for: j
    This point is inside the key for: l


This is our first example that really demonstrates how the library can be used
to explore things outside of what we might think of as a traditional touchscreen
keyboard. There's a lot of flexibility here, especially when you consider the
characters as generic labels rather than letters, and we're looking forward to
seeing what people come up with.

### Randomizing Keyboards

We're particularly interested in seeing how different keyboard layouts affect
things like typing speed or the rate of errors and so we have a couple of
additional helper functions that might prove helpful. First we'll demonstrate
RandomSwap which swaps some number of pairs of keys labels. This is useful for
finding similar keyboards with small variations.


```python
hexagonal_keyboard = keyboards.MakeHexagonalKeyboard()
hexagonal_keyboard.RandomSwap(3)
keyboards.DrawKeyboard(hexagonal_keyboard, figsize=(8,4))
```


![png](http://sangaline.com/github/dodona/README_files/README_53_0.png)


If you want a totally random arrangement of letters then you can easily do that
as well.


```python
hexagonal_keyboard.Randomize()
keyboards.DrawKeyboard(hexagonal_keyboard, figsize=(8,4))
```


![png](http://sangaline.com/github/dodona/README_files/README_55_0.png)


## Wordlists

In order to simulate realistic user input it's important to have a collection of
words and their corresponding frequencies. This is accomplished in dodona with
wordlist objects. Wordlist objects store their information in several formats
simultaneously so that all of the operations you would want to do after
constructing your wordlist are as fast as possible.

### Building Wordlists

The simplest way to build a wordlist is to add each word individually


```python
short_wordlist = core.WordList()
short_wordlist.AddWord('pot')
short_wordlist.AddWord('pout')
short_wordlist.AddWord('twice')
short_wordlist.AddWord('twice')
print(short_wordlist.WordListDict())
```

    {'pout': 1, 'pot': 1, 'twice': 2}


but if you're in a hurry to get one then we've added a helper function to parse
a text file and construct a wordlist. The default behavior is to ignore anything
other than the characters a-z but this can be changed by passing an alternative
cleaner function.


```python
from urllib.request import urlopen

with open('/tmp/huckfinn.txt', 'w') as f:
    data = urlopen('http://www.gutenberg.org/ebooks/76.txt.utf-8').read()
    f.write(data.decode('utf-8'))
huckfinn_wordlist = wordlists.ReadBook('/tmp/huckfinn.txt')
print('Wordlist size:', huckfinn_wordlist.Words())
```

    Wordlist size: 7866


And you can also merge wordlists by adding them.


```python
huckfinn_wordlist = huckfinn_wordlist + short_wordlist
```

### Accessing Wordlists

One way to access words is by index. The words are automatically sorted by their
number of occurances so that the lowest indices will correspond to the most
common words.


```python
for i in range(5):
    word = huckfinn_wordlist.Word(i)
    frequency = huckfinn_wordlist.Occurances(word)/huckfinn_wordlist.TotalOccurances()
    print('{:8s}: {:.5f}'.format(word, frequency))
```

    and     : 0.05526
    the     : 0.04414
    i       : 0.02813
    a       : 0.02627
    to      : 0.02624


Often when simulating user interactions it's convenient to pick random words
based on their frequency in the corpus.


```python
for i in range(5):
    word = huckfinn_wordlist.RandomWord()
    frequency = huckfinn_wordlist.Occurances(word)/huckfinn_wordlist.TotalOccurances()
    print('{:8s}: {:.5f}'.format(word, frequency))
```

    away    : 0.00159
    under   : 0.00075
    and     : 0.05526
    nobody  : 0.00067
    them    : 0.00449


A slightly more exotic way to find words in a wordlist is based on substring
matching. To illustrate this concept imagine swiping your finder continuously on
your keyboard from the letter p to the letter t. As you do this you go over the
sequence of letters 'poiuyt'. The wordlist can then recursively search it's
radix tree representation to find words contained within this string (by default
the first and last letters have to also match).


```python
huckfinn_wordlist.SubstringMatches('poiuyt')
```




    ['pout', 'pot', 'put']



If you now think about how you would swipe along the keyboard if you were trying
to spell out one of these words you'll see that they would all produce the same
sequence 'poiuyt'. This method of accessing words can be very helpful when we're
trying to figure out what somebody intended to write when they swipe a pattern.

## Input Models

Input models are used to describe how a user interacts with a touch keyboard to
enter information. We'll go into them in a little more depth in just a minute
but first we'll discuss briefly how we represent user inputs.

### Input Vectors

The input from a user trying to enter a word is represented by an object called
in input vector. An input vector is essentially a series of 2D spatial
coordinates coupled with a time. This corresponds to a series of touch events on
a touchscreen and can be used to either describe independent touch events or a
continuous swipe event through discrete time sampling of the motion.

#### Building Input Vectors

Here's an example of how to construct an input vector that corresponds to a
series of touch events spelling out the word 'cat'.


```python
def build_inputvector(word, keyboard, bias=(0,0), randomness=(0,0)):
    inputvector = core.InputVector()
    for time, letter in enumerate(word):
        polygon = keyboard.GetKey(letter)
        x = bias[0] + (polygon.LeftExtreme() + polygon.RightExtreme())/2
        y = bias[1] + (polygon.BottomExtreme() + polygon.TopExtreme())/2
        x += (np.random.normal(0, randomness[0]) if randomness[0] > 0 else 0)
        y += (np.random.normal(0, randomness[1]) if randomness[1] > 0 else 0)
        inputvector.AddPoint(x, y, time)
    return inputvector
        
inputvector = build_inputvector("cat", qwerty_keyboard, bias=(0,-0.3))
keyboards.DrawKeyboard(qwerty_keyboard, inputvector=inputvector, figsize=(8,4))
```


![png](http://sangaline.com/github/dodona/README_files/README_79_0.png)


#### Comparing Input Vectors

When trying to understand what word a user intended to input it can be helpful
to have a way to quantify how similar input vectors are. The simplest way to do
this is just taking the cartesian distance between corresponding points in two
input vectors. We have some of these baked into our included models but here's
an idea of how they work.


```python
def cartesian_distance(vector1, vector2):
    length = vector1.Length()
    if vector2.Length() != length:
        return float('inf')
    distance = 0
    for i in range(length):
        distance += (vector1.X(i) - vector2.X(i)**2)
        distance += (vector1.Y(i) - vector2.Y(i)**2)
    return np.sqrt(distance)
```

### Building an Input Model

All input models have to inherit from the InputModel and then implement a couple
of key functions but after that they can be used anywhere in the dodona
framework: even if you implement the functions in python rather than c++.
Two of the most important functions that each input model needs to have is the
`RandomVector()` and `PerfectVector()` function.

`RandomVector()` returns, as you would imagine, a random input vector for a
given word on a given keyboard.  This allows the model to incorporate the
inexact nature of typing on a touchscreen.

`PerfectVector()` returns a "perfect" input vector, which means that each touch
entry associated with a letter is positioned exactly at the center of each key.
This means that each word has one unique perfect vector for a given keyboard
layout.  This is useful for developing standardized distance measures.

`Distance()` and `VectorDistance()` then let us know how similar a word and
input vector two input vectors are.

Here's a quick example using what we just learned about input vectors to create
a new `InputModel`.


```python
class TouchModel(core.InputModel):
    def __init__(self, bias=(0.25, 0.25), randomness = (0.25, 0.25)):
        core.InputModel.__init__(self) #this line is crucial!
        self.bias = bias
        self.randomness = randomness
        
    def RandomVector(self, word, keyboard):
        return build_inputvector(word, keyboard, self.bias, self.randomness)
    
    def PerfectVector(self, word, keyboard):
        return build_inputvector(word, keyboard, (0, 0), (0, 0))
    
    def VectorDistance(self, vector1, vector2):
        return cartesian_distance(vector1, vector2)
    
    def Distance(self, vector1, word, keyboard):
        return self.VectorDistance(vector1, self.PerfectVector(word, keyboard))
    
touch_model = TouchModel()
```

### Predefined Input Models

In order to model text input a touchscreen keyboard we have developed a couple
of input models which we consider to be generally useful. For our tap typing
bretheren there is the `SimpleGaussianModel` class, which draws discrete points
from a 2D Gaussian distribution centered on each key in the input word. This
model is essentially identically to the TouchModel we demonstrated above. For
swipe input there is the `SimpleInterpolationModel` class, which uses the
SimpleGaussianModel class to generate control points around each letter in the
input word.  It then interpolates between the control points based on a
predetermined interpolation.

Examples of how to use the predefined input models are shown below.

#### SimpleGaussianModel

As described above, this model is intended to be used to model tap typing input.
For each letter in a word a coordinate is generated cooresponding to the
position of a likely touch event.  The coordinates are drawn randomly from a 2D
Gaussian distribution centered around the center of each key.  Some important
parameters are listed below.

 * The width of this distribution in the x and y direction are the two most
important parameters for this model.  The widths are normalized so that a value
of 1.0 corresponds to the width of the key.  These are set using the
`SetXScale()` and `SetYScale()` functions or by setting them both to the same
value with `SetScale()`

 * By default the positions of the input for each successive keys are assumed to
be independent random variables. However, a correlation can be introduced by
setting the correlation coefficient.

 * The seed for the random number generator can also be set by hand to ensure
reproducability.

An example of how to initialize and use this model is shown below.


```python
touch_model = core.SimpleGaussianModel()

#set x and y scales simultaneiously...
touch_model.SetScale(0.25)
#or indidually
touch_model.SetXScale(0.4)
touch_model.SetYScale(0.3)

#set the correlation between successive points
touch_model.SetCorrelation(0.2)
```



Now we can create some input vectors from our model using the `RandomVector()`
and `PerfectVector()` methods of the `SimpleGaussianModel` class.


```python
word = 'tylenol'
random_vector = touch_model.RandomVector(word, qwerty_keyboard)
perfect_vector = touch_model.PerfectVector(word, qwerty_keyboard)
```

Now we can plot the results! The order of the key input is indicated by the
color, yellowish to start and magenta at the end. This is determined by the
timing information in the input vector.


```python
keyboards.DrawKeyboard(qwerty_keyboard, inputvector=random_vector, figsize=(8,4))
```


![png](http://sangaline.com/github/dodona/README_files/README_94_0.png)


The perfect vector would look similar but the points would be in exactly the
center of each corresponding see (making them a little bit harder to see).

#### SimpleInterpolationModel

The `SimpleInterpolationModel` is very similar to the `SimpleGaussianModel`
except that it interpolates between the individual keys to produce a swipe-like
input vector.  Some important parameters are listed below.

 * The `VectorLength` determines how many interpolation points are included in
each swipe input. This number is normalized for every input vector regardless of
the length of the word.
 * `Interpolation` determines which interpolatin technique is used and can be
set to any predefined function contained in
`/dodona/core/inc/InputModels/Interpolation.h`

An example of how to initialize and use the model is shown below.


```python
swipe_model = core.SimpleInterpolationModel()
swipe_model.SetXScale(0.8)
swipe_model.SetYScale(0.5)
swipe_model.SetCorrelation(0.2)

#determine the number of interpolation points in each input vector
swipe_model.SetVectorLength(75)
#set the desired interpolation function
swipe_model.Interpolation = core.SpatialInterpolation
```

Now we can create some input vectors and plot the results!  We will start with a
basic straight line interpolation.


```python
word = 'jones'
random_vector = swipe_model.RandomVector(word, qwerty_keyboard)
perfect_vector = swipe_model.PerfectVector(word, qwerty_keyboard)

keyboards.DrawKeyboard(qwerty_keyboard, inputvector=random_vector, figsize=(8,4))
keyboards.DrawKeyboard(qwerty_keyboard, inputvector=perfect_vector, figsize=(8,4))
```


![png](http://sangaline.com/github/dodona/README_files/README_101_0.png)



![png](http://sangaline.com/github/dodona/README_files/README_101_1.png)


We can also use a fancier interpolation to try and produce more variety in our
swipe inputs.


```python
swipe_model.Interpolation = core.CubicSplineInterpolation
random_vector = swipe_model.RandomVector(word, qwerty_keyboard)
perfect_vector = swipe_model.PerfectVector(word, qwerty_keyboard)

keyboards.DrawKeyboard(qwerty_keyboard, inputvector=random_vector, figsize=(8,4))
keyboards.DrawKeyboard(qwerty_keyboard, inputvector=perfect_vector, figsize=(8,4))
```


![png](http://sangaline.com/github/dodona/README_files/README_103_0.png)



![png](http://sangaline.com/github/dodona/README_files/README_103_1.png)


### Predefined Distance Measure

Each `InputModel` also has a member function called `Distance()` which can be
used to compare how similar a specific swipe pattern is to the perfect vector of
a given word. The included models that we've talked about here all use the
`cartesian_distance()` function that we explained above but these can easily be
overriden.

Let's see how similar the swipe pattern for "jones" from the previous example is
to the word "tylenol".


```python
jones_random_vector = swipe_model.RandomVector('tylenol', qwerty_keyboard)
swipe_model.Distance(jones_random_vector, 'tylenol', qwerty_keyboard)
```




    6.484111332930911



We can then compare the above value to the distance between the input vector for
"jones" and the word "jones" itself to see which word is more likely to match
with the input vector.


```python
swipe_model.Distance(jones_random_vector, 'jones', qwerty_keyboard)
```




    39.38995135921923



We can also compare two input vectors directly by using another `InputModel`
member function called `VectorDistance()`


```python
jones_perfect_vector = swipe_model.PerfectVector('jones', qwerty_keyboard)
swipe_model.VectorDistance(jones_random_vector, jones_perfect_vector)
```




    39.38995135921923



From these examples we can see how we can see how we could build towards
determing the best match for a word.


```python
def best_match(model, input_vector, keyboard, wordlist):
    best_distance, best_word = None, None
    for i in range(wordlist.Words()):
        distance = model.Distance(input_vector, wordlist.Word(i), keyboard)
        if not best_word or distance < best_distance:
            best_distance = distance
            best_word = wordlist.Word(i)
    return best_word

random_vector = swipe_model.RandomVector('agreement', qwerty_keyboard)
matched_word = best_match(swipe_model, random_vector, qwerty_keyboard, huckfinn_wordlist)
print('Agreement?', matched_word)
```

    Agreement? agreement


Not too shabby!
