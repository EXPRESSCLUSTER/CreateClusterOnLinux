#!/usr/bin/perl

#use strict;
use warnings;

##################################################
# Sample script for Linux
##################################################
# Parameters
#-------------------------------------------------
# $clpcreate
#  Set the relative path for clpcreate.
my $clpcreate = './clpcreate';
#
# $type
#  Set type for final action of monitor resources.
#  0: use default final action (do nothing)
#  1: ignore timeout
#  2: initiate BSoD/panic immediately
my $type = 1;
#
# %cluster
#  encode:
#   Japanese: EUC-JP (for Linux)
#   English : ASCII
#   Chinese : GB2312
#  os:
#   windows or linux
my %cluster = (name=>'cluster', encode=>'EUC-JP', os=>'linux');
#
# $server
#  Top of the list is master node.
my $server =
[
    ['server1'],
    ['server2'],
    []
];
# 
# $ip
#  Set IP addresses for the servers.
#  On this case, upper IP addresses for server1 and bottom IP addresses for server2.
my $ip =
[
    ['192.168.137.71', '192.168.138.71', '192.168.139.71', '192.168.140.71'],
    ['192.168.137.72', '192.168.138.72', '192.168.139.72', '192.168.140.72'],
    []
];
#
# $hb
#  Left : The device ID to be used for heartbeat on primary server
#  Right: The device ID to be used for heartbeat on secondary server
my $hb =
[
    ['0', '0'],
    ['1', '1'],
    ['2', '2'],
    ['3', '3'],
    []
];
# 
# $diskhb
#  Device ID, Heartbeat priority, Disk path
my $diskhb =
[
    ['300', '4', '/dev/sdc1'],
    []
];
#
# $pingnp
#  Name, NP priority, Device ID, Group ID, [List ID, IP address], ...
my $pingnp =
[
    ['pingnp1', '0', '10200', '0', ['0', '192.168.137.1'], ['1', '192.168.137.76'], []],
    []
];
#
# $group
#  Name
my $group =
[
    ['failover'],
    []
];
#
# $resource
#
my $resource =
[
    [
        ['fip', 'fip1', ['ip', '192.168.137.70']],
        ['volmgr', 'volmgr1', ['parameters/type', 'lvm'], ['parameters/devname', 'ecxsd']],
        ['disk', 'disk1', ['parameters/disktype', 'lvm'], ['parameters/device', '/dev/ecxsd/sd1'], ['parameters/mount/point', '/mnt/sd1'], ['parameters/fs', 'ext3']],
        ['exec', 'exec1', ['parameters/act/path', 'start.sh'], ['parameters/deact/path', 'stop.sh']],
        []
    ],
    []
];
#
# $rscdepend
#  2nd resource depends on 1st resource.
#    2nd resource starts after 1st resource finishes starting.
#    1st resource stops after 2nd resource finishes stopping.
my $rscdepend =
[
    ['volmgr1', 'disk1'],
    []
];
#
# $monitor
#
my $monitor =
[
    ['userw', 'userw', ['parameters/method', 'keepalive']],
    ['fipw', 'fipw1', ['parameters/monmii', '1'], ['target', 'fip'], ['relation/type', 'grp'], ['relation/name', $group->[0][0]]],
    ['volmgrw', 'volmgrw1', ['parameters/devname', 'ecxsd'], ['target', 'volmgr1'], ['relation/type', 'grp'], ['relation/name', $group->[0][0]]],
    ['diskw', 'diskw1', ['parameters/object', '/dev/sdc2'], ['relation/type', 'grp'], ['relation/name', $group->[0][0]]],
    ['ipw', 'ipw1', ['parameters/list@0/ip', '192.168.137.1'], ['relation/type', 'grp'], ['relation/name', $group->[0][0]]],
    ['genw', 'genw1', ['parameters/path', 'genw.sh'], ['relation/type', 'grp'], ['relation/name', $group->[0][0]]],
    []
];
##################################################


##################################################
# You don't need to change the following lines, maybe.
#
my $ret = 0;
my $i = 0;
my $j = 0;
my $k = 0;
# initialize
$ret = `$clpcreate init $cluster{'encode'}`;

