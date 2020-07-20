# Group Parameters for Windows cluster
- グループパラメータの詳細について説明しています。
- **(default)** と記載された設定値を利用する場合は、コマンドでパラメータを設定する必要はありません。
- 各パラメータに入力可能な文字列や禁則文字列については、CLUSTERPRO のリファレンスガイドを参照してください。

## 目次
- [起動サーバ](#起動サーバ)
- [属性](#属性)
- [起動待ち合わせ](#起動待ち合わせ)
- [停止待ち合わせ](#停止待ち合わせ)

## 起動サーバ
- 起動可能なサーバを個別に設定する場合は、以下のパラメータを設定してください。(デフォルトは全てのサーバでフェイルオーバ可能)
- policy@\<サーバ名\>/order: 起動可能なサーバと優先順位
- 優先順位は0が最も高い
  ```bash
  $ clpcreate add grpparam failover failover1 policy@server1/order 0
  $ clpcreate add grpparam failover failover1 policy@server2/order 1
  ```

## 属性
- start: グループ起動属性
  - 0: 手動起動
  - 1: 自動起動 **(default)**
    ```
    $ clpcreate add grpparam failover failover1 start 0
    ```

- failover: フェイルオーバ属性
  - 0: 手動フェイルオーバ
  - 1: 自動フェイルオーバ **(default)**
    ```
    $ clpcreate add grpparam failover failover1 failover 0
    ```

- failbackフェイルバック属性
  - 0: 手動フェイルバック **(default)**
  - 1: 自動フェイルバック
    ```
    $ clpcreate add grpparam failover failover1 failback 1
    ```

- exclusive: フェイルオーバ排他属性 (X 3.3.5のみ)
  - 0: 排他なし **(default)**
  - 1: 通常排他
  - 2: 完全排他
    ```
    $ clpcreate add grpparam failover failover1 failback 1
    ```

## 起動待ち合わせ
- depend/act/depend@<グループ名>: 対象グループ
  - 最後の引数に空文字を指定してください。
  ```
  $ clpcreate add grpparam failover failover1 depend/act/depend@failover2 ""
  $ clpcreate add grpparam failover failover1 depend/act/depend@failover3 ""
  ```

- depend/act/depend@<グループ名>/sameserver: 同じサーバで起動する場合のみ待ち合わせを行う
  - 0: 待ち合わせを行わない **(default)**
  - 1: 待ち合わせを行う
  ```
  $ clpcreate add grpparam failover failover1 depend/act/depend@failover2/sameserver 1
  ```

- depend/act/timeout: 対象グループの起動待ち時間 **(default 1800)**
  ```
  $ clpcreate add grpparam failover failover1 depend/act/timeout 1000
  ```

## 停止待ち合わせ
- depend/deact/depend@<グループ名>: 対象グループ
  - 最後の引数に空文字を指定してください。
  ```
  $ clpcreate add grpparam failover failover1 depend/deact/depend@failover2 ""
  $ clpcreate add grpparam failover failover1 depend/deact/depend@failover3 ""
  ```

- depend/deact/timeout: 対象グループの起動待ち時間 **(default 1800)**
  ```
  $ clpcreate add grpparam failover failover1 depend/deact/timeout 1000
  ```

- depend/deact/cluster/use: クラスタ停止時に対象グループの停止を待ち合わせる
  - 0: 待ち合わせない
  - 1: 待ち合わせる **(default)**
  ```
  $ clpcreate add grpparam failover failover1 depend/deact/cluster/use 0
  ```

- depend/deact/server/use: サーバ停止時に対象グループの停止を待ち合わせる
  - 0: 待ち合わせない **(default)**
  - 1: 待ち合わせる
  ```
  $ clpcreate add grpparam failover failover1 depend/deact/server/use 1
  ```