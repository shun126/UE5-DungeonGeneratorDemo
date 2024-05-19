# Change Log - Procedural 3D Dungeon Generator Plug-in

## Unreleased-1.5.13 (27)
### Changes
*
### 変更点
*

## 20240519-1.5.12 (26)
### Changes
* Fixed wall exclusion information within sublevels
### 変更点
* サブレベル内の壁進入禁止情報を修正

## 20240512-1.5.11 (25)
### Changes
* Changed so that candelabras can be spawned without pillars
* Added sub-level lottery establishment
* Fixed a bug with multiplayer support
  * Changed room sensors, doors, candelabra, and interiors to spawn only on server
  * Temporarily removed `Spawn on client
### 変更点
* 柱が無くても燭台をスポーンできるように変更
* サブレベルの抽選確立を追加
* マルチプレイヤー対応の不具合を修正
  * 部屋センサー、ドア、燭台、インテリアはサーバーのみスポーンするように変更
  * `Spawn on client`の一時的な廃止

## 20240424-1.5.10 (24)
### Changes
* Added door generation probability to DungeonRoomSensor
* Unreal Engine 5.4 support
### 変更点
* ドアの生成確率をDungeonRoomSensorに追加
* Unreal Engine 5.4対応

## 20240327-1.5.9 (23)
### Changes
* Fixed several bugs
### 変更点
* いくつかの不具合を修正

## 20240325-1.5.8 (22)
### Changes
* パラメータへのコメントを追加
* 生成時の余白を追加
### 変更点
* Added comments to parameters
* Added margins for generation

## 20240223-1.5.7 (21)
### Changes
* Changed to discard Editor-only actors at runtime generation
* Fixed several bugs
### 変更点
* ランタイム生成時にEditorのみのアクターを破棄するように変更
* いくつかの不具合を修正

## 20240210-1.5.6 (20)
### Changes
* Added rules for generating torch
* Fixed several bugs
### 変更点
* 燭台の生成ルールを追加
* いくつかの不具合を修正

## 20240201-1.5.5 (19)
### Changes
* Extended the ability to choose to spawn actors in the client
### 変更点
* クライアントでのアクターのスポーンを選択できるように拡張

## 20240129-1.5.4 (18)
### Changes
* Fixed a replication bug.
### 変更点
* レプリケーションの不具合を修正

## 20240126-1.5.3 (17)
### Changes
* Discontinued support for Unreal Engine 4
* Changed door generation to before room sensors
* Added function to get door from sensor
* Added function to get candlestick from sensor
* Added MissionGraph validity specification
* Added complex passage generation
* Added dungeon generation mode with no hierarchy
* Replication changed to standard enabled
### 変更点
* Unreal Engine 4のサポートを終了
* ドアの生成を部屋のセンサーの前に変更
* センサーからドアを取得する関数を追加
* センサーから燭台を取得する関数を追加
* MissionGraph有効性の指定を追加
* 複雑な通路の生成を追加
* 階層のないダンジョン生成モードを追加
* レプリケーションを標準で有効に変更

## 20231211-1.5.2 (16)
### Changes
* Fixed a bug that prevented specifying doors.
### 変更点
* ドアを指定できない不具合を修正

## 20231202-1.5.1 (15)
### Changes
* Support for Android (UE5.3 only)
* Added a system to support load reduction
* Added a system for specifying the prohibition of generating walls and doors to level streaming
* Fixed several bugs
### 変更点
* Androidに対応 (UE5.3のみ)
* 負荷軽減をサポートするシステムを追加
* レベルストリーミングに壁やドアの生成禁止を指定するシステムを追加
* いくつかの不具合を修正

## 20231022-1.5.0 (14)
### Changes
* Added room and passageway mesh set assets
* Fixed some bugs
### 変更点
* 部屋と通路のメッシュセットアセットを追加
* いくつかの不具合を修正

## 20230930-1.4.8 (13)
### Changes
* Fixed a bug when the start room was not set
### 変更点
* スタート部屋を未設定の時の不具合を修正

## 20230920-1.4.7 (12)
### Changes
* Register dungeon mesh in DungeonPartsDatabase asset
* Fixed some bugs
### 変更点
* DungeonPartsDatabaseアセットにダンジョンのメッシュを登録
* いくつかの不具合を修正

## 20230908-1.4.6 (11)
### Changes
* Support for Unreal Engine 5.3
* Unreal Engine 4 no longer supported
* Support for loading sublevels in the start and finish rooms
* Added plug-in content
### 変更点
* Unreal Engine 5.3に対応
* Unreal Engine 4のサポートを終了
* スタート部屋、ゴール部屋のサブレベル読み込みに対応
* プラグインコンテンツの追加

## 20230901-1.4.5 (10)
### Changes
* Modified to generate dungeons on a flat surface if room merging or room margins are less than 1
* Added minimap information asset
* Added world space to texture space conversion class
* modified paths for plugin content
* Added sample data to plugin, eliminated dungeon hierarchy specification
* Fixed cache misalignment in system tags
* Started network functionality verification
* Moved PlayerStart off center of start room if more than one PlayerStart was installed
### 変更点
* 部屋の結合または部屋の余白が１以下ならば平面にダンジョンを生成するように修正
* ミニマップ情報アセットを追加
* ワールド空間からテクスチャ空間への変換クラスを追加
* プラグインコンテンツのパスを修正
* プラグインにサンプルデータを追加、ダンジョンの階層指定を廃止
* システムタグのキャッシュずれを修正
* ネットワーク機能検証開始
* PlayerStartが複数設置されていた場合はスタート部屋の中心からずらして配置

## 20230801-1.4.4 (9)
### Changes
* Interior Decorator beta version released
* Added interior assets
* Fixed some bugs
### 変更点
* インテリアデコレーターベータ版リリース
* インテリアアセットを追加
* いくつかの不具合を修正

## 20230514-1.4.3 (8)
### Changes
* Interior Decorator Verification
* Removed sample models as plug-in assets were added
### 変更点
* インテリアデコレーターの検証
* プラグインアセットを追加に伴ってサンプルモデルを削除

## 20230514-1.4.2 (7)
### Changes
* UE5.2 support
* Supports sub-level merging
* Copy LevelStreaming actors in editor mode to level
### 変更点
* Unreal Engine 5.2に対応
* サブレベルのマージに対応
* エディタモードの LevelStreamingアクターをレベルにコピー

## 20230409-v1.4.1 (6)
### Changes
* Confirmed that the package can be created.
* Add a test that generates pre-created sublevels in the room.
* Add a test for vertical margins.
* Add a test generation of minimap texture assets.
### 変更点
* パッケージが作成できることを確認
* 部屋にあらかじめ作成されたサブレベルを生成するテスト
* 垂直マージンのテスト
* ミニマップのテクスチャアセットの生成テスト

## 20230403-v1.4.0 (5)
### Changes
* Generate mini-maps in two types of pixel size and resolution (dots/meters)
### 変更点
* ミニマップをピクセルサイズと解像度（ドット/メートル）の二種類から生成

## 20230321-v1.3.1 (4)
### Changes
* Fixed mini-map generationo fail.
### 変更点
* ミニマップ生成時にクラッシュする問題を修正

## 20230319-v1.3.0 (3)
### Changes
* Supports mini-maps
### 変更点
* ミニマップに対応

## 20230316-v1.2.0 (2)
### Changes
* Compatible with Unreal Engine 4.27.2
### 変更点
* Unreal Engine 4.27.2に対応

## 20230308-v1.0.1 (1)
### Changes
* Fixed hang-up when referencing minimap textures when dungeon creation fails.
* Changed random room placement method to be more randomly distributed.
* Improved dungeon creation speed.
### 変更点
* ダンジョン生成に失敗した時にミニマップのテクスチャを参照するとハングアップする問題を修正
* 部屋のランダム配置方法をよりランダムに分散するよう変更
* ダンジョン生成速度を改善

## 20230303-v1.0.0 (0)
### Changes
* Initial release version
### 変更点
* 初回リリース版
