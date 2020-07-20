# Cluster Parameters for Linux cluster
- クラスタパラメータの詳細について説明しています。
- **(default)** と記載された設定値を利用する場合は、コマンドでパラメータを設定する必要はありません。
- 各パラメータに入力可能な文字列や禁則文字列については、CLUSTERPRO のリファレンスガイドを参照してください。

## 目次
- [リカバリ](#リカバリ)
- [NP解決](#NP解決)
- [監視](#監視)
- [WebManager](#WebManager)

## リカバリ
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
  |   2|OSシャットダウン|**(default)**|
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
  |   0|何もしない(活性/非活性異常として扱う)||
  |   2|クラスタサービス停止とOSシャットダウン|**(default)**|
  |   3|クラスタサービス停止とOS再起動||
  |   8|sysrqパニック||
  |   9|keepaliveリセット||
  |  10|keepaliveパニック||
  |  11|BMCリセット||
  |  12|BMCパワーオフ||
  |  13|BMCパワーサイクル||
  |  14|BMC NMI||
  |  15|I/O-Fencing(High-End Server Option)||

## NP解決
- cluster/networkpartition/npaction: NP発生時動作
  ```bash
  $ clpcreate add clsparam cluster/networkpartiton/npaction 6
  ```
  |数値|最終動作|備考|
  |----|---------|--|
  |既定値|クラスタサービス停止とOSシャットダウン||
  |   0|何もしない(活性/非活性異常として扱う)||
  |   1|クラスタサービス停止||
  |   2|クラスタサービス停止とOSシャットダウン|**(default)**|
  |   3|クラスタサービス停止とOS再起動||
  |   4|sysrqパニック||
  |   5|keepaliveリセット||
  |   6|keepaliveパニック||
  |   7|BMCリセット||
  |   8|BMCパワーオフ||
  |   9|BMCパワーサイクル||
  |  10|BMC NMI||
  |  11|I/O-Fencing(High-End Server Option)||

## 監視
- haltp/method: 監視方法
  - softdog
  - ipmi
  - ipmi(High-End Server Option)
  - keepalive **(default)**
    ```bash
    $ clpcreate add clsparam haltp/method keepalive
    ```
    
- haltp/action: タイムアウト発生時動作
  - RESET **(default)**
  - PANIC (softdog の場合は既定値かつ固定値のためコマンドでは指定不要)
    ```bash
    $ clpcreate add clsparam haltp/action PANIC
    ```

- cluster/sysinfo/collect: システムリソース情報を収集する
  - 0: チェックしない **(default)**
  - 1: チェックする

## WebManager
- webmgr/security/clientlist/iprest: クライアントのIPアドレスによって接続を制御する
  - 0: チェックしない **(default)**
  - 1: チェックする
    ```
    $ clpcreate add clsparam webmgr/security/clientlist/iprest 1
    ```
  - 制御対象のIPアドレスは、以下で設定してください。 
    ```
    $ clpcreate add webmgr clientlist 192.168.100.1
    $ clpcreate add webmgr clientlist 192.168.200.0/24
    ```