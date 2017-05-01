THE COMPILING PROCESS:

1. You need to install the modified version of ngSPICE from its source code:
    1. cd to ngspice
    2. a) the the instructions are in the file named INSTALL in sections 1.3 and
          1.4 (for Linux)
    2. b) you can run the following commands:
          $ ./autogen.sh
          $ mkdir debug
          $ cd debug
          $ ../configure --enable-xspice --enable-cider --enable-openmp
          $ make
          $ sudo make install

2. You need to have the R interpreter installed on your system alongside the
   packages that we need:
    1. install R
    2. a) install Rcpp and RInside:
            - http://dirk.eddelbuettel.com/code/rcpp.html
            - http://dirk.eddelbuettel.com/code/rinside.html
    2. b) you can run the following command in R (run R with sudo):
          $ install.packages(c('Rcpp', 'RInside'))

3. Run cmake and make:
    1. you may need to adjust the paths in the CMakeLists (lines from 10 to 20)
       according to your system in order to load the dependencies
    2. run the following commands:
       $ mkdir build; cd build
       $ cmake ../
       $ make

4. You can run the binary:
    $ ./bt