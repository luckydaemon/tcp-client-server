# tcp-client-server
tcp client-server app with ASN1 length octets coding

# Requirements

## Common requirements
Installed GMP - GNU Multi-Precision Library.  

## Platform-specific

For Windows OS - installed msys2 with gcc compiler packages.

# Build

Compile server program with command:

> g++ server.cpp -o server -lgmp -lgmpxx -lpthread

Compile client program with command:

> g++ client.cpp -o client -lgmp -lgmpxx

# Running the app

For server: just run complied program from command line

For client: run with 1 argument - length value to encode. 

Workflow: Client receives length value, encode it and send to server. Server get encoded value, decode it and prints orgignal value in console.

Folder "binaries" contains compiled for Windows 10 .exe files with required .dlls (when running through msys2 console .dlls are not needed). 
