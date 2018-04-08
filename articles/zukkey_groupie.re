= Groupie実践入門
== はじめに
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

簡単に訳すと、複雑なRecyclerViewのレイアウトを簡単にして、ヘッダーやフッターなどをセクションと考えて論理的なグループという単位でコンテンツを扱えるようになるということです。実際にGroupieを導入してサンプルを作成していきましょう。

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
この節では、DataBindingを@<em>{使用しない}場合について話していきます。@<br>{}
DataBindingを使用しない場合は、次の2行をappのgradleの中に記述を加えてください。

//emlist[][]{
  compile 'com.xwray:groupie:2.0.3'

  compile 'com.xwray:groupie-kotlin-android-extensions:2.0.3'
//}

DataBindingを使用しない場合はこれで完了です。

==== DataBindingを使用する場合
この節では、DataBindingを@<em>{使用する}場合について話していきます。@<br>{}
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

== Groupieを用いてサンプルを作成してみよう
今回は、簡単なHeaderの付いたRecyclerViewを実装していきます。

サンプルコードのリポジトリは@<href>{https://github.com/yutaro6547/GroupieSampleForTechBook,こちら}（https://github.com/yutaro6547/GroupieSampleForTechBook）になります。

実際に作成したサンプルは次のとおりです。

//image[Sample][Header付きRecyclerViewのサンプル]{
    画像
//}

今回作成していくサンプルは、@<em>{DataBindingを使用した}物になります。

=== Header付きRecyclerViewをGroupieを用いて作成する
次に、Groupieを使用して先ほど作成したHeader付きのRecyclerViewと同じものを作成します。

その前にGroupとItemに付いての説明をしていきます。


Groupieでは、通常RecyclerView.Adapterに追加していくところをGroupAdapterにRecyclerViewを追加していきます。


GroupieにおけるGroupはビルドする単位のブロックで、Itemに当たるのがAdapterが拡張してリサイクルするユニットになります。


GroupとItemはAdapterに適宜交換可能で任意で追加することができます。


それでは、まずはItemクラスを作成していきます。

==== HeaderItemクラスを作成する
RecyclerViewにタイトルとサブタイトルのついたRecyclerViewを用意するためのHeaderのレイアウトを作っていきます。

レイアウトのソースコードは次のURLを参照してください。


参照先URL: @<href>{https://github.com/yutaro6547/GroupieSampleForTechBook/blob/master/app/src/main/res/layout/item_header.xml, GroupieSampleForTechBook/item_header.xml}

DataBindingを利用しているので<layout>タグで囲ってください。



次に、このレイアウトを利用したHeaderItemClassを作成していきます。



参照先URL: @<href>{https://github.com/yutaro6547/GroupieSampleForTechBook/blob/master/app/src/main/java/com/example/suzukihiroshitarou/groupiesamplefortechbook/HeaderItem.kt, GroupieSampleForTechBook/HeaderItem.kt}


DataBindingを利用している場合は、@<em>{BindableItemを継承した}クラスを作成してください。


DataBindingを利用しない場合は、Itemクラスを継承したクラスを作成する必要があります。


今回は、任意のタイトルとサブタイトルをコードで指定できるようにHeaderItemClassを作りました。


次に中身のRecyclerViewのItemクラスを作成していきます。

==== ContentItemクラスを作成する
Headerの下につけるRecyclerViewのItemクラスを作成していきます。
レイアウトのソースコードは次のURLを参照してください。


参照先URL: @<href>{https://github.com/yutaro6547/GroupieSampleForTechBook/blob/master/app/src/main/res/layout/item_content.xml, GroupieSampleForTechBook/item_content.xml}



今回は、あらかじめアプリ内のdrawableに用意している画像を使って、ImageViewに表示したものをRecyclerViewに表示するようにしています。


角丸のImageViewを利用したかったので今回は、@<href>{https://github.com/vinc3m1/RoundedImageView, RoundedImageView}というライブラリを利用しています。


次に、このレイアウトを利用したContentItemクラスを作成していきます。


参照先URL: @<href>{https://github.com/yutaro6547/GroupieSampleForTechBook/blob/master/app/src/main/java/com/example/suzukihiroshitarou/groupiesamplefortechbook/ContentItem.kt, GroupieSampleForTechBook/ContentItem.kt}


これで、RecyclerViewに使用するItemクラスの作成は完了です。

==== Groupクラスを用意する
今回は、@<href>{https://github.com/lisawray/groupie/blob/master/example/src/main/java/com/xwray/groupie/example/ColumnGroup.kt,公式のサンプル}を利用します。


ここまできたら、最後にActivityに追加していきましょう。

==== Activityで全てのItemをgroupAdapterに追加していく
全体のソースコードは、@<href>{https://github.com/yutaro6547/GroupieSampleForTechBook/blob/master/app/src/main/java/com/example/suzukihiroshitarou/groupiesamplefortechbook/MainActivity.kt, こちら}（https://github.com/yutaro6547/GroupieSampleForTechBook/blob/master/app/src/main/java/com/example/suzukihiroshitarou/groupiesamplefortechbook/MainActivity.kt）にあります。


まずは、Groupieの実装に必要なGroupAdapterを定義します。
今回はカラムが３つのグリッドリストを作成するので次のようにしています。

//emlist[][]{
  val groupAdapter = GroupAdapter<ViewHolder>().apply {
    spanCount = 3
}
//}

次に、GroupieではコンテンツのリストをSectionという単位でまとめます。
ここで、先ほどまでで作成していたHeaderItemとContentItemをSection単位でまとめて、GroupAdapterに追加していきます。
実際のコードは次のとおりです。

//emlist[][]{
  Section(HeaderItem(R.string.test_title, R.string.test_sub_title)).apply {
    add(ColumnGroup(contentItem1))
    add(ColumnGroup(contentItem2))
    groupAdapter.add(this)
}
//}
cententItem1とcontentItem2は、全体のソースコードを見ると分かるのですが、すでにあらかじめ用意してある画像のarrayから読みだしてリストにしたものをColumnGroupに入れるようにしています。

次に、ActivityのRecyclerViewにgroupAdapterを入れていきます。
実際のコードは次のとおりです。

//emlist[][]{
  binding.recyclerView.apply {
    layoutManager = GridLayoutManager(context, groupAdapter.spanCount).apply {
        spanSizeLookup = groupAdapter.spanSizeLookup
    }
    addItemDecoration(ColumnItemDecoration(MARGIN))
    adapter = groupAdapter
}
//}
ItemDecorationで適宜マージンの調整を行なっています。
これで、Groupieを使用したサンプルを作成することができました。

== 私が考えるGroupieを使うことのメリットとデメリット
=== Groupieを使うことのメリット

公式のREADMEにも書かれていますが、Groupieでは、グループの内容を変更することで自動的に親に通知が届き、通知がGroupAdapterに到達すると変更通知が送られるというメリットがあります。



これは、Sectionの中のupdate関数を追っていくとよく分かります。公式のコードは次のとおりです。

//emlist[][]{
  final DiffUtil.DiffResult diffResult = DiffUtil.calculateDiff(new DiffUtil.Callback() {
          @Override
          public int getOldListSize() {
              return oldBodyItemCount;
          }

          @Override
          public int getNewListSize() {
              return newBodyItemCount;
          }

          @Override
          public boolean areItemsTheSame(int oldItemPosition, int newItemPosition) {
              Item oldItem = getItem(oldBodyGroups, oldItemPosition);
              Item newItem = getItem(newBodyGroups, newItemPosition);
              return newItem.isSameAs(oldItem);
          }

          @Override
          public boolean areContentsTheSame(int oldItemPosition, int newItemPosition) {
              Item oldItem = getItem(oldBodyGroups, oldItemPosition);
              Item newItem = getItem(newBodyGroups, newItemPosition);
              return newItem.equals(oldItem);
          }

      @Nullable
      @Override
      public Object getChangePayload(int oldItemPosition, int newItemPosition) {
          Item oldItem = getItem(oldBodyGroups, oldItemPosition);
          Item newItem = getItem(newBodyGroups, newItemPosition);
          return oldItem.getChangePayload(newItem);
      }
  });

  super.removeAll(children);
　children.clear();
　children.addAll(newBodyGroups);
　super.addAll(newBodyGroups);

　diffResult.dispatchUpdatesTo(listUpdateCallback);
　if (newBodyItemCount == 0 || oldBodyItemCount == 0) {
    refreshEmptyState();
}
//}

Group毎にItemとその中身のContentの差分を計算してDiffUtilクラスのdispatchUpdatesToにlistUpdateCallbackを投げています。
次に、ListUpdateCallbackを見ていきましょう。公式は次のとおりです。

//emlist[][]{
  private ListUpdateCallback listUpdateCallback = new ListUpdateCallback() {
      @Override
      public void onInserted(int position, int count) {
          notifyItemRangeInserted(getHeaderItemCount() + position, count);
      }

      @Override
      public void onRemoved(int position, int count) {
          notifyItemRangeRemoved(getHeaderItemCount() + position, count);
      }

      @Override
      public void onMoved(int fromPosition, int toPosition) {
          final int headerItemCount = getHeaderItemCount();
          notifyItemMoved(headerItemCount + fromPosition, headerItemCount + toPosition);
      }

      @Override
      public void onChanged(int position, int count, Object payload) {
          notifyItemRangeChanged(getHeaderItemCount() + position, count, payload);
      }
  };
//}

内部的にpositionで比較してくれているので、どんなデータを構成していても手動でindexをよしなにやるという手間を無くしupdate関数を渡すだけで変更があれば差分を変更通知してくれるというメリットが分かります。



また、GroupieではGroupに必要な機能を自分で実装できるので柔軟にカスタマイズ可能です。
その際に、SectionまたはNestedGroupを拡張する必要があります。継承することで自由に自分好みのGroupを定義できます。



セクションは、diffing、ヘッダー、フッター、プレースホルダーなどの一般的なRecyclerViewの考え方をサポートし、NestedGroupは、グループの任意のネスト、リスナーの登録/登録解除、
アニメーションをサポートするための細かい変更通知、およびアダプターの更新をサポートしています。



他にもifまたはwhenの分岐がなくなることによる可読性と拡張性、DataBindingとも相性が良くモデルとViewを簡単に結びつけることが可能になります。



そして、2018年3月現在も開発がストップしておりません。

=== Groupieを使うことのデメリット
Groupieを使うことのデメリットとしてあげられるのは、すでに確立されている既存のアプリに導入していくのは難しいかもしれません。


また、Groupの階層を重ねるとスクロールした際にカクツクことがあります。パフォーマンス面ではまだ改善点があるという懸念点があります。


さらに、似たライブラリとしてEpoxyがあり、こちらも開発がストップしておらず勢いも衰えていません。Groupieが押され気味であるのでもしかしたら今後開発がストップする可能性もなくはないという点が懸念されます。

== おわりに
今回は、Groupieの導入からサンプル作成と私が考えるGroupieのメリットとデメリットについての紹介でした。


今回作成したサンプルは次のリポジトリで見ることができます。


@<href>{https://github.com/yutaro6547/GroupieSampleForTechBook, yutaro6547/GroupieSampleForTechBook}

今回Groupieを触ってみて、総じて使いやすく便利なライブラリであると思いますが、Epoxyもあるので今後どちらが主流になっていくのか気になります。

また、もっと深いことも今後はやっていき、Epoxyとの違いについても今後探っていきたいなと考えています。


@<href>{http://rozkey.hatenablog.com/, "zukkeyの技術奮闘記（http://rozkey.hatenablog.com/）"}という個人ブログもやっておりますので、よかったら見にきてください。
