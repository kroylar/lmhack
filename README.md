# lmhack
License Manager username switcher

## What

This software creates **LD_PRELOAD**able libraries to use when checking out licenses from Flexera license servers. These libraries will hook the *fopen* linux syscall, and if the path being opened matches */etc/passwd* will instead serve up a fake *passwd* file that only contains your username. This fake password file is stored in */tmp/lmspoof-\<username\>*, so it will work on a multi-user system.

## Why
The Flexera Software Licences Manager application that is used on a variety of proprietary applications has an extremely simple method of determining a user's username. This involves finding the UID of the user and greping */etc/passwd* for the matching username. If no matching uid/username is found, it will simply use the last username in the list.

If your username is not in */etc/passwd* (which could happen for several reasons, including using LDAP for authentication) you will end up checking out licenses as whatever that last username in the list is. This could be a system generated name or even someone else's!

Administrators have the option of allowing only certain usernames to checkout licenses. So you could eventually be blocked from getting licenses from your license servers.

## Build

run `make`

2 files will be generated, *lmhack.so* and *lmhack32.so*.

Copy *lmhack.so* to your 64bit library path (typically */usr/lib*). ie:
`cp lmhack.so /usr/lib/`

Copy *lmhack32.so* to your 32bit library path (typically */usr/lib32*). ie:
`cp lmhack32.so /usr/lib32/lmhack.so`

Note that we renamed the 32bit binary to *lmhack.so* as to make use of dynamic library selection based upon the architecture of the program being run. Also please note that sometimes distributions will have */usr/lib* for 32bit libraries and */usr/lib64* for 64bit libraries.

## Use

Simply use `LD_PRELOAD=lmhack.so` in your environment. You can either use it on the command line along with the program you are calling, or export it for all programs to use. Once the *fopen* symbol has been first hijacked, the only overhead will be 1 or 2 if statements, and any requests not accessing */etc/passwd* will be passed along to the system *fopen*.
