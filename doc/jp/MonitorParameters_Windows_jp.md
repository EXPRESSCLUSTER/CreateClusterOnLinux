# Resource Parameters for Windows cluster
- モニタリソースパラメータの詳細について説明しています。
- **(default)** と記載された設定値を利用する場合は、コマンドでパラメータを設定する必要はありません。
- **ver1**を使用する場合は、コマンド例の**clpcfset**を**clpcreate**に置き換えてください。
- 各パラメータに入力可能な文字列や禁則文字列については、CLUSTERPRO のリファレンスガイドを参照してください。

## 目次
- [共通パラメータ](#共通パラメータ)
- [フローティングIPモニタリソース](#フローティングIPモニタリソース)
- [カスタムモニタリソース](#カスタムモニタリソース)
- [ディスクTURモニタリソース](#ディスクモニタリソース)
- [ディスクRWモニタリソース](#ディスクRWモニタリソース)
- [IPモニタリソース](#IPモニタリソース)
- [ユーザ空間モニタリソース](#ユーザ空間モニタリソース)
- [ボリュームマネージャモニタリソース](#ボリュームマネージャモニタリソース)
- [PIDモニタリソース](#PIDモニタリソース)

## 共通パラメータ
### 監視 (共通)
- polling/interval: 監視処理のインターバル
- polling/timeout: 監視処理のタイムアウト
- emergency/timeout/notreconfirmation/use: タイムアウト発生時にリトライしない
  - 0: 無効
  - 1: 有効
- emergency/timeout/notrecovery/use: タイムアウト発生時に回復動作を実行しない
  - 0: 無効
  - 1: 有効
- polling/reconfirmation: リトライ回数
  ```bash
  $ clpcfset add monparam fipw fipw1 polling/interval 60
  $ clpcfset add monparam fipw fipw1 polling/timeout 120
  $ clpcfset add monparam fipw fipw1 polling/reconfirmation 1
  ```
- polling/timing: 監視タイミング
  - 0: 常時
  - 1: 活性時
    - 既定値が常時のモニタリソースにおいて、活性時に変更する場合、target も併せて指定してください。
- target: 監視対象のリソース
　- polling/timing が 1 の場合 (活性時監視) 、target を設定する必要があります。
  - polling/timing が 0 の場合 (常時監視) 、設定は不要です。
    ```bash
    $ clpcfset add monparam fipw fipw1 target fip1
    ```
- firstmonwait: 監視開始待ち時間
- polling/servers@\<id\>/name: 監視を行うサーバを選択する
  - 規定値である全てのサーバで監視を行う場合は、設定する必要はありません。
  - 監視を行うサーバが1つの場合、以下のように実行してください。
    ```bash
    $ clpcfset add monparam fipw fipw1 polling/servers@0/name <サーバ名>
    ```
  - 監視を行うサーバが2つ以上の場合、以下のように実行してください。
    ```bash
    $ clpcfset add monparam fipw fipw1 polling/servers@0/name <サーバ名>
    $ clpcfset add monparam fipw fipw1 polling/servers@1/name <サーバ名>
    ```

### 回復動作
- カスタム設定のみ対応しています。
- 適切なパラメータを設定することで、テンプレート(「回復対象を再起動」等)の動作と同様の回復動作を設定することができます。
  - 回復対象に対してフェイルオーバ実行
    ```bash
    $ clpcfset add monparam genw genw1 emergency/threshold/restart 0
    $ clpcfset add monparam genw genw1 emergency/threshold/fo 1
    ```
  - 回復対象を再起動、効果がなければフェイルオーバ実行
    ```bash
    $ clpcfset add monparam genw genw1 emergency/threshold/restart 1
    $ clpcfset add monparam genw genw1 emergency/threshold/fo 1
    ```
  - 回復対象を再起動
    ```bash
    $ clpcfset add monparam genw genw1 emergency/threshold/restart 1
    $ clpcfset add monparam genw genw1 emergency/threshold/fo 0
    ```
  - 最終動作のみ実行
    ```bash
    $ clpcfset add monparam genw genw1 emergency/threshold/restart 0
    $ clpcfset add monparam genw genw1 emergency/threshold/fo 0
    ```
  - カスタム設定
    - 以降に記載するパラメータから必要なものを設定してください。
- パラメータの詳細
- 回復対象
  - リソースの場合
    ```bash
    $ clpcfset add monparam <モニタリソースのタイプ名> <モニタリソース名> relation/type rsc
    $ clpcfset add monparam <モニタリソースのタイプ名> <モニタリソース名> relation/name <リソース名>
    ```
  - グループの場合
    ```bash
    $ clpcfset add monparam <モニタリソースのタイプ名> <モニタリソース名> relation/type grp
    $ clpcfset add monparam <モニタリソースのタイプ名> <モニタリソース名> relation/name <フェイルオーバグループ名>
    ```
  - LocalServer の場合
    ```bash
    add monparam <モニタリソースのタイプ名> <モニタリソース名> relation/type cls
    $ clpcfset add monparam <モニタリソースのタイプ名> <モニタリソース名> relation/name LocalServer
    ```
- emergency/threshold/script: 回復スクリプト実行回数
- emergency/preaction/userestart: 再活性前にスクリプトを実行する
  - 0: 実行しない
  - 1: 実行する
- emergency/threshold/restart: 最大再活性回数
  ```bash
  $ clpcfset add monparam genw genw1 emergency/threshold/restart 1
  ```
- emergency/preaction/usefailover: フェイルオーバ実行前にスクリプトを実行する
  - 0: 実行しない
  - 1: 実行する
- emergency/threshold/fo: 最大フェイルオーバ回数
  ```bash
  $ clpcfset add monparam genw genw1 emergency/threshold/fo 1
  ```
- emergency/preaction/usefailover: 最終動作前にスクリプトを実行する
  - 0: 実行しない
  - 1: 実行する
- emergency/action: 最終動作
  ```bash
  $ clpcfset add monparam genw genw1 emergency/action 3
  ```
  |数値|最終動作|
  |----|---------|
  |   1|何もしない|
  |   2|グループ停止|
  |   3|クラスタサービス停止|
  |   4|クラスタサービス停止とOSシャットダウン|
  |   5|クラスタサービス停止とOS再起動|
  |   6|意図的なストップエラーの発生|

- スクリプト設定
  - 回復動作でスクリプトを実行する設定をした場合は以下のパラメータも設定してください。
  - emergency/preaction/default: スクリプトのタイプ
    - 0: ユーザアプリケーション
    - 1: この製品で作成したスクリプト
  - emergency/preaction/path: ファイル
    - ユーザアプリケーションの場合: スクリプトの絶対パス
    - この製品で作成したスクリプトの場合: preaction.bat
  - emergency/preaction/timeout: タイムアウト


## フローティングIPモニタリソース
### タイプ名: fipw
- parameters/monmii: NIC Link Up/Downを監視する
  - 0: 監視しない (既定値)
  - 1: 監視する
    ```bash
    $ clpcfset add monparam genw genw1 parameters/monmii 1
    ```

## カスタムモニタリソース
### タイプ名: genw
- parameters/default: スクリプトのタイプ
    - 0: ユーザアプリケーション
    - 1: この製品で作成したスクリプト
- parameters/path ファイル
  - ユーザアプリケーションの場合: スクリプトの絶対パス
  - この製品で作成したスクリプトの場合: genw.bat

## ディスクTURモニタリソース
### タイプ名: sdw
- parameters/object: ディスクリソース
  - 対象のディスクリソースを指定してください。
    ```bash
    $ clpcfset add monparam diskw diskw1 parameters/object sd1
    ```

## ディスクRWモニタリソース
### タイプ名: diskw
- parameters/file: ファイル名
  - 対象ファイルの絶対パスを指定してください。
    ```bash
    $ clpcfset add monparam diskw diskw1 parameters/object /dev/sdc2
    ```
- parameters/size: I/Oサイズ **(default 3000000)**
- parameters/stallaction: ストール異常検出時動作
  - 0: 何もしない
  - 1: HWリセット
  - 2: 意図的なストップエラーの発生 **(default)**
- parameters/diskfullerr: ディスクフル検出時動作
  - 0: 回復動作を実行しない
  - 1: 回復動作を実行する **(default)**
- parameters/writecache: Write Through方式を有効にする
  - 0: チェックする
  - 1: チェックしない **(default)**

## IPモニタリソース
### タイプ名: ipw
- parameters/list@\<id\>/ip: IPアドレス
  - 監視対象の IPアドレスが1つの場合、以下のように実行してください。
    ```bash
    $ clpcfset add monparam ipw ipw1 parameters/list@1/ip <IP アドレス>
    ```
  - 複数の IP アドレスを監視対象とする場合、以下のように実行してください。
    ```bash
    $ clpcfset add monparam ipw ipw1 parameters/list@1/ip <IP アドレス>
    $ clpcfset add monparam ipw ipw1 parameters/list@2/ip <IP アドレス>
    ```
- parameters/pingtimeout: Pingタイムアウト **(default 5000)**

## ユーザ空間モニタリソース
### タイプ名: userw
- parameters/method: 監視方法
  - keepaliveを指定してください。
    ```bash
    $ clpcfset add monparam userw userw1 parameters/method keepalive
    ```
- parameters/stallaction: タイムアウト発生時動作
  - 0: 何もしない
  - 1: HWリセット
  - 2: 意図的なストップエラーの発生 **(default)**
- parameters/mkthread: ダミースレッドの作成
  - 0: チェックしない
  - 1: チェックする **(default)**

