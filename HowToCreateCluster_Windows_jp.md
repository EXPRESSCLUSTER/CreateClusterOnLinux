# CLIでWindowsクラスタを作成する方法
CLIベースでWindowsクラスタを作成する方法について説明しています。

## 目次
- [CLUSTERPROのインストール](#CLUSTERPROのインストール)
- [ライセンスの登録](#ライセンスの登録)
- [HBAフィルタリングの設定](#HBAフィルタリングの設定)
- [サーバの再起動](#サーバの再起動)
- [クラスタ構成情報の作成](#クラスタ構成情報の作成)
- [クラスタ用スクリプトの配置](#クラスタ用スクリプトの配置)
- [クラスタ構成情報の配信](#クラスタ構成情報の配信)
- [CLUSTERPROサービスの再起動](#CLUSTERPROサービスの再起動)
- [クラスタの起動](#クラスタの起動)

----

## CLUSTERPROのインストール

両サーバにCLUSTERPROをインストールします。

```
# "<silent-install.batのパス>\silent-install.bat" -i <応答ファイルのパス>
```

## ライセンスの登録

clplcnscコマンドで両サーバにライセンスを登録します。

## HBAフィルタリングの設定

構築予定のクラスタにて、共有ディスクを利用される場合には、HBA に対してフィルタリングの設定を行う必要があります。

以下のレポジトリで提供されている clpdiskctrl.exe で HBA のフィルタリング設定を行うことができます。
https://github.com/EXPRESSCLUSTER/ControlDisk

## サーバの再起動

reboot等のOSコマンドで両サーバを再起動します。

## クラスタ構成情報の作成

コマンドを用いてクラスタ構成情報(clp.conf)を作成します。
- Windowsクラスタのパラメータ一覧
  - https://github.com/EXPRESSCLUSTER/CreateClusterOnLinux/blob/master/doc/jp/CommandUsage_v2_Windows_jp.md
- サンプルスクリプト
  - 共有ディスク (PING方式NP解決リソース)
    - https://github.com/EXPRESSCLUSTER/CreateClusterOnLinux/blob/master/template_scripts/clpcreate_v2/create_win_disk_pingnp.pl
  - 共有ディスク (Witnessハートビート & HTTP方式NP解決リソース)
    - https://github.com/EXPRESSCLUSTER/CreateClusterOnLinux/blob/master/template_scripts/clpcreate_v2/create_win_disk_witnesshb_httpnp.pl

## クラスタ用スクリプトの配置
構築予定のクラスタにて、スクリプトリソースまたはカスタムモニタリソースを利用される場合には、それらに含まれるスクリプトを以下のように配置してください。
```
clp.conf
scripts/
 |
 +-- フェイルオーバグループ名/ 
 |    |
 |    +-- スクリプトリソース名/ 
 |         |
 |         +-- start.bat
 |         |
 |         +-- stop.bat
 |
 +-- monitor.s/
      |
      +-- カスタムモニタリソース名/
           |
           +-- genw.bat

E.g.
clp.conf
scripts/
 |
 +-- failover/ 
 |    |
 |    +-- script/ 
 |         |
 |         +-- start.bat
 |         |
 |         +-- stop.bat
 |
 +-- monitor.s/
      |
      +-- genw/
           |
           +-- genw.bat
```

## クラスタ構成情報の配信

clpcfctrlコマンドでクラスタ構成情報を各サーバに配信します。

## CLUSTERPROサービスの再起動

配信したクラスタ構成情報を読み込ませるために、両サーバでCLUSTERPROの各種サービスを再起動します。

- CLUSTERPRO X 4.1以前の場合
  ```
  # clpcl -r --web --alert
  ```

- CLUSTERPRO X 4.2の場合
  ```
  # clpcl -r --web --alert --ib --api
  ```
  引き続きマスタノードではライセンスファイルを配信するために以下のコマンドを実行します。
  ```
  # clplcnsc --distribute
  ```

## クラスタの起動

clpclコマンドでクラスタを起動します。

```
clpcl -s -a
```
