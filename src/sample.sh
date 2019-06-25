#!/usr/bin/bash
CLPCREATE="./clpcreate"
$CLPCREATE init EUC-JP
$CLPCREATE add cls mycluster EUC-JP linux
$CLPCREATE add srv server1 0
$CLPCREATE add srv server2 1
$CLPCREATE add ip server1 0 192.168.137.71
$CLPCREATE add ip server1 1 192.168.138.71
$CLPCREATE add ip server1 2 192.168.139.71
$CLPCREATE add ip server1 3 192.168.140.71
$CLPCREATE add ip server2 0 192.168.137.72
$CLPCREATE add ip server2 1 192.168.138.72
$CLPCREATE add ip server2 2 192.168.139.72
$CLPCREATE add ip server2 3 192.168.140.72
$CLPCREATE add hb 0 0
$CLPCREATE add hb 1 1
$CLPCREATE add hb 2 2
$CLPCREATE add hb 3 3
$CLPCREATE add np ping pingnp1 0 10200 0 192.168.137.1
$CLPCREATE add np ping pingnp1 0 10200 1 192.168.137.76
$CLPCREATE add objnum 14
