={start} はじめに
こんにちは、初めまして。
株式会社エウレカにてPairsJPのAndroidの開発を行っている@<href>{https://twitter.com/zukkey59, @zukkey}です。
@<br>{}
今回は、Android開発で複雑なレイアウトを組むときのRecyclerViewをよしなにやってくれるライブラリのGroupieについてご紹介いたします。

第2章以降で話す内容は次のとおりです。

 * Groupieとは
 ** Groupieの導入
 *** DataBindingを使用しない場合
 *** DataBindingを使用する場合
@<br>{}
@<br>{}

={groupie} Groupieとは
@<href>{https://github.com/lisawray/groupie, Groupie}は、 Lisa Wrayさんが作成・提供しているライブラリでAndroid開発でのRecyclerViewの複雑なレイアウトを簡単にしてくれます。

公式のREADMEには次のように書かれています。

//quote{
Groupie is a simple, flexible library for complex RecyclerView layouts.

Groupie lets you treat your content as logical groups and handles change notifications for you -- think sections with headers and footers, expandable groups, blocks of vertical columns, and much more.

It makes it easy to handle asynchronous content updates and insertions and user-driven content changes.

At the item level, it abstracts the boilerplate of item view types, item layouts, viewholders, and span sizes.

//}

簡単に訳すと、複雑なRecyclerViewのレイアウトを簡単にして、ヘッダーやフッターなどをセクションと考えて論理的なグループという単位でコンテンツを扱えるようになるということです。
@<br>{}
@<br>{}

=={intro} Groupieの導入
まずはじめに、Groupieを導入するためにいくつかgradleに設定する必要があります。@<br>{}

先に共通で行っておくべきことについて触れ、あとでDataBindingを使用する時と使用しない時で分けて説明していきます。@<br>{}

公式のissueでも取り上げられているviewholderのviewのキャッシュが効かなくなる問題があるので、最初に次の一文をappのgradleの中で定義してください。

@<br>{}
//list[androidExtensions][viewholderでviewのキャッシュを有効化]{
  androidExtensions {
    experimental = true
  }
//}
@<br>{}

DataBindingを使用する場合と、使用しない場合によって異なるため分けて説明していきます。　　
@<br>{}
@<br>{}

=== DataBindingを使用しない場合
この節では、DataBindingを使用しない場合について話していきます。
DataBindingを使用しない場合は、次の2行をappのgradleの中に記述を加えてください。

@<br>{}
//list[no_databinding_in_groupie][DataBindingを使用しない場合appのgradleファイルに追加する]{
  compile 'com.xwray:groupie:2.0.3'

  compile 'com.xwray:groupie-kotlin-android-extensions:2.0.3'
//}
@<br>{}

DataBindingを使用しない場合はこれで完了です。
@<br>{}
@<br>{}

=== DataBindingを使用する場合
ここでは、DataBindingを使用しない場合がすでに済んでいる前提で話を進めます。
@<br>{}
