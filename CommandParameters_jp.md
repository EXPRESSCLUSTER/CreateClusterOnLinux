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
- グループのタイプ: 
- グループ名

## リソースの追加
```bash

```

## 
