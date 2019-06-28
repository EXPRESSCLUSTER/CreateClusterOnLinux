# Command Parameters
- clpcreate コマンドに指定可能なパラメータについて説明しています。
- 各パラメータに入力可能な文字列や禁則文字列については、CLUSTERPRO のリファレンスガイドを参照してください。

## クラスタ構成情報ファイルの初期化
```bash
$ clpcreate init <エンコードの種類>
```
- エンコードの種類は、クラスタのプロパティを参照してください。

## クラスタの追加
```bash
$ clpcreate add cls <クラスタ名> <エンコード> <OS の種類>
```
- エンコード: クラスタサーバの OS の言語を指定してください。日本語の場合は EUC-JP を指定してください。
- OS の種類: クラスタサーバの OS の種類を指定してください。
- サンプルスクリプトでは、以下のように設定しています。
  ```perl
  my %cluster = (name=>'cluster', encode=>'EUC-JP', os=>'linux');
   : 
  # add a cluster to initial configuration
  $ret = `$clpcreate add cls $cluster{'name'} $cluster{'encode'} $cluster{'os'}`;
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

## IP アドレスの追加


## グループの追加
```bash
$ clpcreate add <グループのタイプ> <グループ名>
```
- グループのタイプ: 通常のグループには **failover** を指定してください。
- グループ名: グループ名を指定してください。
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
$ clpcreate add rsc <リソースを追加するグループ名> <リソースタイプ> <リソース名>

# リソースのパラメータの追加
$ clpcreate add rscparam
```
- リソースを追加するグループ名: リソースを追加したいグループ名を指定してください。
- リソースタイプ: リソースのタイプ (e.g. fip, disk, exec) を指定してください。
- リソース名: リソース名を指定してください。
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

## リソースの依存関係

## リソースのパラメータ
### 共通パラメータ

### ディスクリソース (タイプ名: disk)
- parameters/disktype: ディスクのタイプ
  - disk: 通常のデバイスをマウントする場合
  - lvm: ボリュームグループ (VG) 上の論理ボリューム (LV) をマウントする場合 
- parameters/device: デバイス名
- parameters/mount/point: マウントポイント
- parameters/fs: ファイルシステム
  - ext3, ext4, xfs を指定してください

### exec リソース (タイプ名: exec)
- parameters/act/path: Start script のパス
  - この製品で作成したスクリプト (既定値) の場合、*start.sh* を指定してください。
　- ユーザアプリケーションの場合、任意のパス (e.g. /opt/test/start.sh) を指定してください。
- parameters/deact/path: Stop script のパス
  - この製品で作成したスクリプト (既定値) の場合、*stop.sh* を指定してください。
　- ユーザアプリケーションの場合、任意のパス (e.g. /opt/test/stop.sh) を指定してください。

### フローティング IP リソース (タイプ名: fip)
- ['ip', '192.168.137.70']],

### ボリュームマネージャリソース (タイプ名: volmgr)

## モニタリソースのパラメータ
