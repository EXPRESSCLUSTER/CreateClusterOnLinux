# Command Parameters
- clpcreate コマンドに指定可能なパラメータについて説明しています。
- 各パラメータに入力可能な文字列や禁則文字列については、CLUSTERPRO のリファレンスガイドを参照してください。

## 目次
- [クラスタ構成情報ファイルの初期化](#クラスタ構成情報ファイルの初期化)
- [クラスタの追加](#クラスタの追加)
- [サーバの追加](#サーバの追加)
- [IPアドレスの追加](#IPアドレスの追加)
- [カーネルモードハートビートの追加](#カーネルモードハートビートの追加)
- [ユーザモードハートビートの追加](#ユーザモードハートビートの追加)
- [ディスクハートビートの追加](#ディスクハートビートの追加)
- [BMCハートビートの追加](#BMCハートビートの追加)
- [ネットワークパーティション解決リソースの追加](#ネットワークパーティション解決リソースの追加)
- [グループの追加](#グループの追加)
- [リソースの追加](#リソースの追加)
- [リソースの依存関係の追加](#リソースの依存関係の追加)
- [モニタリソースの追加](#モニタリソースの追加)
- [クラスタのパラメータ](#クラスタのパラメータ)
- [グループのパラメータ](#グループのパラメータ)
- [リソースのパラメータ](#リソースのパラメータ)
- [モニタリソースのパラメータ](#モニタリソースのパラメータ)

## クラスタ構成情報ファイルの初期化
```bash
$ clpcreate init <エンコード>
```
- エンコードは、CLUSTERPROの言語に応じて次の値を入力してください。
  - EUC-JP: 日本語
  - ASCII: 英語
  - GB2312: 中国語

## クラスタの追加
```bash
# クラスタの追加
$ clpcreate add cls <クラスタ名> <文字コード> <エンコード> <OS の種類>

# クラスタのパラメータの追加
$ clpcreate add clsparam <タグ名> <パラメータ>
```
- 文字コード: CLUSTERPRO の言語に応じて次の値を入力してください。
  - EUC-JP: 日本語
  - ASCII: 英語
  - GB2312: 中国語
- エンコード: WebUI で clp.conf を作成した場合に、WebUI が動くサーバの OS と CLUSTERPRO の言語により決定されるパラメータです。
  - OS が Windowsの場合: SJIS
  - OS が Linux かつ日本語の場合: EUC-JP
  - OS が Linux かつ英語の場合: ASCII
  - OS が Linux かつ中国語の場合: GB2312
- OS の種類: クラスタサーバの OS の種類を指定してください。
- タグ名、パラメータについては、[クラスタのパラメータ](#クラスタのパラメータ)を参照してください。
- サンプルスクリプトでは、以下のように設定しています。
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

## リソースの追加
```bash
# リソースの追加
$ clpcreate add rsc <リソースを追加するグループ名> <リソースのタイプ名> <リソース名>

# リソースのパラメータの追加
$ clpcreate add rscparam <リソースのタイプ名> <リソース名> <タグ名> <パラメータ>
```
- リソースを追加するグループ名: リソースを追加したいグループ名を指定してください。
- リソースのタイプ名: リソースのタイプ (e.g. fip, disk, exec) を指定してください。
- リソース名: リソース名を指定してください。
- タグ名、パラメータについては、[リソースのパラメータ](#リソースのパラメータ)を参照してください。
- サンプルスクリプトでは、以下のように設定しています。
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

## サーバの追加
```bash
$ clpcreate add srv <サーバ名> <プライオリティ>
``` 
- サーバ名: クラスタのサーバ名を指定してください (e.g. hostname コマンドの実行結果)。
- プライオリティ: マスタサーバが 0 になります。以降のサーバは 1 ずつインクリメントしてください。
  - 2 ノード構成 (server1, server2) で、server1 がマスタサーバの場合、server1 のプライオリティが 0、server2 のプライオリティが 1 となります。
- サンプルスクリプトでは、以下のように設定しています。
  ```perl
  # $server
  #  Top of the list is master node.
  my $server =
  [
      ['server1'],
      ['server2'],
      [] # 末尾に空の配列を挿入してください
  ];
   : 
  # add a server to a cluster
  for ($i = 0; $i < scalar(@$server); $i++)
  {
      next if (scalar(@{$server->[$i]}) == 0);
      $ret = `$clpcreate add srv $server->[$i][0] $i`;
  }
  ```

## IPアドレスの追加
```bash
$ clpcreate add ip <サーバ名> <デバイスID> <IPアドレス>
```
- サーバ名: クラスタのサーバ名を指定してください (e.g. hostname コマンドの実行結果)。
- デバイスID: 各サーバのIPアドレスのデバイスIDを指定してください。以降のデバイスIDは 1 ずつインクリメントしてください。
  - ユーザモードハートビート、カーネルモードハートビートは 0 から始まります。
    - 例: lanhb1のデバイスIDが0, lankhb1のデバイスIDが1, lanhb2のデバイスIDが2
  - BMCハートビートは、600 から始まります。
  - デバイスIDはサーバ毎で独立となっています。(デバイスID 0 が複数存在することになる。)
- IPアドレス: CLUSTERPROで用いるIPアドレスを指定してください。
- サンプルスクリプトでは、以下のように設定しています。
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
  
## カーネルモードハートビートの追加
```bash
$ clpcreate add khb <デバイスID> <プライオリティ>
```
- デバイスID: IPアドレスのデバイスIDを指定してください。
- プライオリティ: ハートビートのプライオリティは 0 から指定してください。
- サンプルスクリプトでは、以下のように設定しています。
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
  
## ユーザモードハートビートの追加
```bash
$ clpcreate add hb <デバイスID> <プライオリティ>
```
- デバイスID: IPアドレスのデバイスIDを指定してください。
- プライオリティ: ハートビートのプライオリティは 0 から指定してください。

## ディスクハートビートの追加
```bash
$ clpcreate add diskhb <デバイスID> <プライオリティ>
$ clpcreate add diskhbsrv <デバイスID> <ディスクデバイス>
```
- ディスクハートビートを追加するには、2つのコマンドを実行する必要があります。
  - clpcreate add diskhb
  - clpcreate add diskhbsrv
- デバイスID: ディスクハートビートのデバイスIDを 300 から指定してください。以降のデバイスIDは1ずつインクリメントしてください。
- プライオリティ: ハートビートのプライオリティは 0 から指定してください。
- ディスクデバイス: ディスクハートビートで用いるデバイスのパスを指定してください。
  - 現在は、両サーバのディスクデバイスのパスが同一であることを前提としています。
- サンプルスクリプトでは、以下のように設定しています。
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

## ネットワークパーティション解決リソースの追加
```bash
$ clpcreate add np ping <プライオリティ> <デバイスID> <グループID> <リストID> <IPアドレス>
$ clpcreate add npsrv ping <サーバ名> <デバイスID> <使用の有無>
```
- NP解決リソースを追加するには、2つのコマンドを実行する必要があります。
  - clpcreate add np
  - clpcreate add npsrv
- プライオリティ: NP解決リソースのプライオリティは 0 から指定してください。
- デバイスID: NP解決リソースのデバイスIDを 10200 から指定してください。以降のデバイスは 1 ずつインクリメントしてください。
- グループID: NP解決リソースのグループIDを 0 から指定してください。以降のグループIDは 1 ずつインクリメントしてください。
- リストID: IPアドレスのIDを 0 から指定してください。以降のIDは 1 ずつインクリメントしてください。
- IPアドレス: NP解決リソースで用いるIPアドレスを指定してください。
- サーバ名: サーバ名を指定してください。
- 使用の有無: 各サーバについて、使用する場合は 1 、使用しない場合は空文字 "" を指定してください。
- サンプルスクリプトでは、以下のように設定しています。
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
  
## BMCハートビートの追加
```bash
$ clpcreate add bmchb <デバイスID> <プライオリティ>
```
- デバイスID: IPアドレスのデバイスID 600 以降を指定してください。以降のデバイス ID は1ずつインクリメントしてください。
- プライオリティ: ハートビートのプライオリティは 0 から指定してください。

## グループの追加
```bash
# グループの追加
$ clpcreate add grp <グループのタイプ> <グループ名>

# グループのパラメータの追加
$ clpcreate add grpparam <グループのタイプ名> <グループ名> <タグ名> <パラメータ>
```
- グループのタイプ: 通常のグループには **failover** を指定してください。
- グループ名: グループ名を指定してください。
- タグ名、パラメータについては、[グループのパラメータ](#グループのパラメータ)を参照してください。
- サンプルスクリプトでは、以下のように設定しています。
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

## リソースの追加
```bash
# リソースの追加
$ clpcreate add rsc <リソースを追加するグループ名> <リソースのタイプ名> <リソース名>

# リソースのパラメータの追加
$ clpcreate add rscparam <リソースのタイプ名> <リソース名> <タグ名> <パラメータ>
```
- リソースを追加するグループ名: リソースを追加したいグループ名を指定してください。
- リソースのタイプ名: リソースのタイプ (e.g. fip, disk, exec) を指定してください。
- リソース名: リソース名を指定してください。
- タグ名、パラメータについては、[リソースのパラメータ](#リソースのパラメータ)を参照してください。
- サンプルスクリプトでは、以下のように設定しています。
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

## リソースの依存関係の追加
```bash
$ clpcreate add rscdep <依存されるリソース名> <依存するリソース名>
```
- グループの起動時、依存されるリソースの起動後、依存するリソースを起動します。
- グループの停止時、依存するリソースの起動後、依存されるリソースを停止します。
- サンプルスクリプトでは、以下のように設定しています。
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

## モニタリソースの追加
```bash
# モニタリソースを追加する
$ clpcreate add mon <モニタリソースのタイプ名> <モニタリソース名>

# モニタリソースのパラメータを追加する
$ clpcreate add monparam <モニタリソースのタイプ名> <モニタリソース名> <タグ名> <パラメータ>
```
- モニタリソースのタイプ名: モニタリソースのタイプ名 (e.g. diskw, fipw, pidw) を指定してください。
- タグ名、パラメータについては、[モニタリソースのパラメータ](#モニタリソースのパラメータ)を参照してください。
- サンプルスクリプトでは、以下のように設定しています。
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

## クラスタのパラメータ
#### リカバリ
- pm/exec0/recover: クラスタサービスのプロセス異常時動作
- pm/exec1/recover
- pm/exec2/recover
  - 上記 3 つのパス全てに同じ値を設定してください。
    ```bash
    $ clpcreate add clsparam pm/exec0/recover 7
    $ clpcreate add clsparam pm/exec1/recover 7
    $ clpcreate add clsparam pm/exec2/recover 7
    ```
  |数値|最終動作|備考|
  |----|---------|--|
  |   2|OSシャットダウン||
  |   3|OS再起動||
  |   5|sysrqパニック||
  |   6|keepaliveリセット||
  |   7|keepaliveパニック||
  |   8|BMCリセット||
  |   9|BMCパワーオフ||
  |  10|BMCパワーサイクル||
  |  11|BMC NMI||
  |  12|I/O-Fencing(High-End Server Option)||

- cluster/rsctimeout/rsctoaction: グループリソースの活性/非活性ストール発生時動作
  ```bash
  $ clpcreate add clsparam cluster/rsctimeout/rsctoaction 0
  ```
  |数値|最終動作|備考|
  |----|---------|--|
  |既定値|クラスタサービス停止とOSシャットダウン||
  |   0|何もしない(活性/非活性異常として扱う)||
  |   3|クラスタサービス停止とOS再起動||
  |   8|sysrqパニック||
  |   9|keepaliveリセット||
  |  10|keepaliveパニック||
  |  11|BMCリセット||
  |  12|BMCパワーオフ||
  |  13|BMCパワーサイクル||
  |  14|BMC NMI||
  |  15|I/O-Fencing(High-End Server Option)||

- cluster/networkpartition/npaction: NP発生時動作
  ```bash
  $ clpcreate add clsparam cluster/networkpartiton/npaction 6
  ```
  |数値|最終動作|備考|
  |----|---------|--|
  |既定値|クラスタサービス停止とOSシャットダウン||
  |   0|何もしない(活性/非活性異常として扱う)||
  |   1|クラスタサービス停止||
  |   2|クラスタサービス停止とOSシャットダウン||
  |   3|クラスタサービス停止とOS再起動||
  |   4|sysrqパニック||
  |   5|keepaliveリセット||
  |   6|keepaliveパニック||
  |   7|BMCリセット||
  |   8|BMCパワーオフ||
  |   9|BMCパワーサイクル||
  |  10|BMC NMI||
  |  11|I/O-Fencing(High-End Server Option)||

#### 監視
- haltp/method: 監視方法
  - softdog
  - keepalive
    ```bash
    $ clpcreate add clsparam haltp/method keepalive
    ```
    
- haltp/action: タイムアウト発生時動作
  - RESET
  - PANIC (softdog の場合は既定値かつ固定値のためコマンドでは指定不要)
    ```bash
    $ clpcreate add clsparam haltp/action PANIC
    ```

#### WebManager
- webmgr/security/clientlist/iprest: クライアントのIPアドレスによって接続を制御する
  - 0: 制御しない
  - 1: 制御する
  ```
  $ clpcreate add clsparam webmgr/security/clientlist/iprest 1
  ```
  - 制御対象のIPアドレスは、以下で設定してください。 
    ```
    $ clpcreate add webmgr clientlist 192.168.100.1
    $ clpcreate add webmgr clientlist 192.168.100.0/24
    ```

## グループのパラメータ
#### 起動サーバ
- 起動可能なサーバを個別に設定する場合は、以下のパラメータを設定してください。(デフォルトは全てのサーバでフェイルオーバ可能)
- policy@\<サーバ名\>/order: 起動可能なサーバと優先順位
- 優先順位は0が一番高い
  ```bash
  $ clpcreate add grpparam failover failover1 policy@server1/order 0
  $ clpcreate add grpparam failover failover1 policy@server2/order 1
  ```

## リソースのパラメータ
### 共通パラメータ
#### 復旧動作
- act/retry: 活性リトライしきい値
- act/fo: フェイルオーバしきい値
- act/action: (活性異常検出時の) 最終動作
- deact/retry: 非活性リトライしきい値
- deact/action: (非活性異常検出時の) 最終動作
  ```bash
  $ clpcreate add rscparam fip fip1 act/retry 1
  $ clpcreate add rscparam fip fip1 act/fo 1
  $ clpcreate add rscparam fip fip1 act/action 2
  $ clpcreate add rscparam fip fip1 deact/retry 1
  $ clpcreate add rscparam fip fip1 deact/action 5
  ```
  |数値|最終動作|備考|
  |----|---------|--|
  |   0|何もしない (次のリソースを活性/非活性する)||
  |   1|何もしない (次のリソースを活性/非活性しない)||
  |   2|グループ停止|活性異常検出時のみ指定可能|
  |   3|クラスタサービス停止||
  |   4|クラスタサービス停止とOSシャットダウン||
  |   5|クラスタサービス停止とOS再起動||
  |   8|sysrqパニック||
  |   9|keepaliveリセット||
  |  10|keepaliveパニック||
  |  11|BMCリセット||
  |  12|BMCパワーオフ||
  |  13|BMCパワーサイクル||
  |  14|BMC NMI||

### ディスクリソース (タイプ名: disk)
- parameters/disktype: ディスクのタイプ
  - 通常のデバイスをマウントする場合 **disk** を指定してください。
  - ボリュームグループ (VG) 上の論理ボリューム (LV) をマウントする場合、**lvm** を指定してください。
- parameters/device: デバイス名
- parameters/mount/point: マウントポイント
- parameters/fs: ファイルシステム
  - ext3, ext4, xfs を指定してください
- parameters/fsck/timing: Mount実行前のfsckアクション
  - 0: 実行しない
  - 1: 必ず実行する
  - 2: 指定回数に達したら実行する
- parameters/fsck/interval: Mount実行前のfsckアクションを実行するまでのMount回数
  - parameters/fsck/timing を 2 に設定した場合にのみ設定してください。

### exec リソース (タイプ名: exec)
- parameters/act/path: Start script のパス
  - **この製品で作成したスクリプト (既定値)** の場合、**start.sh** を指定してください。
  - **ユーザアプリケーション** の場合、任意のパス (e.g. /opt/test/start.sh) を指定してください。
- parameters/deact/path: Stop script のパス
  - **この製品で作成したスクリプト (既定値)** の場合、**stop.sh** を指定してください。
  - **ユーザアプリケーション** の場合、任意のパス (e.g. /opt/test/stop.sh) を指定してください。
- parameters/timeout/start: 開始スクリプトのタイムアウト
- parameters/timeout/stop: 終了スクリプトのタイムアウト
- parameters/act/sync: 開始スクリプトの同期/非同期
  - 0: 非同期
  - 1: 同期
- parameters/deact/sync: 終了スクリプトの同期/非同期
  - 0: 非同期
  - 1: 同期

### フローティング IP リソース (タイプ名: fip)
- parameters/ip: フローティング IP アドレス
  - インタコネクトと同じサブネットの IP アドレスを指定してください。

### ボリュームマネージャリソース (タイプ名: volmgr)
- parameters/type
  - LVM を使う場合 **lvm** を指定してください。
- parameters/devname
  - LVM を使う場合、VG 名を指定してください。

## モニタリソースのパラメータ
### 共通パラメータ
#### 監視 (共通)
- polling/interval: 監視処理のインターバル
- polling/timeout: 監視処理のタイムアウト
- emergency/dumpcollect/use: タイムアウト発生時に監視プロセスのダンプを採取する
  - usew, genwでは設定できないことにご注意ください。
  - 0: 無効
  - 1: 有効
- emergency/timeout/notreconfirmation/use: タイムアウト発生時にリトライしない
  - 0: 無効
  - 1: 有効
- emergency/timeout/notrecovery/use: タイムアウト発生時に回復動作を実行しない
  - 0: 無効
  - 1: 有効
- polling/reconfirmation: リトライ回数
  ```bash
  $ clpcreate add monparam fipw fipw1 polling/interval 60
  $ clpcreate add monparam fipw fipw1 polling/timeout 120
  $ clpcreate add monparam fipw fipw1 polling/reconfirmation 1
  ```
- polling/timing: 監視タイミング
  - 0: 常時
  - 1: 活性時
    - 既定値が常時のモニタリソースにおいて、活性時に変更する場合、target も併せて指定してください。
- target: 監視対象のリソース
　- polling/timing が 1 の場合 (活性時監視) 、target を設定する必要があります。
  - polling/timing が 0 の場合 (常時監視) 、設定は不要です。
    ```bash
    $ clpcreate add monparam fipw fipw1 target fip1
    ```
- firstmonwait: 監視開始待ち時間
- polling/servers@\<id\>/name: 監視を行うサーバを選択する
  - 監視を行うサーバが1つの場合、以下のように実行してください。
    ```bash
    $ clpcreate add monparam fipw fipw1 polling/servers@0/name <サーバ名>
    ```
  - 監視を行うサーバが2つ以上の場合、以下のように実行してください。
    ```bash
    $ clpcreate add monparam fipw fipw1 polling/servers@0/name <サーバ名>
    $ clpcreate add monparam fipw fipw1 polling/servers@1/name <サーバ名>
    ```

#### 回復動作
- カスタム設定のみ対応しています。
- 適切なパラメータを設定することで、テンプレート(「回復対象を再起動」等)の動作と同様の回復動作を設定することができます。
  - 回復対象に対してフェイルオーバ実行
    ```bash
    $ clpcreate add monparam genw genw1 emergency/threshold/restart 0
    $ clpcreate add monparam genw genw1 emergency/threshold/fo 1
    ```
  - 回復対象を再起動、効果がなければフェイルオーバ実行
    ```bash
    $ clpcreate add monparam genw genw1 emergency/threshold/restart 1
    $ clpcreate add monparam genw genw1 emergency/threshold/fo 1
    ```
  - 回復対象を再起動
    ```bash
    $ clpcreate add monparam genw genw1 emergency/threshold/restart 1
    $ clpcreate add monparam genw genw1 emergency/threshold/fo 0
    ```
  - 最終動作のみ実行
    ```bash
    $ clpcreate add monparam genw genw1 emergency/threshold/restart 0
    $ clpcreate add monparam genw genw1 emergency/threshold/fo 0
    ```
  - カスタム設定
    - 以降に記載するパラメータから必要なものを設定してください。
- パラメータの詳細
- 回復対象
  - リソースの場合
    ```bash
    $ clpcreate add monparam <モニタリソースのタイプ名> <モニタリソース名> relation/type rsc
    $ clpcreate add monparam <モニタリソースのタイプ名> <モニタリソース名> relation/name <リソース名>
    ```
  - グループの場合
    ```bash
    $ clpcreate add monparam <モニタリソースのタイプ名> <モニタリソース名> relation/type grp
    $ clpcreate add monparam <モニタリソースのタイプ名> <モニタリソース名> relation/name <フェイルオーバグループ名>
    ```
  - LocalServer の場合
    ```bash
    add monparam <モニタリソースのタイプ名> <モニタリソース名> relation/type cls
    $ clpcreate add monparam <モニタリソースのタイプ名> <モニタリソース名> relation/name LocalServer
    ```
- emergency/threshold/script: 回復スクリプト実行回数
- emergency/preaction/userestart: 再活性前にスクリプトを実行する
  - 0: 実行しない
  - 1: 実行する
- emergency/threshold/restart: 最大再活性回数
  ```bash
  $ clpcreate add monparam genw genw1 emergency/threshold/restart 1
  ```
- emergency/preaction/usefailover: フェイルオーバ実行前にスクリプトを実行する
  - 0: 実行しない
  - 1: 実行する
- emergency/threshold/fo: 最大フェイルオーバ回数
  ```bash
  $ clpcreate add monparam genw genw1 emergency/threshold/fo 1
  ```
- emergency/preaction/usefailover: 最終動作前にスクリプトを実行する
  - 0: 実行しない
  - 1: 実行する
- emergency/action: 最終動作
  ```bash
  $ clpcreate add monparam genw genw1 emergency/action 3
  ```
  |数値|最終動作|
  |----|---------|
  |   1|何もしない|
  |   2|グループ停止|
  |   3|クラスタサービス停止|
  |   4|クラスタサービス停止とOSシャットダウン|
  |   5|クラスタサービス停止とOS再起動|
  |   8|sysrqパニック|
  |   9|keepaliveリセット|
  |  10|keepaliveパニック|
  |  11|BMCリセット|
  |  12|BMCパワーオフ|
  |  13|BMCパワーサイクル|
  |  14|BMC NMI|

- スクリプト設定
  - emergency/preaction/default: スクリプトのタイプ
    - 0: ユーザアプリケーション
    - 1: この製品で作成したスクリプト
  - emergency/preaction/path: ファイル
    - ユーザアプリケーションの場合: スクリプトの絶対パス
    - この製品で作成したスクリプトの場合: preaction.sh
  - emergency/preaction/timeout: タイムアウト


### フローティング IP モニタリソース (タイプ名: fipw)
- parameters/monmii: NIC Link Up/Downを監視する
  - 0: 監視しない (既定値)
  - 1: 監視する
    ```bash
    $ clpcreate add monparam genw genw1 parameters/monmii 1
    ```

### カスタムモニタリソース (タイプ名: genw)
- parameters/path
  - **この製品で作成したスクリプト** の場合、genw.sh を指定してください。
  - **ユーザアプリケーション** の場合、任意のパス (e.g. /opt/test/genw.sh) を指定してください。
    ```bash
    $ clpcreate add monparam genw genw1 parameters/path genw.sh
    ```

### ディスクモニタリソース (タイプ名: diskw)
- parameters/object
  - 監視先のディスクのパスを指定してください。
    ```bash
    $ clpcreate add monparam diskw diskw1 parameters/object /dev/sdc2
    ```

### IP モニタリソース (タイプ名: ipw)
- parameters/list@\<id\>/ip
  - 監視対象の IPアドレスが1つの場合、以下のように実行してください。
    ```bash
    $ clpcreate add monparam ipw ipw1 parameters/list@0/ip <ゲートウェイ の IP アドレス>
    ```
  - 複数の IP アドレスを監視対象とする場合、以下のように実行してください。
    ```bash
    $ clpcreate add monparam ipw ipw1 parameters/list@0/ip <ゲートウェイ#1 の IP アドレス>
    $ clpcreate add monparam ipw ipw1 parameters/list@1/ip <ゲートウェイ#2 の IP アドレス>
    ```

### ユーザ空間モニタリソース (タイプ名: userw)
- parameters/method: 監視方法
  - keepalive: X 4.x 以降の既定値
  - softdog: X 3.x までの既定値
    ```bash
    $ clpcreate add monparam userw userw1 parameters/method keepalive
    ```
- parameters/action: タイムアウト発生時動作
  - RESET
  - PANIC (softdog の場合は既定値かつ固定値のためコマンドでは指定不要)
    ```bash
    $ clpcreate add monparam userw userw1 parameters/action PANIC
    ```
### ボリュームマネージャモニタリソース (タイプ名: volmgrw)
- parameters/devname: VG 名を指定してください。
  ```bash
  $ clpcreate add monparam volmgrw volmgrw parameters/devname <VG 名>
  ```

### PIDモニタリソース (タイプ名: pidw)
- 固有のパラメータはありません。
