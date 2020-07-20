# Cluster Parameters for Windows cluster
- クラスタパラメータの詳細について説明しています。
- **(default)** と記載された設定値を利用する場合は、コマンドでパラメータを設定する必要はありません。
- **ver1**を使用する場合は、コマンド例の**clpcfset**を**clpcreate**に置き換えてください。
- 各パラメータに入力可能な文字列や禁則文字列については、CLUSTERPRO のリファレンスガイドを参照してください。

## 目次
- [リカバリ](#リカバリ)
- [NP解決](#NP解決)
- [監視](#監視)
- [WebManager](#WebManager)

## リカバリ
- pm/exec0/recover: クラスタサービスのプロセス異常時動作
- pm/exec1/recover
  - 上記 2 つのパス全てに同じ値を設定してください。
    ```bash
    $ clpcfset add clsparam pm/exec0/recover 6
    $ clpcfset add clsparam pm/exec1/recover 6
    ```
  |数値|最終動作|備考|
  |----|---------|--|
  |   5|緊急シャットダウン|**(default)**|
  |   6|意図的なストップエラーの発生||
  |   7|HWリセット||

- cluster/rsctimeout/rsctoaction: グループリソースの活性/非活性ストール発生時動作
  ```bash
  $ clpcfset add clsparam cluster/rsctimeout/rsctoaction 0
  ```
  |数値|最終動作|備考|
  |----|---------|--|
  |既定値|クラスタサービス停止とOSシャットダウン||
  |   0|何もしない(活性/非活性異常として扱う)||
  |   5|緊急シャットダウン|**(default)**|
  |   6|意図的なストップエラーの発生||

## NP解決
- cluster/networkpartition/npaction: NP発生時動作
  ```bash
  $ clpcfset add clsparam cluster/networkpartiton/npaction 6
  ```
  |数値|最終動作|備考|
  |----|---------|--|
  |   1|クラスタサービス停止||
  |   2|緊急シャットダウン|**(default)**|
  |   3|クラスタサービス停止とOSシャットダウン||
  |   4|クラスタサービス停止とOS再起動||
  |   5|意図的なストップエラーの発生||
  |   6|HWリセット||

## 監視
- haltp/method: 監視方法
  - softdog
  - keepalive
    ```bash
    $ clpcfset add clsparam haltp/method keepalive
    ```
    
- haltp/action: タイムアウト発生時動作
  - RESET
  - PANIC (softdog の場合は既定値かつ固定値のためコマンドでは指定不要)
    ```bash
    $ clpcfset add clsparam haltp/action PANIC
    ```

- cluster/sysinfo/collect: システムリソース情報を収集する
  - 0: チェックしない **(default)**
  - 1: チェックする

## WebManager
- webmgr/security/clientlist/iprest: クライアントのIPアドレスによって接続を制御する
  - 0: チェックしない **(default)**
  - 1: チェックする
    ```
    $ clpcfset add clsparam webmgr/security/clientlist/iprest 1
    ```
  - 制御対象のIPアドレスは、以下で設定してください。 
    ```
    $ clpcfset add webmgr clientlist 192.168.100.1
    $ clpcfset add webmgr clientlist 192.168.200.0/24
    ```