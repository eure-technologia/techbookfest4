= AndroidのためのProGuard入門

== はじめに
#@# TODO: この章ではどんなことが書かれているのか、なぜ書こうと思ったのかとかを書く

== ProGuardを理解する
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

圧縮（shrink）ステップでは、ProGuardはどのクラスやコードが使用されているのか再帰的にチェックしていく作業をさきに述べたエントリーポイントから開始します。そして使用されていないクラスやコードの全てが破棄されます。@<img>{entory_point}
//image[entory_point][エントリーポイント]{
    エントリーポイント
//}

最適化（optimize）ステップでは、圧縮（shrink）ステップを経たコードを元に最適化が進められます。エントリーポイントではないクラスとメソッドがprivateや、staticや、finalにされたり、使用されていない引数が削除されたり、いくつかのメソッドがインライン化されたりします。

難読化（obfuscate）ステップでは、エントリーポイントではないクラスとクラスメンバーの名前が変更されます。このステップ全体では、エントリーポイントを保持し続けることで変更されたクラスとクラスメンバーが元の名前でアクセス可能であることが保証されています。

事前検証（preverify）ステップだけはエントリーポイントに影響を受けることはありません。

=== リフレクションの対応
//footnote[リフレクション][実行時にオブジェクトの情報を参照して、それを元にオブジェクトを操作する方法]
//footnote[イントロスペクション][実行時にオブジェクトの情報を参照して、その情報にさらに変更を加える方法]
リフレクション@<fn>{リフレクション}やイントロスペクション@<fn>{イントロスペクション}を使用しているコードに対してProGuradを実行する場合、各ステップの実行において問題が発生する場合があります。ProGuardでは、コードの中で動的に作成または実行されるクラスまたはクラスメンバーは、エントリーポイントと同様に事前に指定されている必要があるからです。
たとえば、@<code>{Class.forName("SomeClass")}メソッドは、実行時に任意のクラスへの参照を返します。コンフィギュレーションファイルからクラス名が読み取らたあとで、その一覧には無いクラスや動的に生成されるクラスのうち、どのクラスやメソッドがProGuardによる処理をされた後の名前になっているのか、どのクラスやメソッドがProGuardによる処理をされておらず、元の名前のままになっているのかということを予測するのは一般的に不可能です。このためProGuardの処理の対象としたくないクラスやクラスメンバーは、@<em>{-keep}オプションにより明確に設定する必要があります。

しかし、次のようなコードの場合にはProGuradの標準の処理によって適切な処理が行われます。
//list[reflection][リフレクション例]{
Class.forName("SomeClass")
SomeClass.class
SomeClass.class.getField("someField")
SomeClass.class.getDeclaredField("someField")
SomeClass.class.getMethod("someMethod", new Class[] {})
SomeClass.class.getMethod("someMethod", new Class[] { A.class })
SomeClass.class.getMethod("someMethod", new Class[] { A.class, B.class })
SomeClass.class.getDeclaredMethod("someMethod", new Class[] {})
SomeClass.class.getDeclaredMethod("someMethod", new Class[] { A.class })
SomeClass.class.getDeclaredMethod("someMethod", new Class[] { A.class, B.class })
AtomicIntegerFieldUpdater.newUpdater(SomeClass.class, "someField")
AtomicLongFieldUpdater.newUpdater(SomeClass.class, "someField")
AtomicReferenceFieldUpdater.newUpdater(SomeClass.class, SomeType.class, "someField")
//}

== Androidアプリ開発におけるProGuardを理解する
ここまでは一般的なProGuradについて説明してきました。
ここからはAndroidアプリ開発におけるProGuradについて説明していきたいと思います。

