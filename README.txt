THE COMPILING PROCESS:
These instructions were tested on Ubuntu 14.04 and 16.04 and Fedora 24.

1. Make sure you have at least the 6th version of g++.

2. You need to install the modified version of ngSPICE from its source code:
    1. cd to ngspice
    2. a) the instructions are in the file named INSTALL in sections 1.3 and
          1.4 (for Linux)
    2. b) you can run the following commands (you may need to update your
          system first and have autoconf, libtool, bison and automake
          installed):
            $ ./autogen.sh
            $ mkdir debug; cd debug
            $ ../configure --with-ngshared
            $ make
            $ sudo make install

3. You need to have the R interpreter installed on your system alongside the
   packages that we need:
    1. install R on your system
    2. sudo mkdir -p /usr/lib64/R/library/
    3. a) install Rcpp and RInside:
            - http://dirk.eddelbuettel.com/code/rcpp.html
            - http://dirk.eddelbuettel.com/code/rinside.html
    3. b) you can run the following command in R (run R with sudo):
          $ install.packages(c('Rcpp', 'RInside'), lib='/usr/lib64/R/library/')
          The Rcpp package will probably not create the libRcpp.so file
          (probably a bug), in such case you can create a symlink that points
          to it:
          $ sudo ln -s /usr/lib64/R/library/Rcpp/libs/Rcpp.so /usr/lib64/R/library/Rcpp/libs/libRcpp.so
          There are sometimes errors when running these packages installed only
          with sudo. In order to avoid them, run R again without sudo and
          install the packages locally:
          install.packages(c('Rcpp', 'RInside'))

4. Run cmake and make:
    1. go back to the directory where you extracted the zip file
    2. you may need to adjust the paths in the CMakeLists.txt file (lines from
       10 to 20) according to your system in order to load the dependencies
    3. run the following commands:
       $ mkdir build; cd build
       $ cmake ../
       $ make

5. Run the binary:
    $ ./bt [OPTIONS]...
