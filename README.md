# Stilt Fox&reg; Dial Up
### version 0.x.x
Stilt Fox&reg; Dial Up is a collection of libraries designed to make a networked program easier to write. These
libraries focus on the server side, with the idea that the program you're going to be writing will be serving
requests. HTTP version 1.1 is supported but may not be up to full ISO specification.

This program also allows for parsing raw socket data. This means that if you aren't writing something based on HTTP
then you can still just use the socket on its own.

## Legal Stuff
Stilt Fox&reg; is not liable for any damages done to your hardware. For more information see LICENSE file.\
Stilt Fox&reg; is trademarked. You may not use the Stilt Fox™ name, however this code is free to reference and use.\
You may contribute to this library, however all contributions will share the same license as this library, and you agree
that Stilt Fox&reg; owns the copyright for any contributions.

------------------------------------------------------------------------------------------------------------------------
## Important!
These instructions assume you are using a Linux based operating system to compile the code. No testing or compiling
has been performed on Windows by Stilt Fox&reg;. If you choose to use Windows as your main development platform you are
on your own. If you have a strong desire to use Stilt Fox&reg; libraries and products, switching to Linux is highly
recommended.

With The death of Windows 10 (as of the time of writing this) directly around the corner, the roadmap to support Windows
systems has been completely canceled.

### On Macintosh
Mac compilation also has not been tested. Mostly because a Mac computer is required to do it. From what we understand
this code should be able to run on Mac, but may require some tweaking. Because of the similarities between Mac and Linux
a lot of times the code is portable. If you happen to test this out, and it works please let us know.
------------------------------------------------------------------------------------------------------------------------
## Prerequisites
### Main Program
These libraries and programs are required to build and run this project.
- Cmake
  - version 3.5.0 or greater
- C++ compiler
  - must support C++ 20 standard or higher
  - we at Stilt Fox&reg; use g++