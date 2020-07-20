# Resource Parameters for Windows cluster
- リソースパラメータの詳細について説明しています。
- **(default)** と記載された設定値を利用する場合は、コマンドでパラメータを設定する必要はありません。
- **ver1**を使用する場合は、コマンド例の**clpcfset**を**clpcreate**に置き換えてください。
- 各パラメータに入力可能な文字列や禁則文字列については、CLUSTERPRO のリファレンスガイドを参照してください。

## 目次
- [共通パラメータ](#共通パラメータ)
- [ディスクリソース](#ディスクリソース)
- [スクリプトリソース](#スクリプトリソース)
- [フローティングIPリソース](#フローティングIPリソース)

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
  |   6|意図的なストップエラーの発生||

## ディスクリソース
### タイプ名: sd
- parameters/volumemountpoint: ドライブ文字
  - ドライブ文字をアルファベット一文字で指定してください。
- server@<サーバ名>/parameters/volumeguid: 起動可能サーバのGUID
  - 両サーバについて指定してください。

## スクリプトリソース
### タイプ名: script
- クラスタ構成情報を配信する前にstart.batとstop.batを適切なパスに配置してください。

## フローティングIPリソース
### タイプ名: fip
- parameters/ip: フローティング IP アドレス
  - インタコネクトと同じサブネットの IP アドレスを指定してください。