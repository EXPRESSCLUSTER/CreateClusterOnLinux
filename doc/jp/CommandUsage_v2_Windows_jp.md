# Command Usage for Windows cluster
- clpcfset コマンド **(ver2)** の使用法について説明しています。
- 各パラメータに入力可能な文字列や禁則文字列については、CLUSTERPRO のリファレンスガイドを参照してください。

## 注意
- SANブート構成にはまだ対応しておりません。

## 目次
- [クラスタの作成](#クラスタの作成)
- [サーバの追加](#サーバの追加)
- [HBAの追加](#HBAの追加)
- [デバイスの追加](#デバイスの追加)
- [カーネルモードハートビートの追加](#カーネルモードハートビートの追加)
- [BMCハートビートの追加](#BMCハートビートの追加)
- [Witnessハートビートの追加](#Witnessハートビートの追加)
- [ディスク方式ネットワークパーティション解決リソースの追加](#ディスク方式ネットワークパーティション解決リソースの追加)
- [PING方式ネットワークパーティション解決リソースの追加](#PING方式ネットワークパーティション解決リソースの追加)
- [HTTP方式ネットワークパーティション解決リソースの追加](#PING方式ネットワークパーティション解決リソースの追加)
- [グループの追加](#グループの追加)
- [リソースの追加](#リソースの追加)
- [リソースの依存関係の追加](#リソースの依存関係の追加)
- [モニタリソースの追加](#モニタリソースの追加)

## クラスタの作成
```bash
# クラスタの作成
$ clpcfset create <クラスタ名> <エンコード> <WebUIのエンコード> <OS の種類>
```
- カレントディレクトリにclp.confが生成されます。
- エンコード: CLUSTERPRO の言語に応じて次の値を入力してください。
  - SJIS: 日本語
  - ASCII: 英語
  - GB2312: 中国語
- WebUIのエンコード: WebUI で clp.conf を作成した場合に、WebUI が動くサーバの OS と CLUSTERPRO の言語により決定されるパラメータです。
  - OS が Windowsの場合: SJIS
  - OS が Linux かつ日本語の場合: EUC-JP
  - OS が Linux かつ英語の場合: ASCII
  - OS が Linux かつ中国語の場合: GB2312
- OS の種類: クラスタサーバの OS の種類を指定してください。

```bash
# クラスタのパラメータの追加
$ clpcfset add clsparam <タグ名> <パラメータ>
```
- タグ名、パラメータについては、[クラスタのパラメータ](https://github.com/EXPRESSCLUSTER/CreateClusterOnLinux/blob/master/doc/jp/ClusterParameters_Linux_jp.md)を参照してください。

## サーバの追加
```bash
$ clpcfset add srv <サーバ名> <プライオリティ>
``` 
- サーバ名: クラスタのサーバ名を指定してください (e.g. hostname コマンドの実行結果)。アルファベットは小文字である必要があります。
- プライオリティ: マスタサーバが 0 になります。以降のサーバは 1 ずつインクリメントしてください。
  - 2 ノード構成 (server1, server2) で、server1 がマスタサーバの場合、server1 のプライオリティが 0、server2 のプライオリティが 1 となります。

## HBAの追加
```bash
$ clpcfset add hba <サーバ名> <HBA ID> <portnumber> <deviceid> <instanceid>
```
- サーバ名: クラスタのサーバ名を指定してください。
- HBA ID: 初期値は0です。以降は1ずつ増えていきます。
- portnumber, deviceid, instanceid
  - HBA の情報を指定してください。
    - バックスラッシュを含む際はエスケープにご注意ください。
  - 以下のレポジトリで提供されている clpdiskctrl.exe を用いて HBA の情報を取得することができます。
    - https://github.com/EXPRESSCLUSTER/ControlDisk

## デバイスの追加
- ハートビートやネットワークパーティション解決リソースを追加する前に、対応するデバイスを追加する必要があります。
```bash
$ clpcfset add device <サーバ名> <デバイスのタイプ> <デバイスID> <info> [<extend>]
```
- サーバ名: クラスタのサーバ名を指定してください。
- デバイスのタイプ: 以下のIDを指定してください。
  - カーネルモードハートビート: lan
  - BMCハートビート: bmc
  - Witnessハートビート: witness
  - ディスク方式NP解決リソース: disknp
  - PING方式NP解決リソース: ping
  - HTTP方式NP解決リソース: http
- デバイスID: 以下の値から始まり、追加する毎に1ずつ増えていきます。各サーバに同じデバイスIDのデバイスを設定してください。
  - カーネルモードハートビート: 0
  - ディスクハートビート: 300
  - BMCハートビート: 600
  - Witnessハートビート: 700
  - ディスク方式NP解決リソース: 10100
  - PING方式NP解決リソース: 10200
  - HTTP方式NP解決リソース: 10700
- info: 以下の値を入力してください。
  - カーネルモードハートビート: IPアドレス
  - BMCハートビート: IPアドレス
  - Witnessハートビート: 0 (使用しない) / 1 (使用する)
  - ディスク方式NP解決リソース: GUID
      - 以下のレポジトリで提供されている clpdiskctrl.exe を用いて GUID を取得することができます。
        - https://github.com/EXPRESSCLUSTER/ControlDisk
  - PING方式NP解決リソース: 0 (使用しない) / 1 (使用する)
  - HTTP方式NP解決リソース: 0 (使用しない) / 1 (使用する)
- extend:
  - Witnessハートビート: Witnessサーバの ホストアドレス:ポート番号
  - ディスク方式NP解決リソース: ドライブレター (例: F:\\)
    - バックスラッシュのエスケープにご注意ください。
  - HTTP方式NP解決リソース: 接続先の ホストアドレス:ポート番号

## カーネルモードハートビートの追加
```bash
$ clpcfset add hb lankhb <デバイスID> <プライオリティ>
```
- プライオリティ: ハートビートのプライオリティは 0 から指定してください。

## BMCハートビートの追加
```bash
$ clpcfset add bmchb <デバイスID> <プライオリティ>
```
- プライオリティ: ハートビートのプライオリティは 0 から指定してください。
- IPアドレス: BMCハートビートで用いるIPアドレスを指定してください。

## Witnessハートビートの追加
```bash
$ clpcfset add hb witnesshb <デバイスID> <プライオリティ> <ホスト>
```
- プライオリティ: ハートビートのプライオリティは 0 から指定してください。
- ホスト: Witnessサーバの ホストアドレス:ポート番号

## ディスク方式ネットワークパーティション解決リソースの追加
```bash
$ clpcfset add np disknp <デバイスID> <プライオリティ>
```
- プライオリティ: NP解決リソースのプライオリティは 0 から指定してください。

## PING方式ネットワークパーティション解決リソースの追加
```bash
$ clpcfset add np pingnp <デバイスID> <プライオリティ> <グループID> <リストID> <IPアドレス>
```
- プライオリティ: NP解決リソースのプライオリティは 0 から指定してください。
- グループID: NP解決リソースのグループIDを 0 から指定してください。以降のグループIDは 1 ずつインクリメントしてください。
- リストID: IPアドレスのIDを 0 から指定してください。以降のIDは 1 ずつインクリメントしてください。
- IPアドレス: NP解決リソースで用いるIPアドレスを指定してください。

## HTTP方式ネットワークパーティション解決リソースの追加
```bash
$ clpcfset add np httpnp <デバイスID> <プライオリティ> <ホスト>
```
- プライオリティ: NP解決リソースのプライオリティは 0 から指定してください。
- ホスト: Witnessサーバの ホストアドレス:ポート番号

## グループの追加
```bash
# グループの追加
$ clpcfset add grp <グループのタイプ> <グループ名>

# グループのパラメータの追加
$ clpcfset add grpparam <グループのタイプ名> <グループ名> <タグ名> <パラメータ>
```
- グループのタイプ: 通常のグループには **failover** を指定してください。
- グループ名: グループ名を指定してください。
- タグ名、パラメータについては、[グループのパラメータ](https://github.com/EXPRESSCLUSTER/CreateClusterOnLinux/blob/master/doc/jp/GroupParameters_Windows_jp.md)を参照してください。

## リソースの追加
```bash
# リソースの追加
$ clpcfset add rsc <リソースを追加するグループ名> <リソースのタイプ名> <リソース名>

# リソースのパラメータの追加
$ clpcfset add rscparam <リソースのタイプ名> <リソース名> <タグ名> <パラメータ>
```
- リソースを追加するグループ名: リソースを追加したいグループ名を指定してください。
- リソースのタイプ名: リソースのタイプ (e.g. fip, disk, exec) を指定してください。
- リソース名: リソース名を指定してください。
- タグ名、パラメータについては、[リソースのパラメータ](https://github.com/EXPRESSCLUSTER/CreateClusterOnLinux/blob/master/doc/jp/ResourceParameters_Windows_jp.md)を参照してください。

## リソースの依存関係の追加
```bash
$ clpcfset add rscdep <依存されるリソースのタイプ> <依存されるリソース名> <依存するリソース名>
```
- グループの起動時、依存されるリソースの起動後、依存するリソースを起動します。
- グループの停止時、依存するリソースの停止後、依存されるリソースを停止します。

## モニタリソースの追加
```bash
# モニタリソースを追加する
$ clpcfset add mon <モニタリソースのタイプ名> <モニタリソース名>

# モニタリソースのパラメータを追加する
$ clpcfset add monparam <モニタリソースのタイプ名> <モニタリソース名> <タグ名> <パラメータ>
```
- モニタリソースのタイプ名: モニタリソースのタイプ名 (e.g. diskw, fipw, pidw) を指定してください。
- タグ名、パラメータについては、[モニタリソースのパラメータ](https://github.com/EXPRESSCLUSTER/CreateClusterOnLinux/blob/master/doc/jp/MonitorParameters_Windows_jp.md)を参照してください。