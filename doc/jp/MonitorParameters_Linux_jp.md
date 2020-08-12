# Monitor Parameters for Linux cluster
- モニタリソースパラメータの詳細について説明しています。
- **(default)** と記載された設定値を利用する場合は、コマンドでパラメータを設定する必要はありません。
- **ver1**を使用する場合は、コマンド例の**clpcfset**を**clpcreate**に置き換えてください。
- 各パラメータに入力可能な文字列や禁則文字列については、CLUSTERPRO のリファレンスガイドを参照してください。

## 目次
- [共通パラメータ](#共通パラメータ)
- [ディスクRWモニタリソース](#ディスクRWモニタリソース)
- [フローティングIPモニタリソース](#フローティングIPモニタリソース)
- [カスタムモニタリソース](#カスタムモニタリソース)
- [IPモニタリソース](#IPモニタリソース)
- [PIDモニタリソース](#PIDモニタリソース)
- [プロセス名モニタリソース](#プロセス名モニタリソース)
- [ユーザ空間モニタリソース](#ユーザ空間モニタリソース)
- [ボリュームマネージャモニタリソース](#ボリュームマネージャモニタリソース)

## 共通パラメータ
### 監視 (共通)
- polling/interval: 監視処理のインターバル
- polling/timeout: 監視処理のタイムアウト
- emergency/dumpcollect/use: タイムアウト発生時に監視プロセスのダンプを採取する
  - usew, genwでは設定できないことにご注意ください。
  - 0: 無効 **(default)**
  - 1: 有効
- emergency/timeout/notreconfirmation/use: タイムアウト発生時にリトライしない
  - 0: 無効
  - 1: 有効 **(default)**
- emergency/timeout/notrecovery/use: タイムアウト発生時に回復動作を実行しない
  - 0: 無効
  - 1: 有効 **(default)**
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
- emergency/preaction/use: 最終動作前にスクリプトを実行する
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
  |   8|sysrqパニック|
  |   9|keepaliveリセット|
  |  10|keepaliveパニック|
  |  11|BMCリセット|
  |  12|BMCパワーオフ|
  |  13|BMCパワーサイクル|
  |  14|BMC NMI|
  |  15|I/O Fencing(High-End Server Option)|
  |  16|リソース停止|

- スクリプト設定
  - 回復動作でスクリプトを実行する設定をした場合は以下のパラメータも設定してください。
  - emergency/preaction/default: スクリプトのタイプ
    - 0: ユーザアプリケーション
    - 1: この製品で作成したスクリプト
  - emergency/preaction/path: ファイル
    - ユーザアプリケーションの場合: スクリプトの絶対パス
    - この製品で作成したスクリプトの場合: preaction.sh
  - emergency/preaction/timeout: タイムアウト

## ディスクRWモニタリソース
### タイプ名: diskw
- parameters/object: 監視先
  - 監視先のディスクのパスを指定してください。
    ```bash
    $ clpcfset add monparam diskw diskw1 parameters/object /dev/sdc2
    ```
- parameters/size: I/Oサイズ
  - 監視方法がREAD(O_DIRECT)のときは512を指定してください。

## フローティングIPモニタリソース
### タイプ名: fipw
- parameters/monmii: NIC Link Up/Downを監視する
  - 0: 監視しない **(default)**
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

## IPモニタリソース
### タイプ名: ipw
- parameters/list@\<id\>/ip: IPアドレス
  - 監視対象の IPアドレスが1つの場合、以下のように実行してください。
    ```bash
    $ clpcfset add monparam ipw ipw1 parameters/list@0/ip <IP アドレス>
    ```
  - 複数の IP アドレスを監視対象とする場合、以下のように実行してください。
    ```bash
    $ clpcfset add monparam ipw ipw1 parameters/list@0/ip <IP アドレス>
    $ clpcfset add monparam ipw ipw1 parameters/list@1/ip <IP アドレス>
    ```

## PIDモニタリソース
### タイプ名: pidw
- 固有のパラメータはありません。

## プロセス名モニタリソース
## タイプ名: psw
- parameters/processname: プロセス名
- parameters/processnum: プロセス数下限値

## ユーザ空間モニタリソース
### タイプ名: userw
- parameters/method: 監視方法
  - keepalive: X 4.x 以降の既定値
  - softdog: X 3.x までの既定値
    ```bash
    $ clpcfset add monparam userw userw1 parameters/method keepalive
    ```
- parameters/action: タイムアウト発生時動作
  - RESET
  - PANIC (softdog の場合は既定値かつ固定値のためコマンドでは指定不要)
    ```bash
    $ clpcfset add monparam userw userw1 parameters/action PANIC
    ```

## ボリュームマネージャモニタリソース
### タイプ名: volmgrw
- parameters/devname: VG 名を指定してください。
  ```bash
  $ clpcfset add monparam volmgrw volmgrw parameters/devname <VG 名>
  ```