=== Androidアプリ向けにデフォルトで用意されているProGuard設定ファイル
Androidアプリ開発におけるProGuardですが、デフォルトで用意されている設定ファイルがいくつか存在します。
@<em>{~/AndroidSDK/tools/proguard}配下にある、@<em>{proguard-android-optimize.txt}と@<em>{proguard-android.txt}の2つです。この2つの設定の違いは最適化（optimize）ステップを実行するかどうかの違いがあります。@<em>{proguard-android-optimize.txt}の方には最適化（optimize）ステップを行う際の@<list>{optimize-diff}のような部分が記載されています。

//list[optimize-diff][proguard-android-optimize.txtとproguard-android.txtの差分]{
-optimizations !code/simplification/arithmetic
                ,!code/simplification/cast,!field/*,!class/merging/*
-optimizationpasses 5
-allowaccessmodification
//}

@<em>{-optimizations}を追加することで最適化（optimize）ステップを実行します。オプションをつけることで特定の最適化（optimize）のみを実行することや逆に特定の最適化（optimize）を行わない指定も可能です。
@<list>{optimize-diff}の場合は変数への読み書き、キャストの実行、変数に関わる部分の最適化（書き込みだけの変数の削除）、分岐するが同じ処理をしている部分のマージなどを行わないオプションを指定しています。
他にもさまざまなオプションがありますのでぜひ@<href>{https://www.guardsquare.com/en/proguard/manual/optimizations, 公式サイト | optimizations}を参考にしてみてください。


=== ProGuard実行後に作成されるファイル
AndroidアプリでProGuardを実行するといくつかのtxtファイルが生成されます。実際にbuild.gradleで設定しているproductfFlavorsとbuildTypesの内容によって異なりますが、デフォルトのまま実行している場合は概ね次のようなPathの構成でtxtファイルが生成されるはずです。
//list[generaeted-txt-files][生成されるtxtファイル一覧]{
ProjectRoot/app/build/outputs/
    └── mapping
        └── debug
            ├── dump.txt
            ├── mapping.txt
            ├── seeds.txt
            └── usage.txt
//}

それぞれのファイルの内容は次のようになっています。

 * dump.txt：ProGuardを実行した上で作成したapk内のすべてのクラスファイルの内部構造
 * mapping.txt：難読化される前のクラス、メソッド、フィールド名と難読化後の名前間のマッピング
 * seeds.txt：難読化されていないクラスとフィールドのリスト
 * usage.txt：apk内から削除されたコードの一覧

Crashlyticsのようなサードパーティ性のツールを使っている際に、ProGuard実行済みのAndroidアプリでもクラスやメソッドの名前が難読化される前のもので見ることができるのはこれらのtxtファイルを使って難読化前の状態を復元しているからです。

GoogleDeveloperコンソールの場合は開発者が手動でmapping.txtをアップロードすることで解決していますが、Crashlyticsの場合は自動でやってくれています。賢いですね。

=== ライブラリ配布時に便利なconsumerProguardFilesの設定
一般的なAndroidアプリでProGuardを使う場合に必要な最低限の設定は、さきの節で登場した@<em>{proguard-android-optimize.txt}か@<em>{proguard-android.txt}のどちらかを使うことでまかなうことが可能です。
しかしAndroidアプリ開発ではいくつかのデファクトになっているサードパーティ製のライブラリもあります。これらのライブラリを使っている時のProGuardの設定を利用者側に意識させずにいい感じにできるようにするのが@<em>{consumerProguardFiles}です。

ライブラリ作成者は@<list>{consumerProGuardFiles}のようにbuild.gradleに書き足すことでproguard-rules.proに記載されているProGuardルールをライブラリ利用者に意識させること無く提供することが可能です。

//list[consumerProGuardFiles][build.gradle]{
android {
    consumerProguardFiles 'proguard-rules.pro'
}
//}

ORMのOrmaはこの設定が記述されていたりします。

== ProGuardを適応したAndroidアプリを作る
Try And Errorでやっていきます。

== ProGuradの代わりに開発中のR8について

== おわりに

== 参考URL
* @<href>{https://www.youtube.com/watch?v=AdfKNgyT438, Best Practices to Slim Down Your App Size（Google I/O '17）}