# add a cluster to initial configuration
$ret = `$clpcreate add cls $cluster{'name'} $cluster{'encode'} $cluster{'os'}`;

# add a server to a cluster
for ($i = 0; $i < scalar(@$server); $i++)
{
    next if (scalar(@{$server->[$i]}) == 0);
    $ret = `$clpcreate add srv $server->[$i][0] $i`;
}

# add an IP address to a server
for ($i = 0; $i < scalar(@$server); $i++)
{
    next if (scalar(@{$server->[$i]}) == 0);
    for ($j = 0; $j < scalar(@{$ip->[$i]}); $j++)
    {
        $ret = `$clpcreate add ip $server->[$i][0] $j $ip->[$i][$j]`;
    }
}

# add a heartbeat interface to a cluster
for ($i = 0; $i < scalar(@$hb); $i++)
{
    next if (scalar(@{$hb->[$i]}) == 0);
    $ret = `$clpcreate add hb $hb->[$i][0] $hb->[$i][1]`;
}

# add a disk heartbeat to a cluster
for ($i = 0; $i < scalar(@$diskhb); $i++)
{
    next if (scalar(@{$diskhb->[$i]}) == 0);
    $ret = `$clpcreate add diskhb $diskhb->[$i][0] $diskhb->[$i][1]`;
}

# add a disk heartbeat to a server
for ($i = 0; $i < scalar(@$server); $i++)
{
    next if (scalar(@{$server->[$i]}) == 0);
    for ($j = 0; $j < scalar(@$diskhb); $j++)
    {
        next if (scalar(@{$diskhb->[$j]}) == 0);
        $ret = `$clpcreate add diskhbsrv $server->[$i][0] $diskhb->[$j][0] $diskhb->[$j][2]`;
    }
}

# add a network partition resource to a cluster
for ($i = 0; $i < scalar(@$pingnp); $i++)
{
    next if (scalar(@{$pingnp->[$i]}) == 0);
    for ($j = 4; $j < scalar(@{$pingnp->[$i]}); $j++)
    {
        next if (scalar(@{$pingnp->[$i][$j]}) == 0);
        $ret = `$clpcreate add np ping $pingnp->[$i][0] $pingnp->[$i][1] $pingnp->[$i][2] $pingnp->[$i][3] $pingnp->[$i][$j][0] $pingnp->[$i][$j][1]`;
    }
}

# add a network partition resource to a server
for ($i = 0; $i < scalar(@$server); $i++)
{
    next if (scalar(@{$server->[$i]}) == 0);
    for ($j = 0; $j < scalar(@$pingnp); $j++)
    {
        next if (scalar(@{$pingnp->[$j]}) == 0);
        $ret = `$clpcreate add npsrv ping $server->[$i][0] $pingnp->[$j][2] 1`;
    }
}

# add a failover group to a cluster
for ($i = 0; $i < scalar(@$group); $i++)
{
    next if (scalar(@{$group->[$i]}) == 0);
    $ret = `$clpcreate add grp failover $group->[$i][0]`;
}

# add a resouce to a group
for ($i = 0; $i < scalar(@$resource); $i++)
{
    next if (scalar(@{$resource->[$i]}) == 0);
    for ($j = 0; $j < scalar(@{$resource->[$i]}); $j++)
    {
        next if (scalar(@{$resource->[$i]->[$j]}) == 0);
        $ret = `$clpcreate add rsc $group->[$i][0] $resource->[$i][$j][0] $resource->[$i][$j][1]`;
        for ($k = 2; $k < scalar(@{$resource->[$i]->[$j]}); $k++)
        {
            $ret = `$clpcreate add rscparam $resource->[$i][$j][0] $resource->[$i][$j][1] $resource->[$i][$j][$k][0] $resource->[$i][$j][$k][1]`;
        }
    }
}

# add a resource dependence
for ($i = 0; $i < scalar(@$rscdepend); $i++)
{
    next if (scalar(@{$rscdepend->[$i]}) == 0);
    for ($j = 0; $j < scalar(@$resource); $j++)
    {
        next if (scalar(@{$resource->[$j]}) == 0);
        for ($k = 0; $k < scalar(@{$resource->[$j]}); $k++)
        {
            next if (scalar(@{$resource->[$j][$k]}) == 0);
            if ($resource->[$j][$k][1] eq $rscdepend->[$i][1])
            {
                $ret = `$clpcreate add rscdep $resource->[$j][$k][0] $resource->[$j][$k][1] $rscdepend->[$i][0]`;
            }
        }
    }
}

