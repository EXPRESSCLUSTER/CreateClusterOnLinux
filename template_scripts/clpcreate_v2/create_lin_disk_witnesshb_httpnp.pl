#!/usr/bin/perl

#use strict;
use warnings;

##################################################
# Sample script for Linux
##################################################
# Cluster configuration
# -------------------------------------------------
# 2 cluster nodes
# Heartbeat
#   2 Kernel mode heartbeat
#   1 Disk heartbeat
#   1 Witness heartbeat
# NP resolution resource
#   - http
# Failover group
#   - Floating IP address
#   - Disk
#   - Exec
# User mode monitor
# Floating IP address monitor
# Disk monitor
#
#-------------------------------------------------
# Parameters
#-------------------------------------------------
# $clpcfset
#  Set the relative path for clpcfset.
my $clpcfset = './clpcfset';
#
# %cluster
#  encode:
#    This parameter depends on the language of CLUSTERPRO
#    Japanese: EUC-JP
#    English : ASCII
#    Chinese : GB2312
#  webui:
#    This parameter depends on the language of a server on which you create a clp.conf on WebUI
#    If you use WebUI on Windows machine
#      SJIS
#    If you use WebUI on Linux macine
#      EUC-JP: Japanese
#      ASCII : ASCII
#      GB2312: Chinese
#  os:
#   windows or linux
my %cluster = (name=>'cluster', encode=>'EUC-JP', webui=>'EUC-JP', os=>'linux');
# $cluster_param
#  tag, parameter
my $cluster_param =
[
    []
];
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
# $device
#  servernama, type, device ID, info, [extend]
my $device =
[
    # Devices of 1st server
    [
        ['lan', '0', '192.168.137.71'],
        ['lan', '1', '192.168.138.71'],
        ['disk', '300', '/dev/sdd1'],
        ['witness', '700', '1', '192.168.137.75:80'],
        ['http', '10700', '1'],
        []
    ],
    # Devices of 2nd server
    [
        ['lan', '0', '192.168.137.72'],
        ['lan', '1', '192.168.138.72'],
        ['disk', '300', '/dev/sdd1'],
        ['witness', '700', '1', '192.168.137.75:80'],
        ['http', '10700', '1'],
        []
    ],
    []
];
#
# $hb
#  type, device ID, priority
my $hb =
[
    ['lankhb', '0', '0'],
    ['lankhb', '1', '1'],
    ['diskhb', '300', '2'],
    ['witnesshb', '700', '3', '192.168.137.75:80'],
    []
];
#
# $pingnp
#  Type, Device ID, NP priority, Group ID, [List ID, IP address], ...
my $pingnp =
[
    []
];
#
# $httpnp
#  Type, Device ID, NP priority, IPaddress:port
my $httpnp =
[
    ['httpnp', '10700', '0', '192.168.137.75:80'],
    []
];
#
# $group
#  Name
my $group =
[
    ['failover1'],
    []
];
#
# $resource
#
my $resource =
[
    # Resources in 1st failover group
    [
        ['fip', 'fip1', ['parameters/ip', '192.168.137.70']],
        ['disk', 'disk1', ['parameters/disktype', 'disk'], ['parameters/device', '/dev/sdd2'], ['parameters/mount/point', '/mnt/disk'], ['parameters/fs', 'ext3']],
        ['exec', 'exec1', ['parameters/act/path', 'start.sh'], ['parameters/deact/path', 'stop.sh']],
        []
    ],
    # Resources in 2nd failover group
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
    ['userw', 'userw', ['relation/type', 'cls'], ['relation/name', 'LocalServer']],
    ['fipw', 'fipw1', ['target', 'fip1'], ['relation/type', 'rsc'], ['relation/name', 'fip1']],
    ['diskw', 'diskw1', ['parameters/object', '/dev/sdd2'], ['parameters/size', '512'], ['relation/type', 'rsc'], ['relation/name', 'disk1']],
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

# create a cluster
$ret = `$clpcfset create $cluster{'name'} $cluster{'encode'} $cluster{'webui'} $cluster{'os'}`;

# add cluster parameters
for ($i = 0; $i < scalar(@$cluster_param); $i++)
{
    next if (scalar(@{$cluster_param->[$i]}) == 0);
    $ret = `$clpcfset add clsparam $cluster_param->[$i][0] $cluster_param->[$i][1]`;
}

# add servers
for ($i = 0; $i < scalar(@$server); $i++)
{
    next if (scalar(@{$server->[$i]}) == 0);
    $ret = `$clpcfset add srv $server->[$i][0] $i`;
}

# add devices
for ($i = 0; $i < scalar(@$server); $i++)
{
    next if (scalar(@{$server->[$i]}) == 0);
    for ($j = 0; $j < scalar(@{$device->[$i]}); $j++)
    {
        if (scalar(@{$device->[$i][$j]}) == 3)
        {
            $ret = `$clpcfset add device $server->[$i][0] $device->[$i][$j][0] $device->[$i][$j][1] $device->[$i][$j][2]`;
        }
        elsif(scalar(@{$device->[$i][$j]}) == 4)
        {
            $ret = `$clpcfset add device $server->[$i][0] $device->[$i][$j][0] $device->[$i][$j][1] $device->[$i][$j][2] $device->[$i][$j][3]`;
        } 
    }
}

# add heartbeat interfaces
for ($i = 0; $i < scalar(@$hb); $i++)
{
    next if (scalar(@{$hb->[$i]}) == 0);
    if (scalar(@{$hb->[$i]}) == 3)
    {
        $ret = `$clpcfset add hb $hb->[$i][0] $hb->[$i][1] $hb->[$i][2]`;
    }
    elsif(scalar(@{$hb->[$i]}) == 4)
    {
        $ret = `$clpcfset add hb $hb->[$i][0] $hb->[$i][1] $hb->[$i][2] $hb->[$i][3]`;
    }
}

# add ping NP resolution resources
for ($i = 0; $i < scalar(@$pingnp); $i++)
{
    next if (scalar(@{$pingnp->[$i]}) == 0);
    for ($j = 4; $j < scalar(@{$pingnp->[$i]}); $j++)
    {
        next if (scalar(@{$pingnp->[$i][$j]}) == 0);
        $ret = `$clpcfset add np $pingnp->[$i][0] $pingnp->[$i][1] $pingnp->[$i][2] $pingnp->[$i][3] $pingnp->[$i][$j][0] $pingnp->[$i][$j][1]`;
    }
}

# add http NP resolution resources
for ($i = 0; $i < scalar(@$httpnp); $i++)
{
    next if (scalar(@{$httpnp->[$i]}) == 0);
    $ret = `$clpcfset add np $httpnp->[$i][0] $httpnp->[$i][1] $httpnp->[$i][2] $httpnp->[$i][3]`;
}

# add failover groups
for ($i = 0; $i < scalar(@$group); $i++)
{
    next if (scalar(@{$group->[$i]}) == 0);
    $ret = `$clpcfset add grp failover $group->[$i][0]`;
}

# add resources
for ($i = 0; $i < scalar(@$resource); $i++)
{
    next if (scalar(@{$resource->[$i]}) == 0);
    for ($j = 0; $j < scalar(@{$resource->[$i]}); $j++)
    {
        next if (scalar(@{$resource->[$i]->[$j]}) == 0);
        $ret = `$clpcfset add rsc $group->[$i][0] $resource->[$i][$j][0] $resource->[$i][$j][1]`;
        for ($k = 2; $k < scalar(@{$resource->[$i]->[$j]}); $k++)
        {
            $ret = `$clpcfset add rscparam $resource->[$i][$j][0] $resource->[$i][$j][1] $resource->[$i][$j][$k][0] $resource->[$i][$j][$k][1]`;
        }
    }
}

# add resource dependencies
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
                $ret = `$clpcfset add rscdep $resource->[$j][$k][0] $resource->[$j][$k][1] $rscdepend->[$i][0]`;
            }
        }
    }
}

