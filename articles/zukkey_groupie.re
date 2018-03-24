= Groupie実践入門
こんにちは、初めまして。
株式会社エウレカにてPairsJPのAndroidの開発を行っている@<href>{https://twitter.com/zukkey59, @zukkey}です。
今回は、Android開発で複雑なレイアウトを組むときのRecyclerViewをよしなにやってくれるライブラリのGroupieについてご紹介いたします。

== Groupieとは
@<href>{https://github.com/lisawray/groupie, Groupie}は、 Lisa Wrayさんが作成・提供しているライブラリでAndroid開発でのRecyclerViewの複雑なレイアウトを簡単にしてくれます。

公式のREADMEには次のように書かれています。

//quote{
Groupie is a simple, flexible library for complex RecyclerView layouts.

Groupie lets you treat your content as logical groups and handles change notifications for you -- think sections with headers and footers, expandable groups, blocks of vertical columns, and much more.

It makes it easy to handle asynchronous content updates and insertions and user-driven content changes.

At the item level, it abstracts the boilerplate of item view types, item layouts, viewholders, and span sizes.

//}

簡単に訳すと、複雑なRecyclerViewのレイアウトを簡単にして、ヘッダーやフッターなどをセクションと考えて論理的なグループという単位でコンテンツを扱えるようになるということです。

=== Groupieの導入
まずはじめに、Groupieを導入するためにいくつかgradleに設定する必要があります。@<br>{}

先に共通で行っておくべきことについて触れ、あとでDataBindingを使用する時と使用しない時で分けて説明していきます。@<br>{}

公式のissueでも取り上げられているviewholderのviewのキャッシュが効かなくなる問題があるので、最初に次の一文をappのgradleの中で定義してください。

//emlist[][]{
  androidExtensions {
    experimental = true
  }
//}

次に、compileSDKVersionとtargetVersionを２７まで上げ、buildToolsVersionを'27.0.2'にする必要があります。
これを追加しないとエラーが吐かれるので追加してください。（2018年３月現在）
//emlist[][]{
  android{
    ...
    compileSDKVersion 27
    defaultConfig {
      targetSDKVersion 27
      buildToolsVersion "27.0.2"
    }
    ...
  }
//}

また、com.android.support:appcompat-v7とcom.android.support:designも'27.0.2'にする必要があるので、appのgradleの中で次のように追加してください。
//emlist[][]{
  dependencies {
    ...
    implementation 'com.android.support:appcompat-v7:27.0.2'
    implementation 'com.android.support:design:27.0.2'
    ...
  }
//}

DataBindingを使用する場合と、使用しない場合によって異なるため分けて説明していきます。　　

==== DataBindingを使用しない場合
この節では、DataBindingを使用しない場合について話していきます。@<br>{}
DataBindingを使用しない場合は、次の2行をappのgradleの中に記述を加えてください。

//emlist[][]{
  compile 'com.xwray:groupie:2.0.3'

  compile 'com.xwray:groupie-kotlin-android-extensions:2.0.3'
//}

DataBindingを使用しない場合はこれで完了です。

==== DataBindingを使用する場合
この節では、DataBindingを使用する場合について話していきます。@<br>{}
DataBindingを使用する場合は、次の2行をappのgradleの中のdependenciesに記述を加えてください。

//emlist[][]{
    compile 'com.xwray:groupie:2.0.3'

    compile 'com.xwray:groupie-kotlin-android-extensions:2.0.3'

    compile 'com.xwray:groupie-databinding:2.0.3'
//}

また、DataBindingを使用する場合は下記をappのgradleの中に追加する必要があります。
//emlist[][]{
  dataBinding {
        enabled = true
    }
//}
このままだと、kaptのエラーが出るので次の1行をappのgradleの中で上の方に追加してください。
//emlist{
  apply plugin: 'kotlin-kapt'
//}

最後に、appのgradleの中のdependenciesに次の1行を追加してください。
//emlist[][]{
  kapt 'com.android.databinding:compiler:3.0.1'
//}

これで、DataBindingに対応したGroupieの導入が完了しました。