# add a monitor resource to a cluster
for ($i = 0; $i < scalar(@$monitor); $i++)
{
    next if (scalar(@{$monitor->[$i]}) == 0);
    $ret = `$clpcreate add mon $monitor->[$i][0] $monitor->[$i][1]`;
    for ($j = 2; $j < scalar(@{$monitor->[$i]}); $j++)
    {
        $ret = `$clpcreate add monparam $monitor->[$i][0] $monitor->[$i][1] $monitor->[$i][$j][0] $monitor->[$i][$j][1]`;
    }
    if ($monitor->[$i][0] eq 'userw')
    {
        $ret = `$clpcreate add monparam $monitor->[$i][0] $monitor->[$i][1] relation/type cls`;
        $ret = `$clpcreate add monparam $monitor->[$i][0] $monitor->[$i][1] relation/name LocalServer`;
    }
    else
    {
        if ($type == 0)
        {
            # do nothing
            $ret = 0;
        }
        elsif ($type == 1)
        {
            $ret = `$clpcreate add monparam $monitor->[$i][0] $monitor->[$i][1] emergency/threshold/restart 0`;
            $ret = `$clpcreate add monparam $monitor->[$i][0] $monitor->[$i][1] emergency/timeout/notreconfirmation/use 1`;
            $ret = `$clpcreate add monparam $monitor->[$i][0] $monitor->[$i][1] emergency/timeout/notrecovery/use 1`;
        }
        elsif ($type == 2)
        {
            # set keepalive panic (10)
            $ret = `$clpcreate add monparam $monitor->[$i][0] $monitor->[$i][1] relation/type cls`;
            $ret = `$clpcreate add monparam $monitor->[$i][0] $monitor->[$i][1] relation/name LocalServer`;
            $ret = `$clpcreate add monparam $monitor->[$i][0] $monitor->[$i][1] emergency/action 10`;
        }
    }
}

# add object number
my $srvnum = 0;
for ($i = 0; $i < scalar(@$server); $i++)
{
    $srvnum++ if (scalar(@{$server->[$i]}) != 0);
}
my $hbnum = 0;
for ($i = 0; $i < scalar(@$hb); $i++)
{
    $hbnum++ if (scalar(@{$hb->[$i]}) != 0);
}
my $diskhbnum = 0;
for ($i = 0; $i < scalar(@$diskhb); $i++)
{
    $diskhbnum++ if (scalar(@{$diskhb->[$i]}) != 0);
}
my $npnum = 0;
for ($i = 0; $i < scalar(@$pingnp); $i++)
{
    $npnum++ if (scalar(@{$pingnp->[$i]}) != 0);
}
my $grpnum = 0;
for ($i = 0; $i < scalar(@$group); $i++)
{
    $grpnum++ if (scalar(@{$group->[$i]}) != 0);
}
my $rscnum = 0;
for ($i = 0; $i < scalar(@$resource); $i++) 
{
    next if (scalar(@{$resource->[$i]}) == 0);
    for ($j = 0; $j < scalar(@{$resource->[$i]}); $j++) 
    {
        $rscnum++ if (scalar(@{$resource->[$i][$j]}) != 0);
    }
}
my $monnum = 0;
for ($i = 0; $i < scalar(@$monitor); $i++)
{
    $monnum++ if (scalar(@{$monitor->[$i]}) != 0);
}

my $objnum = $srvnum + ($srvnum * ($hbnum + $diskhbnum + $npnum)) + $grpnum + $rscnum + $monnum + 4;
$ret = `$clpcreate add objnum $objnum`;

$ret = `xmllint --format --output clp.conf clp.conf`

# convert CRLF to LF

# Debug
#print "objnum is $objnum\n";
#print "server:$srvnum, hb:$hbnum, diskhb:$diskhbnum, np:$npnum, grp:$grpnum, rsc:$rscnum, mon:$monnum\n"
