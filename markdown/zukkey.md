# はじめに
こんにちは、初めまして。
株式会社エウレカにてPairsJPのAndroidの開発を行っている[@zukkey](https://twitter.com/zukkey59)です。
今回は、Android開発で複雑なレイアウトを組むときのRecyclerViewをよしなにやってくれるライブラリのGroupieについてご紹介いたします。

今回話す内容は以下の通りです。

- Groupieとは
- Groupieの使い方
  - Groupieの導入
    - DataBindingを使用しない場合
    - DataBindingを使用する場合


# Groupieとは
[Groupie](https://github.com/lisawray/groupie)は、 Lisa Wrayさんが作成・提供しているライブラリでAndroid開発でのRecyclerViewの複雑なレイアウトを簡単にしてくれます。

公式のREADMEには以下のように書かれています。

>Groupie is a simple, flexible library for complex RecyclerView layouts.

>Groupie lets you treat your content as logical groups and handles change notifications for you -- think sections with headers and footers, expandable groups, blocks of vertical columns, and much more. It makes it easy to handle asynchronous content updates and insertions and user-driven content changes. At the item level, it abstracts the boilerplate of item view types, item layouts, viewholders, and span sizes.

簡単に訳すと、複雑なRecyclerViewのレイアウトを簡単にして、ヘッダーやフッターなどをセクションと考えて論理的なグループという単位でコンテンツを扱えるようになるということです。

# Groupieの使い方
## Groupieの導入
まずはじめに、Groupieを導入するためにいくつか設定する必要があります。
DataBindingを使用する場合と、使用しない場合によって異なるため分けて説明していきます。　　
### DataBindingを使用しない場合
この節では、DataBindingを使用しない場合について話していきます。