# add monitor resources
for ($i = 0; $i < scalar(@$monitor); $i++)
{
    next if (scalar(@{$monitor->[$i]}) == 0);
    $ret = `$clpcfset add mon $monitor->[$i][0] $monitor->[$i][1]`;
    for ($j = 2; $j < scalar(@{$monitor->[$i]}); $j++)
    {
        $ret = `$clpcfset add monparam $monitor->[$i][0] $monitor->[$i][1] $monitor->[$i][$j][0] $monitor->[$i][$j][1]`;
    }
}

# add object number
#my $srvnum = 0;
#for ($i = 0; $i < scalar(@$server); $i++)
#{
#    $srvnum++ if (scalar(@{$server->[$i]}) != 0);
#}
#my $hbnum = 0;
#for ($i = 0; $i < scalar(@$hb); $i++)
#{
#    $hbnum++ if (scalar(@{$hb->[$i]}) != 0);
#}
#my $diskhbnum = 0;
#for ($i = 0; $i < scalar(@$diskhb); $i++)
#{
#    $diskhbnum++ if (scalar(@{$diskhb->[$i]}) != 0);
#}
#my $npnum = 0;
#for ($i = 0; $i < scalar(@$pingnp); $i++)
#{
#    $npnum++ if (scalar(@{$pingnp->[$i]}) != 0);
#}
#my $grpnum = 0;
#for ($i = 0; $i < scalar(@$group); $i++)
#{
#    $grpnum++ if (scalar(@{$group->[$i]}) != 0);
#}
#my $rscnum = 0;
#for ($i = 0; $i < scalar(@$resource); $i++) 
#{
#    next if (scalar(@{$resource->[$i]}) == 0);
#    for ($j = 0; $j < scalar(@{$resource->[$i]}); $j++) 
#    {
#        $rscnum++ if (scalar(@{$resource->[$i][$j]}) != 0);
#    }
#}
#my $monnum = 0;
#for ($i = 0; $i < scalar(@$monitor); $i++)
#{
#    $monnum++ if (scalar(@{$monitor->[$i]}) != 0);
#}

#my $objnum = $srvnum + ($srvnum * ($hbnum + $diskhbnum + $npnum)) + $grpnum + $rscnum + $monnum + 4;
#$ret = `$clpcfset add objnum $objnum`;

$ret = `xmllint --format --output clp.conf clp.conf`

# convert CRLF to LF

# Debug
#print "objnum is $objnum\n";
#print "server:$srvnum, hb:$hbnum, diskhb:$diskhbnum, np:$npnum, grp:$grpnum, rsc:$rscnum, mon:$monnum\n"
