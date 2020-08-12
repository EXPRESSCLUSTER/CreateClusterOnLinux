# Resource Parameters for Linux cluster
- リソースパラメータの詳細について説明しています。
- **(default)** と記載された設定値を利用する場合は、コマンドでパラメータを設定する必要はありません。
- **ver1**を使用する場合は、コマンド例の**clpcfset**を**clpcreate**に置き換えてください。
- 各パラメータに入力可能な文字列や禁則文字列については、CLUSTERPRO のリファレンスガイドを参照してください。

## 目次
- [共通パラメータ](#共通パラメータ)
- [ディスクリソース](#ディスクリソース)
- [EXECリソース](#EXECリソース)
- [フローティングIPリソース](#フローティングIPリソース)
- [ボリュームマネージャリソース](#ボリュームマネージャリソース)

## 共通パラメータ
### 復旧動作
- act/retry: 活性リトライしきい値
- act/fo: フェイルオーバしきい値
- act/action: (活性異常検出時の) 最終動作
- deact/retry: 非活性リトライしきい値
- deact/action: (非活性異常検出時の) 最終動作
  ```bash
  $ clpcfset add rscparam fip fip1 act/retry 1
  $ clpcfset add rscparam fip fip1 act/fo 1
  $ clpcfset add rscparam fip fip1 act/action 2
  $ clpcfset add rscparam fip fip1 deact/retry 1
  $ clpcfset add rscparam fip fip1 deact/action 5
  ```
  |数値|最終動作|備考|
  |----|---------|--|
  |   0|何もしない (次のリソースを活性/非活性する)||
  |   1|何もしない (次のリソースを活性/非活性しない)|**(act/actionのdefault)**|
  |   2|グループ停止|活性異常検出時のみ指定可能|
  |   3|クラスタサービス停止|活性異常検出時のみ指定可能|
  |   4|クラスタサービス停止とOSシャットダウン|**(deact/actionのdefault)**|
  |   5|クラスタサービス停止とOS再起動||
  |   8|sysrqパニック||
  |   9|keepaliveリセット||
  |  10|keepaliveパニック||
  |  11|BMCリセット||
  |  12|BMCパワーオフ||
  |  13|BMCパワーサイクル||
  |  14|BMC NMI||
  |  15|I/O Fencing(High-End Server Option)||

## ディスクリソース
### タイプ名: disk
- parameters/disktype: ディスクのタイプ
  - disk, raw, lvm, vxvm から指定してください
- parameters/fs: ファイルシステム
  - ext3, ext4, xfs, reiserfs, vxfs, zfs から指定してください
- parameters/device: デバイス名
- parameters/rawdevice: RAWデバイス名
- parameters/mount/point: マウントポイント
- parameters/fsck/timing: Mount実行前のfsckアクション
  - 0: 実行しない
  - 1: 必ず実行する
  - 2: 指定回数に達したら実行する **s(default)**
- parameters/fsck/interval: Mount実行前のfsckアクションを実行するまでのMount回数　**(default 10回)**
  - parameters/fsck/timing を 2 に設定した場合にのみ設定してください。

## EXECリソース
### タイプ名: exec
- parameters/act/path: Start script のパス
  - **この製品で作成したスクリプト (default)** の場合、**start.sh** を指定してください。
  - **ユーザアプリケーション** の場合、任意のパス (e.g. /opt/test/start.sh) を指定してください。
- parameters/deact/path: Stop script のパス
  - **この製品で作成したスクリプト (default)** の場合、**stop.sh** を指定してください。
  - **ユーザアプリケーション** の場合、任意のパス (e.g. /opt/test/stop.sh) を指定してください。
- parameters/timeout/start: 開始スクリプトのタイムアウト **(default 1800)**
- parameters/timeout/stop: 終了スクリプトのタイムアウト **(default 1800)**
- parameters/act/sync: 開始スクリプトの同期/非同期
  - 0: 非同期
  - 1: 同期 **(default)**
- parameters/deact/sync: 終了スクリプトの同期/非同期
  - 0: 非同期
  - 1: 同期 **(default)**

## フローティングIPリソース
### タイプ名: fip
- parameters/ip: フローティング IP アドレス
  - インタコネクトと同じサブネットの IP アドレスを指定してください。

## ボリュームマネージャリソース
### タイプ名: volmgr
- parameters/type
  - LVM を使う場合 **lvm** を指定してください。
- parameters/devname
  - LVM を使う場合、VG 名を指定してください。