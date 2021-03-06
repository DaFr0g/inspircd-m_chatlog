About
=====

The Channel Chat Logging module is a third-party InpsIRCd module that logs all
user messages as well as channel joins, parts, and quits. 

It logs with a type of `m_chatlog_user` and `m_chatlog_channel` to make it easy to filter to a separate log file.

Credit
======

The original module was written by [Daniel Rich](mailto:drich@employees.org)

Forked on github by [Josh Enders](mailto:josh.enders@gmail.com).

Forked again by [Daniel Rich](mailto:drich@employees.org)

Forked by DaFr0g.

Building
=========

This module has been successfully built against InspIRCd v2.0.20. It's likely
to build against other versions but this hasn't been verified.

Clone the latest version of InspIRCd from the repository

    git clone git://gitorious.org/inspircd/inspircd.git && pushd inspircd

Create a local branch and checkout into the new branch

    git checkout -b insp205 v2.0.20

Download the module

    wget -O src/modules/extra/m_chatlog.cpp https://raw.github.com/DaFr0g/inspircd-m_chatlog/master/m_chatlog.cpp

Enable the module

    ./configure --enable-extras=m_chatlog.cpp

Configure

    ./configure

Build and install

    make && make install

Configuration
=============

The following lines should be appended to `/etc/inspircd/modules.conf`

    #-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#
    # Chat log module: Logs all chats to the ircd log at default loglevel.
    <module name="m_chatlog.so">
    
    #-#-#-#-#-#-#-#-#-# CHATLOG CONFIGURATION #-#-#-#-#-#-#-#-#-#-#-#-#-#
    # There may be certain nicks that you do not want logged for security
    # reasons. Exceptions can be added below.
    #<chatlog exception="nickserv">
    #<chatlog exception="chanserv">
    
    #-#-#-#-#-#-#-#-#-# LOGGING CONFIGURATION #-#-#-#-#-#-#-#-#-#-#-#-#-#
    <log
    method="file"
    type="USERINPUT USEROUTPUT m_chatlog_user"
    level="default"
    target="logs/chat-user.log">
    <log
    method="file"
    type="USERINPUT USEROUTPUT m_chatlog_channel"
    level="default"
    target="logs/chat-channel.log">
	
