= ProGurad manual for Android

== はじめに
#@# TODO: この章ではどんなことが書かれているのか、なぜ書こうと思ったのかとかを書く

== ProGuardを理解する
=== ProGuard実行時の流れ
//image[ProGuard_build_process][ProGuardのステップ一覧]{
    ProGuardのステップ一覧
//}

ProGuardは、Javaクラスファイルの圧縮（shrink）、最適化（optimize）、難読化（obfuscate）、および事前検証（preverify）を行うツールです。
@<img>{ProGuard_build_process}は@<href>{https://www.guardsquare.com/en/proguard/manual/introduction, ProGuard manual}に記載されているProGuardのステップ一覧です。
先のような効果をもたらすツールは他にもありますが、ProGuardはAndroid SDKの一部として提供されているのでAndroid開発者がすぐに使い始めることが可能です。

圧縮（shrink）ステップでは、未使用のクラス、フィールド、メソッド、および属性を検出して削除します。最適化（optimize）ステップでは、メソッドのバイトコードを解析して最適化します。難読化（obfuscate）ステップは、削除されずに残っているクラス、フィールド、およびメソッドの名前を短い意味の取れない名前に変更します。これらのステップは、コードベースをより小さく、より効率的に、リバースエンジニアリングをより困難にします。最後の事前検証（preverify）ステップでは、クラスに事前確認情報を追加します。これはJava Micro EditionやJava 6以降で必要です。

各ステップを実行するかどうかはオプションで選択することが可能です。たとえば、使っていないクラスやフィールドなどの削除だけを行うために圧縮（shrink）ステップのみ実行することやJava6での実行効率を向上するために事前検証（preverify）ステップを実行するといった方法です。

=== エントリーポイントとは
//footnote[Applets][ネットワークを通してWebブラウザに読み込まれ実行されるJavaのアプリケーションの一形態]
//footnote[MIDlet][Java MEで定義されている携帯小型端末向けアプリケーション形式]
ProGuardはどのコードを変更されないようにするか、またどのコードを削除したり難読化したりすべきかを決定するため、1つかそれ以上のエントリーポイントを指定する必要があります。通常、これらのエントリーポイントは、mainメソッドをもつApplets@<fn>{Applets}、MIDlet@<fn>{MIDlet}、AndroidのActivity、Service、Broadcast Reciver、Content Provider、Custom Application Classなどです。

=== リフレクション


== Androidアプリ開発におけるProGuardを理解する

=== Androidアプリ向けにデフォルトで用意されているProGuard設定ファイル

=== ProGuard実行後に作成されるファイル

=== ライブラリ配布時に便利なconsumerProguardFilesの設定

//list[consumerProguardFiles][build.gradle]{
android {
    consumerProguardFiles 'proguard-rules.pro'
}
//}

== ProGuardを適応したAndroidアプリを作る

== ProGuradの代わりに開発中のR8について

== おわりに
