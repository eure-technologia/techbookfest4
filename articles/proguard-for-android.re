= ProGurad manual for Android

== ProGuardとは

ProGuardは、Javaクラスファイルの圧縮（shrink）、最適化（optimize）、難読化（obfuscate）、および事前検証（preverify）を行うツールです。
上記のような効果をもたらすツールは他にもありますが、ProGuardはAndroid SDKの一部として提供されているのでAndroid開発者がすぐに使い始めることが可能です。

圧縮するステップでは、未使用のクラス、フィールド、メソッド、および属性を検出して削除します。最適化ステップは、メソッドのバイトコードを解析して最適化します。難読化ステップは、短い無意味な名前を使用して、残りのクラス、フィールド、およびメソッドの名前を変更します。これらの最初のステップは、コードベースをより小さく、より効率的に、リバースエンジニアリングするのをより困難にします。最終的な事前確認ステップでは、クラスに事前確認情報を追加します。これはJava Micro EditionおよびJava 6以降で必要です。

これらの各ステップはオプションです。たとえば、ProGuardを使用すると、デッドコードをアプリケーションにリストするだけでなく、Java6で効率的に使用できるようにクラスファイルを事前検証することもできます。

//image[ProGuard_build_process][ProGuardのステップ一覧]{
    ProGuardのステップ一覧
//}

== Androidアプリ開発でProGuardを使うと嬉しいこと

== Androidアプリ向けにデフォルトで用意されているProGuard設定ファイル

== ProGuard実行後に作成されるファイル

== ライブラリ配布時に便利なconsumerProguardFilesの設定

//list[consumerProguardFiles][build.gradle]{
android {
    consumerProguardFiles 'proguard-rules.pro'
}
//}
