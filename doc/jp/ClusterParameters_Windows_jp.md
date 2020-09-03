# Cluster Parameters for Windows cluster
- クラスタパラメータの詳細について説明しています。
- **(default)** と記載された設定値を利用する場合は、コマンドでパラメータを設定する必要はありません。
- **ver1**を使用する場合は、コマンド例の**clpcfset**を**clpcreate**に置き換えてください。
- 各パラメータに入力可能な文字列や禁則文字列については、CLUSTERPRO のリファレンスガイドを参照してください。

## 目次
- [NP解決](#NP解決)
- [監視](#監視)
- [リカバリ](#リカバリ)
- [アラートサービス](#アラートサービス)
- [WebManager](#WebManager)
- [アラートログ](#アラートログ)
- [アカウント](#アカウント)


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

## アラートサービス
- cluster/messages/use: アラート通報設定を有効にする
  - 0: チェックしない **(default)**
  - 1: チェックする
- アラート送信先の変更
  - メッセージの追加
    ```
    $ clpcfset add messages <モジュールタイプ> <イベントID> <Alert Logs> <Alert Extension> <Mail Report> <SNMP Trap> <Event Log(DisableOnly)>

    e.g.
    $ clpcfset add messages apisv 123 1 1 1 0 0
    ```
    - カテゴリはモジュールタイプをもとに自動で判断されます。
    - 送信先は 0: チェックしない、1: チェックする。
  - メッセージにコマンドを追加
    ```
    $ clpcfset add messages cmdline <モジュールタイプ> <イベントID> <コマンド>
    ```
### メール通報
- cluster/mail/address: メールアドレス
- cluster/mail/subject: 件名
- メール送信方法はSMTPのみのため、指定する必要はありません。
- SMTP設定
  - cluster/mail/smtp/charset: メール送信文書の文字コード
    - Shift_JIS
    - ISO-2022-jp
    - ISO-8859-1
  - cluster/mail/smtp/timeout: 通信応答待ち時間 **(default 30)**
  - cluster/mail/smtp/subencode: 件名のエンコードをする
    - 0: チェックしない **(default)**
    - 1: チェックする
  - SMTPサーバの追加
    ```
    $ clpcfset add smtpsrv <プライオリティ> <SMTPサーバ> <SMTPポート番号> <差出人メールアドレス> <SMTP認証を有効にする> <認証方式> <ユーザ名> <パスワード>
    ```
    - プライオリティは0から始まり、追加するたびに1ずつ増えていきます。
    - SMTP認証を有効にする 0: チェックしない、1: チェックする
    - 認証方式 CRAM-MD5, LOGIN, PLAIN
    - 現在clpcfsetはパスワードの暗号化に対応していません。WebUIまたはWebManagerで一時的に設定ファイルを作成し、clp.confの中身に記載されている暗号化されたパスワードを確認してください。
### SNMPトラップ
- 送信先設定
  - 送信先の追加
    ```
    $ clpcfset add snmpsrv <送信先サーバ> <SNMPポート番号> <SNMPバージョン> <SNMPコミュニティ名>
    ```
    - SNMPバージョン v1, v2c
- cluster/cialert/use: 筐体IDランプ連携を使用する
  - 0: チェックしない **(default)**
  - 1: チェックする
- cluster/cialert/repeat: 繰り返し実行
  - 0: 繰り返し実行しない
  - 1: 繰り返し実行する **(default)**
- cluster/cialert/execcmd_interval: インターバル **(default 120)**
- ネットワーク警告灯を使用する

  使用するには以下のコマンドを実行してください。
  ```
  $ clpcfset add alertservice
  ```

## WebManager
- webmgr/security/clientlist/iprest: クライアントのIPアドレスによって接続を制御する
  - 0: チェックしない **(default)**
  - 1: チェックする
    ```
    $ clpcfset add clsparam webmgr/security/clientlist/iprest 1
    ```
  - 接続を許可するIPアドレス(操作権あり)は、以下で設定してください。 
    ```
    $ clpcfset add webmgr clientlist 192.168.100.1
    $ clpcfset add webmgr clientlist 192.168.200.0/24
    ```
  - 接続を許可するIPアドレス(操作権なし)は、以下で設定してください。 
    ```
    $ clpcfset add webmgr clientlistro 192.168.100.1
    $ clpcfset add webmgr clientlistro 192.168.200.0/24
    ```

## アラートログ
- webalert/use: アラートサービスを有効にする
  - 0: チェックしない
  - 1: チェックする **(default)**
- webalert/main/alertlog/maxrecordcount: 保存最大アラートレコード数 **(default 10000)**
- 方法は unicast のみのため指定する必要はありません。
- webalert/daemon/timeout: 通信タイムアウト **(default 30)**

## アカウント
- cluster/account/list@<ユーザID>/username: ユーザ名
  - ユーザIDは0から始まります。その後、追加するたびに1ずつ増えていきます。
- cluster/account/list@<ユーザID>/password: パスワード
  - 現在clpcfsetはパスワードの暗号化に対応していません。WebUIまたはWebManagerで一時的に設定ファイルを作成し、clp.confの中身に記載されている暗号化されたパスワードを確認してください。