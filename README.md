# Endure

Endure is a persistent data structure library for C++11 inspired by (and often baked from) Clojure. Its primary goal is to
provide a basic (and currently extremely minimalistic) set of immutable data structures that can be used safely out-of-the-box
in a multithreaded environment, and strives for simplicity and consistency with the Clojure interfaces.

## Implementation

Endure relies extensively on the standard shared_ptr implementation, which is likely to be lock-free and fast enough for most
use cases. Much of the code has been directly ported from the [ClojureCLR](https://github.com/clojure/clojure-clr) project
source, and beyond that, it has not been size or speed optimized in any way.

## Submodules

For testing, Endure uses [Catch](https://github.com/philsquared/Catch) (which is fantastic!)

## Copyright and license

Copyright © 2014 Jake "Ferris" Taylor

Licensed under the  BSD 2-Clause License (see the file license.txt).
