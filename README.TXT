*****************************
* KnoRBA C++ Library v0.6.2 *
*****************************


Contents
--------

  1. About
  2. License
  3. System Requirements
  4. How to Download
  5. How to Build
  6. How to Use
  7. Where to Find Additional Resources
  8. How to Request a Feature or Report a Bug
  9. Credits



1. About
--------

  This is the C++ programming API for Knowledge Request-Broker Architecture
(KnoRBA).

  KnoRBA is the world first general-purposed framework agent-based programming.
The framwork specifies the way agents are created, stablishes location and
implementation transparency. That is, agents written in any language can
communicate with any other agent written in any other language, regardless of
where in the world it is located, as long as network connection exists between
the two.



2. License
----------

  KnoRBA C++ Library is protected under KNORBA FREE PUBLIC LICENSE. You are
allowed to use, modify, or redistribute the source code, but prohibited to use
it for military, spying, terrorism, and other harmful applications, or to
provide it to another person having the intention for it to be used as such.

  Please refer to the accompanying LICENSE.TXT file for details.



3. System Requirements
----------------------

  - CMake 2.6 or above
  - Any C++98 or above compiler compatible with CMake
  - KFoudation library



4. How to Download
------------------

  Either clone the git repository:

    $git clone https://github.com/hkhandan/libknorba-cpp.git

or, visit

    https://github.com/hkhandan/libknorba-cpp/releases

to download the software as a compressed archive.



5. How to Build
---------------

  Assuming the source code is in "libknorba-cpp" directory,

    $cd libknorba-cpp
    $mkdir build
    $cd build

  If KFoundation is installed in a location other than /usr/local, use ccmake
to set that first. You can also use the ccmake UI to set a custom installation
location for KnoRBA C++ Library.

    $ccmake ..

  When done with the settings, press c, and then g. Then,

    $make
    $sudo make install



6. How to Use
-------------

  You can compile agents using this library, but you need KnoRBA ARE which is
a separated software to run them. See www.knorba.org for more information.

  To use you need to link your code against

    INSTALL_DIR/lib/libknorba.a

and

   INSTALL_DIR/lib/standardprotocols.a

and add

   INSTALL_DIR/include

to your include directories.



7. Where to Find Additional Resources
-------------------------------------

Please checkout

  http://www.knorba.org/

for documents, tutorials, and more.



8. How to Request a Feature or Report a Bug
-------------------------------------------

  Your beedback and ideas are more than welcome. If you have a github account,
the preffered way is via github:

    https://github.com/hkhandan/libknorba-cpp/issues

  Otherwise, please e-mail the authors (below).



9. Credits
----------

  Hamed "Kay" KHANDAN, Dr. Eng.

    Affiliation1: Graduate School of Systems Informatics, Kobe University
    Affiliation2: AICS Advanced Institute for Computational Science
    E-mail: hkhandan@yahoo.com
