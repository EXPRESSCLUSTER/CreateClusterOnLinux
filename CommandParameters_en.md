# Command Parameters
- This document describe the parameters that you can specify as an argument of clpcreate.
- Please refer to EXPRESSCLUSTER Reference Guide for the character strings that can be specified or not allowed to include.

## Contents
- [Initialization of cluster configuration file](#initialization)
- [Cluster](#cluster)
- [Server](#server)
- [IP address](#ip)
- [Kernel mode LAN heartbeat](#khb)
- [LAN heartbeat](#hb)
- [Disk hearbeat](#diskhb)
- [BMC heartbeat](#bmchb)
- [Network partition resolution resource](#np)
- [Group](#group)
- [Resource](#resource)
- [Resource dependency](#rscdepend)
- [Monitor resource](#mon)
- [Cluster parameters](#clsparam)
- [Group parameters](#grpparam)
- [Resource parameters](#rscparam)
- [Monitor resource parameters](#monparam)

## [Initialization of cluster configuration file](#initialization)
```bash
$ clpcreate init <encode>
```
- Input the following parameter depending language setting of EXPRESSCLUSTER.
  - ASCII: English
  - GB2312: Chinese
  - EUC-JP: Japanese

## [Cluster](#cluster)
```bash
# Add cluster
$ clpcreate add cls <cluster name> <character code> <encode> <OS>

# Add cluster parameter
$ clpcreate add clsparam <tag> <parameter>
```
- Character code: Input the following parameter depending language setting of EXPRESSCLUSTER.
  - ASCII: English
  - GB2312: Chinese
  - EUC-JP: Japanese
- Encode: The parameter that is defined when creating clp.conf on WebUI. It depends on the language setting of EXPRESSCLUSTER and the server where WebUI is running.
  - Windows OS: SJIS
  - Linux OS / English: ASCII
  - Linux OS / Chinese: GB2312
  - Linux OS / Japanese: EUC-JP
- OS: windows of linux
- Please refer to [Cluster parameters](#clsparam) for tag name and each parameters.
- The following is an example in sample scripts.
  ```perl
  my %cluster = (name=>'cluster', encode=>'EUC-JP', os=>'linux');
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
   : 
  # add a cluster to initial configuration
  $ret = `$clpcreate add cls $cluster{'name'} $cluster{'encode'} $cluster{'os'}`;

  # add a cluster parameter
  for ($i = 0; $i < scalar(@$cluster_param); $i++)
  {
      next if (scalar(@{$cluster_param->[$i]}) == 0);
      $ret = `$clpcreate add clsparam $cluster_param->[$i][0] $cluster_param->[$i][1]`;
  }
  ```

## [Server](#server)
```bash
$ clpcreate add srv <server name> <priority>
``` 
- Server name: The server name in a cluster (e.g. The output of hostname command). Alphabets must be small letters.
- Priority: The master server's priority is 0. The server priority increases one by one.
  - If a cluster consists of 2 node (server1 and server2) and server1 is the master server, server1's priority is 0 and server2's priority is 1.
- The following is an example in sample scripts.
  ```perl
  # $server
  #  Top of the list is master node.
  my $server =
  [
      ['server1'],
      ['server2'],
      [] # The last element must be an empty array.
  ];
   : 
  # add a server to a cluster
  for ($i = 0; $i < scalar(@$server); $i++)
  {
      next if (scalar(@{$server->[$i]}) == 0);
      $ret = `$clpcreate add srv $server->[$i][0] $i`;
  }
  ```

## [IP address](#ip)
```bash
$ clpcreate add ip <server name> <device id> <ip address>
```
- Server name: The server name in a cluster.
- Device ID: The device ID for the IP address of the server.The device ID increases one by one.
  - The device ID of LAN hearbeat and Kernel mode LAN hearbet start from 0.
    - e.g. lanhb1 is 0, lankhb1 is 1, lanhb2 is 2.
  - The device ID is independent for each servers. (All cluster nodes have the device ID 0.)
- IP address: The IP address that is used for Kernel mode LAN heartbeat or LAN heartbeat
- The following is an example in sample scripts.
  ```perl
  # $ip
  #  Set IP addresses for the servers.
  #  On this case, upper IP addresses for server1 and bottom IP addresses for server2.
  my $ip =
  [
      ['192.168.137.71', '192.168.138.71', '192.168.139.71', '192.168.140.71'],
      ['192.168.137.72', '192.168.138.72', '192.168.139.72', '192.168.140.72'],
      []
  ];
   :
  # add an IP address to a server
  for ($i = 0; $i < scalar(@$server); $i++)
  {
      next if (scalar(@{$server->[$i]}) == 0);
      for ($j = 0; $j < scalar(@{$ip->[$i]}); $j++)
      {
          $ret = `$clpcreate add ip $server->[$i][0] $j $ip->[$i][$j]`;
      }
  }
  ```
  
## [Kernel mode LAN heartbeat](#khb)
```bash
$ clpcreate add khb <device id> <priority>
```
- Device ID: The device ID of the IP address
- Priority: The priority of heartbeat starts from 0.
- The following is an example in sample scripts.
  ```perl
  # $khb
  #  Left : Device ID to be used for kernel heartbeat on primary and secondary server
  #  Right: Priority of the kernel heartbeat
  my $khb =
  [
      ['0', '0'],
      ['1', '1'],
      ['2', '2'],
      ['3', '3'],
      []
  ];
   :
  # add a kernel heartbeat interface to a cluster
  for ($i = 0; $i < scalar(@$khb); $i++)
  {
      next if (scalar(@{$khb->[$i]}) == 0);
      $ret = `$clpcreate add khb $khb->[$i][0] $khb->[$i][1]`;
  }
  ```
  
## [LAN heartbeat](#hb)
```bash
$ clpcreate add hb <device id> <priority>
```
- Device ID: The device ID of the IP address
- Priority: The priority of heartbeat starts from 0.

## [Disk heartbeat](#diskhb)
```bash
$ clpcreate add diskhb <device id> <priority>
$ clpcreate add diskhbsrv <device id> <disk device name>
```
- You need to execute 2 commands to add a disk heartbeat.
  - clpcreate add diskhb
  - clpcreate add diskhbsrv
- Device ID: The device ID of a disk heartbeat starts from 300. The device ID increases one by one.
- Priority: The priority of heartbeat starts from 0.
- Disk device name: The path of the device for disk heart beat.
  - In current clpcreate version, the paths on both servers must be same.
- The following is an example in sample scripts.
  ```perl
  # $diskhb
  #  Device ID, Heartbeat priority, Disk path
  my $diskhb =
  [
      ['300', '4', '/dev/sdc1'],
      []
  ];
   :
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
  ```

## [Network partition resolution resource (PING method)](#np)
```bash
$ clpcreate add np ping <priority> <device id> <group id> <list id> <ip address>
$ clpcreate add npsrv ping <server name> <device id> <use>
```
- To add a Network Partition Resolution Resource, execute 2 commands.
  - clpcreate add np
  - clpcreate add npsrv
- Priority: The priority of a NP resolution resource starts from 0.
- Device ID: The device ID of a NP resolution resource starts from 10200. The device ID increases one by one.
- Group ID: The group ID of a NP resolution resource starts from 0. The group ID increases one by one.
- List ID: The list ID for IP address starts from 0. The list ID increases one by one.
- IP address: The IP address that is used for ping NP resolution resource.
- Server name: The server name in a cluster.
- Use: For each server, specify whether using ping NP or not. If the server use the ping NP, Use is 1. If not, Use is empty string "".
- The following is an example in sample scripts.
  ```perl
  # $pingnp
  #  Name, NP priority, Device ID, Group ID, [List ID, IP address], ...
  my $pingnp =
  [
      ['0', '10200', '0', ['0', '192.168.137.1'], ['1', '192.168.137.76'], []],
      []
  ];
   :
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
  ```
  
## [BMC heartbeat](#bmchb)
```bash
$ clpcreate add bmchbsrv <server name> <device id> <ip address>
$ clpcreate add bmchb <device id> <priority>
```
- Server name: The server name in a cluster.
- Device ID: The device ID of BMC heartbeat starts from 600. The device ID increases one by one.
- Priority: The priority of BMC heartbeat starts from 0.
- IP address: The IP address that is used for BMC heartbeat.

## [Group](#group)
```bash
# Add a group
$ clpcreate add grp <group type> <group name>

# Set a parameter to a group
$ clpcreate add grpparam <group type> <group name> <tag> <parameter>
```
- Group type: The type of a failover group is **failover**.
- Group name: The name of a group.
- For the detail of tags and parameters, please refer to [Group parameters](#grpparam)
- The following is an example in sample scripts.
  ```perl
  my $group =
  [
      ['failover'],
      []
  ];
   :
  # add a failover group to a cluster
  for ($i = 0; $i < scalar(@$group); $i++)
  {
      next if (scalar(@{$group->[$i]}) == 0);
      $ret = `$clpcreate add grp failover $group->[$i][0]`;
  }
  ```

## [Resource](#resource)
```bash
# Add a resource
$ clpcreate add rsc <group name> <resource type> <resource name>

# Set a parameter to a resource
$ clpcreate add rscparam <resource type> <resource name> <tag> <parameter>
```
- Group name: The name of a group which the resource is added to.
- Resource type: The type of a resource (e.g. fip, disk, exec).
- Resource name: The name of a resource.
- For the detail of tags and parameters, please refer to [Resource parameters](#rscparam)
- The following is an example in sample scripts.
  ```perl
  my $resource =
  [
      [
          ['fip', 'fip1', ['parameters/ip', '192.168.137.70']],
          ['volmgr', 'volmgr1', ['parameters/type', 'lvm'], ['parameters/devname', 'ecxsd']],
          ['disk', 'disk1', ['parameters/disktype', 'lvm'], ['parameters/device', '/dev/ecxsd/sd1'], ['parameters/mount/point', '/mnt/sd1'], ['parameters/fs', 'ext3']],
         ['exec', 'exec1', ['parameters/act/path', 'start.sh'], ['parameters/deact/path', 'stop.sh']],
          []
      ],
      []
  ];
   :
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
  ```

## [Resource dependency](#rscdepend)
```bash
$ clpcreate add rscdep <depended resource name> <depending resource name>
```
- When a group is starting, the depending resource is started after the depended resource has been started.
- When a group is stopping, the depended resource is stopped after the depending resource has been stopped.
- The following is an example in sample scripts.
  ```perl
  my $rscdepend =
  [
      ['volmgr1', 'disk1'],
      []
  ];
   :
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
  ```

## [Monitor resource](#mon)
```bash
# Add a monitor resource
$ clpcreate add mon <monitor type> <monitor name>

# Set a parameter to a monitor resource
$ clpcreate add monparam <monitor type> <monitor name> <tag> <parameter>
```
- Monitor type: The type of a monitor resource (e.g. diskw, fipw, pidw).
- For the detail of tags and parameters, please refer to [Monitor resource parameters](#monparam)
- The following is an example in sample scripts.
  ```perl
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
   : 
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
          $ret = `$clpcreate add monparam $monitor->[$i][0]   $monitor->[$i][1] relation/name LocalServer`;
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
              $ret = `$clpcreate add monparam $monitor->[$i][0] $monitor->[$i][1] emergency/timeout/notrecovery/use   1`;
          }
          elsif ($type == 2)
          {
              # set keepalive panic (10)
              $ret = `$clpcreate add monparam $monitor->[$i][0] $monitor->[$i][1] relation/type cls`;
              $ret = `$clpcreate add monparam $monitor->[$i][0] $monitor->[$i][1] relation/name LocalServer`;
              $ret = `$clpcreate add monparam $monitor->[$i][0]   $monitor->[$i][1] emergency/action 10`;
          }
      }
  }
  ```

## [Cluster parameters](#clsparam)
#### Recovery
- pm/exec0/recover: Action When the Cluster Service Process Is Failure
- pm/exec1/recover
- pm/exec2/recover
  - Set the same value to the 3 tags above.
    ```bash
    $ clpcreate add clsparam pm/exec0/recover 7
    $ clpcreate add clsparam pm/exec1/recover 7
    $ clpcreate add clsparam pm/exec2/recover 7
    ```
  |Value|Final Action|Note|
  |----|---------|--|
  |   2|Shut down the OS||
  |   3|Reboot the OS||
  |   5|Sysrq Panic||
  |   6|Keepalive Reset||
  |   7|Keepalive Panic||
  |   8|BMC Reset||
  |   9|BMC Power Off||
  |  10|BMC Power Cycle||
  |  11|BMC NMI||
  |  12|I/O-Fencing(High-End Server Option)||

- cluster/rsctimeout/rsctoaction: Action at Group Resource Activation or Deactivation Stall
  ```bash
  $ clpcreate add clsparam cluster/rsctimeout/rsctoaction 0
  ```
  |Value|Final Action|Note|
  |----|---------|--|
  |Default|Stop the cluster service and shutdown the OS||
  |   0|No operation (Operates as an activity or deactivity failure)||
  |   3|Stop the cluster service and reboot the OS||
  |   8|Sysrq Panic||
  |   9|Keepalive Reset||
  |  10|Keepalive Panic||
  |  11|BMC Reset||
  |  12|BMC Powers Off||
  |  13|BMC Power Cycle||
  |  14|BMC NMI||
  |  15|I/O-Fencing(High-End Server Option)||

#### NP Resolution
- cluster/networkpartition/npaction: Action at NP Occurence
  ```bash
  $ clpcreate add clsparam cluster/networkpartiton/npaction 6
  ```
  |Value|Final Action|Note|
  |----|---------|--|
  |Default|Stop the cluster service and shutdown the OS||
  |   0|No operation (Operates as an activity or deactivity failure)||
  |   1|Stop the cluster service||
  |   2|Stop the cluster service and shutdown OS||
  |   3|Stop the cluster service and reboot OS||
  |   4|Sysrq Panic||
  |   5|Keepalive Reset||
  |   6|Keepalive Panic||
  |   7|BMC Reset||
  |   8|BMC Power Off||
  |   9|BMC Power Cycle||
  |  10|BMC NMI||
  |  11|I/O-Fencing(High-End Server Option)||

#### Monitor
- haltp/method: Method
  - softdog
  - keepalive
    ```bash
    $ clpcreate add clsparam haltp/method keepalive
    ```
    
- haltp/action: Operation at Timeout Detection
  - RESET
  - PANIC (If you use softdog as monitor method, PANIC is set automatically. Do not add this tag.)
    ```bash
    $ clpcreate add clsparam haltp/action PANIC
    ```

#### WebManager
- webmgr/security/clientlist/iprest: Control connection by using client IP address
  - 0: Do not control
  - 1: Control
  ```
  $ clpcreate add clsparam webmgr/security/clientlist/iprest 1
  ```
  - You can specify the IP address that is allowed to access WebUI by the following command. 
    ```
    $ clpcreate add webmgr clientlist 192.168.100.1
    $ clpcreate add webmgr clientlist 192.168.100.0/24
    ```

## [Group parameters](#grpparam)
#### Startup Server
- You can specify the server that the group can start up on. (By default, a group can start up on all servers.)
- policy@\<server name\>/order: Server and Order
- Order 0 is the highest.
  ```bash
  $ clpcreate add grpparam failover failover1 policy@server1/order 0
  $ clpcreate add grpparam failover failover1 policy@server2/order 1
  ```

## [Resource parameters](#rscparam)
### Common parameters
#### Recovery Operation
- act/retry: Retry Count (at Activation Failure)
- act/fo: Failover Threshold
- act/action: Final Action (at Activation Failure)
- deact/retry: Retry Count at Deactivation Failure
- deact/action: Final Action (at Dectivation Failure)
  ```bash
  $ clpcreate add rscparam fip fip1 act/retry 1
  $ clpcreate add rscparam fip fip1 act/fo 1
  $ clpcreate add rscparam fip fip1 act/action 2
  $ clpcreate add rscparam fip fip1 deact/retry 1
  $ clpcreate add rscparam fip fip1 deact/action 5
  ```
  |Value|Final Action|Note|
  |----|---------|--|
  |   0|No operation (active next resource)||
  |   1|No operation (not active next resource)||
  |   2|Stop group|Only when activation failure|
  |   3|Stop the cluster service||
  |   4|Stop the cluster service and shutdown OS||
  |   5|Stop the cluster service and reboot OS||
  |   8|Sysrq Panic||
  |   9|Keepalive Reset||
  |  10|Keepalive Panic||
  |  11|BMC Reset||
  |  12|BMC Power Off||
  |  13|BMC Power Cycle||
  |  14|BMC NMI||

### Disk resource (type: disk)
- parameters/disktype: Disk Type
  - disk: For a normal disk
  - lvm: For a logical volume on a volume group
- parameters/device: Device Name
- parameters/mount/point: Mount Point
- parameters/fs: File System
  - ext3, ext4, xfs
- parameters/fsck/timing: fsck Action Before Mount
  - 0: Not Execute
  - 1: Always Execute
  - 2: Execute at Specified Count
    - parameters/fsck/interval: Count

### EXEC resource  (type: exec)
- parameters/act/path: The absolute path to Start script
  - If you use **Script created with this product**, the value of this tag is **start.sh**.
  - If you use **User Application**, the value of this tag is arbitrary path (e.g. /opt/test/start.sh)
- parameters/deact/path: The absolute path to Stop script
  - If you use **Script created with this product**, the value of this tag is **stop.sh**.
  - If you use **User Application**, the value of this tag is arbitrary path (e.g. /opt/test/stop.sh)
- parameters/timeout/start: Timeout (of Start Script)
- parameters/timeout/stop: Timeout (of Stop Script)
- parameters/act/sync: Synchronous or Asynchronous (of Start Script)
  - 0: Asynchronous
  - 1: Synchronous
- parameters/deact/sync: Synchronous or Asynchronous (of Stop Script)
  - 0: Asynchronous
  - 1: Synchronous

### Floating IP resource (type: fip)
- parameters/ip: IP Address
  - Floating IP address should belong to the same subnet with one of heartbeat interconnect.

### Volume manager resource (type: volmgr)
- parameters/type: Volume Manager
  - lvm: For a logical volume on a volume group
- parameters/devname: Target Name
  - If you use LVM, please specify the volume group name.

## [Monitor resource parameters](#monparam)
### Common parameters
#### Monitor(common)
- polling/interval: Interval
- polling/timeout: Timeout
- emergency/dumpcollect/use: Collect the dump file of the monitor process at timeout occurrence
  - This parameter cannot be used for usew and genw.
  - 0: Uncheck
  - 1: Check
- emergency/timeout/notreconfirmation/use: Do Not Retry at Timieout Occurrence
  - 0: Uncheck
  - 1: Check
- emergency/timeout/notrecovery/use: Do Not Retry at Timieout Occurrence
  - 0: Uncheck
  - 1: Check
- polling/reconfirmation: Retry Count
  ```bash
  $ clpcreate add monparam fipw fipw1 polling/interval 60
  $ clpcreate add monparam fipw fipw1 polling/timeout 120
  $ clpcreate add monparam fipw fipw1 polling/reconfirmation 1
  ```
- polling/timing: Monitor Timing
  - 0: Always
  - 1: Active
    - If you change a monitor timing from Always to Active, please also specify the target.
- target: Target Resource
  - When polling/timing is 0 (Monitor Timing is Always), do not set this parameter.
    ```bash
    $ clpcreate add monparam fipw fipw1 target fip1
    ```
- firstmonwait: Wait Time to Start Monitoring
- polling/servers@\<id\>/name: Choose servers that execute monitoring
  - If you execute the monitoring on all servers, no need to set this parameter.
  - The sample command for monitoring on 1 server.
    ```bash
    $ clpcreate add monparam fipw fipw1 polling/servers@0/name <server name>
    ```
  - The sample command for monitoring on 2 or more server.
    ```bash
    $ clpcreate add monparam fipw fipw1 polling/servers@0/name <server name>
    $ clpcreate add monparam fipw fipw1 polling/servers@1/name <server name>
    ```

#### Recovery Action
- Only Custom settings is supported.
- However by setting appropriate parameters, you can set the same settings with templates (e.g. Execute only the final action)
  - Executing failover to the recovery target
    ```bash
    $ clpcreate add monparam genw genw1 emergency/threshold/restart 0
    $ clpcreate add monparam genw genw1 emergency/threshold/fo 1
    ```
  - Restart the recovery target, and if there is no effect with restart, then failover
    ```bash
    $ clpcreate add monparam genw genw1 emergency/threshold/restart 1
    $ clpcreate add monparam genw genw1 emergency/threshold/fo 1
    ```
  - Restart the recovery target
    ```bash
    $ clpcreate add monparam genw genw1 emergency/threshold/restart 1
    $ clpcreate add monparam genw genw1 emergency/threshold/fo 0
    ```
  - Execute only the final action
    ```bash
    $ clpcreate add monparam genw genw1 emergency/threshold/restart 0
    $ clpcreate add monparam genw genw1 emergency/threshold/fo 0
    ```
  - Custom settings
    - Please set the following parameters on demand.
- The detail of parameters
- Recovery Target
  - If the target is a resource
    ```bash
    $ clpcreate add monparam <monitor type> <monitor name> relation/type rsc
    $ clpcreate add monparam <monitor type> <monitor name> relation/name <resource name>
    ```
  - If the target is a group
    ```bash
    $ clpcreate add monparam <monitor type> <monitor name> relation/type grp
    $ clpcreate add monparam <monitor type> <monitor name> relation/name <group name>
    ```
  - If the target is LocalServer
    ```bash
    $ clpcreate add monparam <monitor type> <monitor name> relation/type cls
    $ clpcreate add monparam <monitor type> <monitor name> relation/name LocalServer
    ```
- emergency/threshold/script: Recovery Script Execution Count
- emergency/preaction/userestart: Execute Script before Reactivation
  - 0: Uncheck
  - 1: Check
- emergency/threshold/restart: Maximum Reactivation Count
  ```bash
  $ clpcreate add monparam genw genw1 emergency/threshold/restart 1
  ```
- emergency/preaction/usefailover: Execute Script before Failover
  - 0: Uncheck
  - 1: Check
- emergency/threshold/fo: Maximum Failover Count
  ```bash
  $ clpcreate add monparam genw genw1 emergency/threshold/fo 1
  ```
- emergency/preaction/usefailover: Execute Script before Final Action
  - 0: Uncheck
  - 1: Check
- emergency/action: Final Action
  ```bash
  $ clpcreate add monparam genw genw1 emergency/action 3
  ```
  |Value|Final Action|
  |----|---------|
  |   1|No operation|
  |   2|Stop group|
  |   3|Stop the cluster service|
  |   4|Stop the cluster service and shutdown OS|
  |   5|Stop the cluster service and reboot OS|
  |   8|Sysrq Panic
  |   9|Keepalive Reset|
  |  10|Keepalive Panic|
  |  11|BMC Reset|
  |  12|BMC Power Off|
  |  13|BMC Power Cycle|
  |  14|BMC NMI|
  |  15|I/O Fencing(High-End Server Option)|
  |  16|Stop resource|

- Script Settings
  - emergency/preaction/default: The type of a script
    - 0: User Application
    - 1: Script created with this product
  - emergency/preaction/path: File
    - User application: Arbitrary path of the script
    - Script created with this product: preaction.sh
  - emergency/preaction/timeout: Timeout


### Floating IP monitor resource (type: fipw)
- parameters/monmii: Monitor NIC Link Up/Down
  - 0: Uncheck (Default)
  - 1: Check
    ```bash
    $ clpcreate add monparam genw genw1 parameters/monmii 1
    ```

### Custom monitor resource (type: genw)
- parameters/path
  - Script created with this product: genw.sh
  - User application: Arbitrary path of the script (e.g. /opt/test/genw.sh)
    ```bash
    $ clpcreate add monparam genw genw1 parameters/path genw.sh
    ```

### Disk RW monitor resource (type: diskw)
- parameters/object: Monitor Target
  - The path of monitored disk
    ```bash
    $ clpcreate add monparam diskw diskw1 parameters/object /dev/sdc2
    ```

### IP monitor resource (type: ipw)
- parameters/list@\<id\>/ip: IP Address List
  - The sample command for monitoring on 1 IP address.
    ```bash
    $ clpcreate add monparam ipw ipw1 parameters/list@0/ip <IP address>
    ```
  - The sample command for monitoring on multiple IP addresses.
    ```bash
    $ clpcreate add monparam ipw ipw1 parameters/list@0/ip <IP address>
    $ clpcreate add monparam ipw ipw1 parameters/list@1/ip <IP address>
    ```

### User mode monitor resource (type: userw)
- parameters/method: Method
  - keepalive: The default method since X 4.x
  - softdog: The default method until X 3.x
    ```bash
    $ clpcreate add monparam userw userw1 parameters/method keepalive
    ```
- parameters/action: Operation at Timeout Detection
  - RESET
  - PANIC
    ```bash
    $ clpcreate add monparam userw userw1 parameters/action PANIC
    ```
  - If you use softdog as monitoring method, RESET is set automatically. Do not change this parameter.

### Volume manager monitor resource (type: volmgrw)
- parameters/devname: Target Name
  ```bash
  $ clpcreate add monparam volmgrw volmgrw parameters/devname <volume group name>
  ```

### PID monitor resource (type: pidw)
- There are no specific parameters.