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
#  charset:
#    This parameter depends on the language of CLUSTERPRO
#    Japanese: EUC-JP
#    English : ASCII
#    Chinese : GB2312
#  encode:
#    This parameter depends on the language of a server that you use to access WebUI
#    If you use WebUI on Windows machine
#      SJIS
#    If you use WebUI on Linux macine
#      EUC-JP: Japanese
#      ASCII : ASCII
#      GB2312: Chinese
#  os:
#   windows or linux
my %cluster = (name=>'cluster', charset=>'EUC-JP', encode=>'EUC-JP', os=>'linux');
# $cluster_param
#  tag, parameter
my $cluster_param =
[
    ['pm/exec0/recover', '7'],
    ['pm/exec1/recover', '7'],
    ['pm/exec2/recover', '7'],
    ['cluster/rsctimeout/rsctoaction', '0'],
    ['cluster/networkpartition/npaction', '6'],
    ['haltp/method', 'keepalive'],
    ['haltp/action', 'PANIC'],
    []
];
#
# $server
#  Top of the list is master node.
my $server =
[
    ['rhel76-217'],
    ['rhel76-218'],
    []
];
# 
# $ip
#  Set IP addresses for the servers.
#  On this case, upper IP addresses for server1 and bottom IP addresses for server2.
my $ip =
[
    ['192.168.0.217', '192.168.1.217'],
    ['192.168.0.218', '192.168.1.218'],
    []
];
#
# $khb
#  Left: Device ID to be used for kernel heartbeat on primary server and secondary server
#  Right: Priority of the kernel heartbeat
my $khb =
[
    ['0', '0'],
    ['1', '1'],
    []
];
# 
# $diskhb
#  Device ID, Heartbeat priority, Disk path
my $diskhb =
[
    ['300', '2', '/dev/sdc1'],
    []
];
#
# $pingnp
#  Name, NP priority, Device ID, Group ID, [List ID, IP address], ...
my $pingnp =
[
    ['0', '10200', '0', ['0', '192.168.1.144'], []],
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
        ['fip', 'fip1', ['parameters/ip', '192.168.1.219']],
        ['disk', 'disk1', ['parameters/disktype', 'disk'], ['parameters/device', '/dev/sdc2'], ['parameters/mount/point', '/mnt/sd1'], ['parameters/fs', 'xfs']],
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
    []
];
#
# $monitor
#
my $monitor =
[
    ['userw', 'userw', ['parameters/method', 'keepalive']],
    ['fipw', 'fipw1', ['parameters/monmii', '1'], ['target', 'fip'], ['relation/type', 'grp'], ['relation/name', $group->[0][0]]],
    ['diskw', 'diskw1', ['parameters/object', '/dev/sdc2'], ['relation/type', 'grp'], ['relation/name', $group->[0][0]]],
    ['ipw', 'ipw1', ['parameters/list@0/ip', '192.168.1.144'], ['relation/type', 'grp'], ['relation/name', $group->[0][0]]],
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
$ret = `$clpcreate add cls $cluster{'name'} $cluster{'charset'} $cluster{'encode'} $cluster{'os'}`;

# add a cluster parameter
for ($i = 0; $i < scalar(@$cluster_param); $i++)
{
    next if (scalar(@{$cluster_param->[$i]}) == 0);
    $ret = `$clpcreate add clsparam $cluster_param->[$i][0] $cluster_param->[$i][1]`;
}

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

# add a kernel heartbeat interface to a cluster
for ($i = 0; $i < scalar(@$khb); $i++)
{
    next if (scalar(@{$khb->[$i]}) == 0);
    $ret = `$clpcreate add khb $khb->[$i][0] $khb->[$i][1]`;
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
    for ($j = 3; $j < scalar(@{$pingnp->[$i]}); $j++)
    {
        next if (scalar(@{$pingnp->[$i][$j]}) == 0);
        $ret = `$clpcreate add np ping $pingnp->[$i][0] $pingnp->[$i][1] $pingnp->[$i][2] $pingnp->[$i][$j][0] $pingnp->[$i][$j][1]`;
    }
}

# add a network partition resource to a server
for ($i = 0; $i < scalar(@$server); $i++)
{
    next if (scalar(@{$server->[$i]}) == 0);
    for ($j = 0; $j < scalar(@$pingnp); $j++)
    {
        next if (scalar(@{$pingnp->[$j]}) == 0);
        $ret = `$clpcreate add npsrv ping $server->[$i][0] $pingnp->[$j][1] 1`;
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

#my $objnum = $srvnum + ($srvnum * ($hbnum + $diskhbnum + $npnum)) + $grpnum + $rscnum + $monnum + 4;
#$ret = `$clpcreate add objnum $objnum`;

$ret = `xmllint --format --output clp.conf clp.conf`

# convert CRLF to LF

# Debug
#print "objnum is $objnum\n";
#print "server:$srvnum, hb:$hbnum, diskhb:$diskhbnum, np:$npnum, grp:$grpnum, rsc:$rscnum, mon:$monnum\n"
