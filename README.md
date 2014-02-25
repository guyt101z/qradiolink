QRadioLink overview
-------------------

Quick DIY Echolink alternative:
QRadioLink is a software used for interconnecting radio stations
over internet or other TCP/IP networks. Radio over IP is probably
the best description for this process. 
Repeaters or simple transceivers are paired with a Linux computer
running this software, which acts both as a repeater controller
as well as a VoIP system. 

QRadioLink can be used for interconnecting amateur radio repeaters
and simplex radio nodes, in a fashion very similar to existing
software packages like Echolink, IRLP, Svxlink and app_rpt/Asterisk.
Each station or repeater needs to have an audio connection with the
computer running QRadioLink. The audio connection is from the input
of the radio to the output port of the soundcard, and from the input
port of the soundcard to the output port of the radio.

The software is designed so it can be run on low-power micro-computers
like the Raspberry Pi or any other board that supports a Linux operating
system. They all have the advantage of small dimensions and
very low power consumption. Most of them have all the interfacing ports
available by default, while others can be installed. 
QRadioLink can work with USB soudcards made by different manufacturers.
In order to function, the system needs electric power, network access
and audio connection to the transceivers or repeaters.
Transmitting can be activated via the VOX function, or by using a
simple hardware interface to connect PTT to the GPIO pins or a serial
port.

Features in development
-----------------------

- Network link control: enable or disable network linking with a DTMF
command
- Support for multiple VoIP servers.
- Unlimited voice announcements. Just write the texts in the database.
- Periodic station indentification. Can be done with a text message or
by recording a small audio file.
- Text messages via radio: send text messages to a gateway by connecting
with a normal browser. Messages are repeated maximum 3 times, at a certain
configurable interval (not smaller than 1 minute). SMS via phone are also
considered.
- Voice mail: send a DTMF command that will record 30 seconds of audio,
which will be played back by sending another DTMF sequence.
- Record a conversation: if you have enough disk space, you can record
a part or the whole of the conversation.
- Connect or disconnect stations or channels with just a few DTMF codes.
- Readback all known stations to the current gateway, along with their
access codes.
- Configuration via a web interface.
- COS/PTT via serial/GPIO pins.

Project status
--------------

QRadioLink is entering the alpha stage. This means we are out of the 
planning stage and we can provide working alpha builds.
Testing them is encouraged, and bug reports are welcome.
Please use the mailing list for discussion and bug reports.

Design and comparison with other linking systems
------------------------------------------------

Unlike Echolink, this software was designed so it can operate without
a classic internet connection. If you can interconnect wirelessly two
stations, you can also use QRadioLink to stream audio between them.
In other words, there is no requirement for a central authority server
to be accessible. This means that remote repeaters can be linked
together without having access to the internet.

Commands are still sent through the classic DTMF signaling, and any
station running QRadioLink can start a call with any other station 
about which it knows, or it can gather in a conference a theoretically
unlimited number of stations.

Although QRadioLink is mostly designed to link amateur radio stations,
it can also be used on any radio network to add VoIP capability.

Use cases
---------

As a use case scenario, station A is a repeater which works normally in
standalone mode (no connection to other stations). A user of the repeater
wishes to connect with repeater B and repeater C which are also in 
standalone mode. The user dials the access codes of stations B and C and
a conference is automatically created. Now the user of repeater A can also 
be heard through repeaters B and C. At the end of the voice call, a 
command can be sent to repeater A to disconnect both B and C, or just one
of them.
In an emergency mode, all repeaters using the system from a certain area
can be connected together, thus creating a large area coverage network.

If a station running QRadioLink also has internet access, it can be connected
to other stations all over the globe which also have internet access.
Because there is no central authority to manage networks, some coordination
is necessary and presents a challenge. This will be solved in the future
by having a distributed database, which will be exchanged between peers,
just like communication between two nodes happens in a peer to peer fashion,
without having a central server to manage them.

Servers
-------

Presently, the VoIP voice communication conferences require the presence of
a VoIP server, which can handle a large number of nodes, and an unlimited
number of separate conferences (channels). 
The server is optimised to run on low-CPU machines like routers or embedded
computers.
With a reasonable throughput network bandwidth, a server should be able to
handle even 100 simultaneous clients without much effort.
This means that a QRadioLink client has lower bandwidth requirements, compared
to the server. The server can be located on a local LAN or wireless network,
or it can be also connected to the internet. Multiple servers can be set up
in the same local network.
All voice communication over the network is full duplex, meaning that a person
talking through a repeater via radio can be heard at the same time with an
incoming network voice message.

Voice codec
-----------

QRadioLink offers a choice of two voice codecs: a low-latency and high-quality
Opus codec, which uses a variable bandwidth which is generally higher than
10 kbps but lower than 30 kbps, and a very-low-bitrate codec, namely Codec2,
which is ideal for stations linked via a slow and unreliable wireless connection.

Network protocol
----------------

QRadioLink offers the choice between two operating protocols:
1. IAX2, using Asterisk as a central conference server and Speex as a voice codec
2. A modified version of the Mumble protocol, with either Opus or Codec2
The second option is the recommended one, as it is the most versatile and is being
actively developed. By using it, you gain access to the latest generation of voice
codecs, being able to choose between lower bandwidth and higher voice quality.
Also, the second option offers a very simple server configuration, at the expense
of being able to interconnect with other Asterisk systems, Echolink and app_rpt.
In the future, it may be possible to integrate QRadioLink with Svxlink, offering
access to Echolink stations if an internet connection is available.
With automatic codec negociation, it will be possible to have the most suitable
codec used by all nodes in a conference.

As far as linking other systems, including digital voice radio, it is certainly
possible to use QRadioLink. The voice servers are agnostic regarding the data stream.


Software dependencies
---------------------

Aside from being designed to run on the Linux operating system, QRadioLink also
depends on some software packages: libssl, libopus, libestools, libprotobuf,
libFestival (for text-to-speech voice announcements), libpulse-simple, pulseaudio,
libcodec2, libiaxclient, libQtCore, libQtNetwork, libQtSql, speech-tools

Installing and configuring
--------------------------

Installing a binary build of the client should be quite simple, as well as configuring
it. It is also intended to make voice servers also quite plug-and-play.
Installing from source will probably be the recommended way, and will require a basic
compiling environment for Linux, as well as the development versions of the libraries
mentioned above.
All configuration for the client will be held in a sqlite database, which can easily
be deployed to multiple locations.

License information
-------------------

QRadioLink is licensed under the GNU GPL v2 license, and also incorporates some code
which is BSD licensed. The source code authors are credited in the source files.

