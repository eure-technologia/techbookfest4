
= 第二言語としてのClojure

こんにちは！株式会社エウレカでフロントエンドエンジニアをしている@<tt>{BOXP（@If_I_were_boxp）}です。
最近はスキーにどっぷりとハマっており、少し休みが取れるやいなやスキー場へと直行する日々を過ごしています。
また、スキーが出来ない夏も充実した日々を過ごすべく、サバゲーの装備を着々と増やしていく毎日です:)

== はじめに

この章は、フロントエンド・サーバーサイド・ネイティブアプリケーション等の多種多様な場面で活躍している関数型言語、Clojureへの入門から実践までをわずか9ページで説明しようとするものです。

前提知識が無くても困らないよう配慮して書いていますが、次の項目に当てはまっているとより理解しやすい内容になっているかもしれません。

	* ひとつ以上のプログラミングを使っての開発経験がある
	* フロントエンド・サーバーサイド・ネイティブアプリケーションいずれかでの開発経験がある
	* コマンドラインでの操作に慣れている

でははじめに、Clojureについてざっくりと説明致します。

== Why Clojure?

ClojureはJVM上・Webブラウザ上・.NET上で動作するマルチパラダイム言語です。
このような言語の存在は今日ではそこまで珍しいものではなく、それぞれのプラットフォームに根付いた言語はもちろんのこと数多くの選択肢が存在します。
しかし、Clojureはその他数多くの言語に一線を成すユニークな特徴を多く持ち、どのプラットフォームでも一貫して次の恩恵が得られます。

=== 高機能なREPLによる高速なアプリケーション開発

REPL（Read Eval Print Loop）は対話型開発環境とも呼ばれるもので、書いたコードを即評価して確認することができるものです。
これそのものはさまざまな言語で利用されていますが、Clojureやその仲間であるLisp系の言語達は一風変わったREPLを持っており、
その力を最大限に活かした開発を行うことができます。

Clojureでのアプリケーション開発サイクル標準的な開発サイクルは次のようなものです。

#@# TODO: 画像がフォルダに入ってない

//image[clojure_repl][clojure_repl]{
REPLを使ったClojureの開発サイクル
//}

	1. REPLを起動し、REPL上でアプリケーションを動作させる（フロントエンドの場合はブラウザに接続する）
	2. 書いたコードをエディタやIDEから直接REPLへ送り評価して確認する
	3. 2番に戻る

エディタやIDEから直接コードを評価し確認することで、高速な開発サイクルを実現できます:)

他にもさまざまな特徴がありますが、書きたいことを全部書くとこの章が終わってしまうのでここまでにします。
その他の点については、Clojureの公式サイト（https://clojure.org/about/rationale）の「Why Clojure?」によくまとまっているので是非読んでみてください。

次はいよいよ実際にClojureを書いていきましょう！

== Clojureの世界へようこそ！

この節では、Clojureの入門から基本的な部分までを手を動かしながらさくっと説明していきます。

=== 全ての道はLeiningenから

Clojureは、コードのビルド・ライブラリの依存管理・REPLの立ち上げ等のコーディングに必要な周辺環境を「Leiningen」というツールを用いて行います。
まずはLeiningenをインストールする所からはじめて行きましょう。

Leiningenはhomebrewなどのパッケージマネージャから簡単にインストールできます。

//emlist[][]{
homebrew install leiningen
//}

Linux環境の場合、パッケージマネージャを使ってインストールすると古いバージョンがインストールされてしまう可能性があるため、次のコマンドでインストールすることをお勧めします。

//emlist[][]{
# Leiningenをダウンロード
$ wget https://raw.githubusercontent.com/technomancy/leiningen/stable/bin/lein

# パスの通ったフォルダへ移動する
$ mv lein ~/bin

# 実行権限を付与
$ chmod u+x ~/bin/lein
//}

インストール後は@<tt>{lein}コマンドによって呼び出します。
REPLを立ち上げる@<tt>{lein repl}を使って、Clojureのコードを試し打ちしてみましょう。

//emlist[][]{
# REPLを立ち上げる
$ lein repl

;; Clojureのコードを試し打ち
user=> (+ 1 2)
//}

結果として@<tt>{3}が返ってくることが確認てきたでしょうか。
ひとまず、立ち上げたREPLは@<tt>{Ctrl-D}を入力することで終了できるのでいったｎ終了させておきましょう。
続いて、REPLの次にLeiningenで頻繁に使う機能であるプロジェクト管理機能を使います。

=== はじめてのプロジェクト

Leiningenには、clojars（https://clojars.org/）に公開されているボイラープレートを使ってプロジェクトを作る機能が標準搭載されています。
これを使うことでWebサーバーやReactを使ったSPA（Single Page Application）のプロジェクトをノータイムで作ることができます。
しかしここでは、簡単の為にもっともシンプルなLeiningenに標準搭載されているボイラープレートから使って行きましょう。

ボイラープレートを使ってプロジェクトを作るには、次のように@<tt>{lein}コマンドを呼び出します。

//emlist[][]{
lein new techbookfest
//}

この結果、下のようなプロジェクトができあがったと思います。

//emlist[][]{
techbookfest
├── CHANGELOG.md <- 仮置きの変更履歴を書いておくファイル
├── doc
│   └── intro.md <- 仮置きのドキュメント
├── LICENSE <- ソースコードのライセンス表示
├── project.clj <- プロジェクトの設定ファイル
├── README.md <- README
├── resources <- 静的ファイルを配置するフォルダ
├── src
│   └── techbookfest
│       └── core.clj <- ソースコード
└── test
    └── techbookfest
        └── core_test.clj <- テストコード
//}

この状態でプロジェクトのフォルダに入ってREPLを起動すると、
@<tt>{src}フォルダに置かれたソースコードが全て読み込まれた上で起動することができます。
次のコマンドで、再度REPLを起動してみましょう。

//emlist[][]{
$ cd techbookfest
$ lein repl
user=>
//}

これで簡単なアプリケーションをREPLで開発して行く準備ができました！
続いて、このREPL環境を使ってClojureの基本をざっと説明していきます。

====[column] Tips

ClojureのREPLは、コマンドライン上から叩く以外にもエディタやIDEから接続して使用する手段も用意されています。尺の都合上ここでは解説しませんが、本格的に開発をする時は是非そちらを使用してください。

	* Vimを使う場合: @<tt>{vim-fireplace（https://github.com/tpope/vim-fireplace）}
	* Emacsを使う場合: @<tt>{cider（https://github.com/clojure-emacs/cider）}
	* IntelliJを使う場合: @<tt>{Cursive（https://cursive-ide.com/userguide/）}

また、特にこだわりがない場合はClojureに必要な設定が詰まったEmacsであるSpacemacs（https://github.com/syl20bnr/spacemacs）を使うと快適です。

====[/column]

=== Clojureの基本

==== Clojureの文法

Clojureの文法はシンプルで、括弧の中に命令→引数の順番に書いていくものです。
次のように@<tt>{user=>}の後に続くコードを入力することで、書いたとおりの出力結果が得られることを確認して行きましょう。

//emlist[][]{
;; Clojureにおいてはこの ;; から始まる行がコメントになります

;; 文法の基本は(命令 引数1 引数2 ...引数N)
user=> (println "hello, world!")
hello, world!
nil

;; 当然入れ子にもできる
user=> (+ 1 2 (+ 1 2))
6

;; docを使うと命令の使い方を確認できます
user=> (doc println)
-------------------------
clojure.core/println
([& more])
  Same as print followed by (newline)
nil
//}

==== ローカル変数/グローバル変数

Clojureでのローカル変数/グローバル変数の定義には、それぞれ@<tt>{let},@<tt>{def}を使います。
注意すべき点として、Clojureにおける変数は必ずImmutable（不変）で再代入が不可能になっていて、RubyやPythonのような一般的な変数と同じことをする場合はatomを用いる必要があります。

//emlist[][]{
;; グローバル変数はdefを使います
user=> (def hoge "hoge")
#'user/hoge

user=> hoge
"hoge"

;; ローカル変数はletを使います
;; (let ...)の中でだけ変数を参照可能です
user=> (let [fuga "fuga"]
  #_=> fuga)
"fuga"

;; 変数に束縛する値をatomでくるむことで、一般的な変数と同じことができます
user=> (def ahoge (atom "hoge"))
#'user/ahoge

;; reset!でatomがもつ値を変更
user=> (reset! ahoge "hogehoge")
"hogehoge"

;; @を付けて呼び出すことでatomがもつ値を取り出す
user=> @ahoge
"hogehoge"
//}

==== 関数

Clojureにおける関数はRubyやPython等の他の言語と同じく第一級オブジェクトです。
またREPLを使って確認してみましょう。

//emlist[][]{
;; 関数
;; Clojureの関数はfnで記述できます
user=> (fn [n] (+ 1 n))
object[user$eval1282$fn__1283 0x5c0695e4 "user$eval1282$fn__1283@5c0695e4"]

user=> ((fn [n] (+ 1 n)) 1)
2

;; 次の形の省略記法も用意されています
user=> (#(+ 1 %) 1)
2

;; %が引数を表し、複数の引数がある時は%1, %2, %3...と増えていきます
user=> (#(+ %1 %2 %3 %4) 1 2 3 4)
10

;; defn, letfnで変数定義を省略できます
user=> (defn plus [n] (+ 1 n))
#'user/plus

user=> (plus 1)
2

user=> (letfn [(plus [n] (+ 1 n))]
  #_=> (plus 1))
2
//}

==== リスト（List)/集合（Sets）/ベクター（Vector）/ハッシュマップ（HashMap）

Clojureにおけるリスト（List）・ベクター（Vector）・集合（Sets）・ハッシュマップ（HashMap）は@<tt>{シーケンス（Sequences）}と呼ばれる共通のインターフェースを持っていて、次の共通項を持っています。

	* お互いへの相互変換が可能である
	* 共通して使用できる命令をもつ（map, reduce, forなど）

例によってREPLで実行しながらどんなものか見ていきましょう。

//emlist[][]{
;; リスト(List)
;; 他の言語で連結リストやコンスセルと呼ばれたりするものです
user=> (cons 1 (cons 2 (cons 3 nil)))
(1 2 3)

;; ↑はこのようにも書けます
user=> '(1 2 3 4)
(1 2 3 4)

user=> (list 1 2 3 4)
(1 2 3 4)

;; ベクター(Vector)
;; 他の言語で配列と呼ばれるそれです
user=> [1 2 3 4]
[1 2 3 4]

;; インデックスを使った高速な要素の呼び出しが必要な時などに使います
user=> (nth [1 2 3 4] 2)
3

;; 他のシーケンスからの変換には@<tt>{vec}を使います
user=> (vec '(1 2 3 4))
[1 2 3 4]

;; letを使って要素をまとめて束縛することができます
user=> (let [[hoge fuga] [1 2]]
  #_=>   (+ hoge fuga))
3

;; 集合(Sets)
;; 順番を考慮せず、重複しない要素をもつシーケンスです
user=> #{1 2 3 4}
#{1 4 3 2}

;; 関数として呼び出すと、引数の値が集合に含まれているかどうか判定できます
user=> (#{1 2 3 4} 3)
3

;; 含まれていない場合
user=> (#{1 2 3 4} 42)
nil

;; 他のシーケンスからの変換には@<tt>{set}を使います
user=> (set '(1 2 3 4))
#{1 4 3 2}

;; ハッシュマップ(HashMap)
;; 他の言語でハッシュ・連想配列・辞書型などとよばれるそれです
user=> {1 2 3 4}
{1 2, 3 4}

;; 関数として呼び出すと、引数に対応する値を返します
user=> ({1 2 3 4} 1)
2

;; ハッシュマップのキーには、Keywordと呼ばれるものがよく使われます
user=> {:hoge 1 :fuga 2}
{:hoge 1, :fuga 2}

;; ハッシュマップの更新にはassoc, updateが良く用いられます
;; assocはキーに対応する値を最後の引数へ置き換えます
user=> (assoc {:hoge 1} :fuga 2)
{:hoge 1, :fuga 2}

;; updateはキーに対応する値へ関数を適用します
user=> (update {:hoge 1} :hoge #(+ 100 %))
{:hoge 101}

;; シーケンスを組み合わせてみる
user=> (filter #{3 5 8 10} (map #(+ 2 %) [1 2 4 5 8]))
(3 10)

;; シーケンスを組み合わせたコードを書く時は、->, ->>, some->, some->>等のThreading Macroが便利
user=> (->> (map #(+ 2 %) [1 2 4 5 8])
  #_=> 	    (filter #{3 5 8 10})
  #_=>      (reduce +))
13
//}

紹介しきれていない部分もありますが、最低限Clojureを書き始めるのに必要な説明をこれで終えられました。
後はClojureの命令が良くまとまったチートシート（https://clojure.org/api/cheatsheet）が用意されているので、このチートシートを傍らに開きながらコーディングすることを勧めます。

最後に、Clojureを使ったサーバーサイド・フロントエンド・ネイティブアプリケーションを作る入り口となるよう、３つのフレームワークを紹介します。

== Clojureと3つのフレームワーク

=== サーバーサイドアプリケーションを作るフレームワーク「duct」

「duct」は、Clojureを使ったサーバーサイドアプリケーション開発でもっとも人気のあるフレームワークのひとつです。

REPLを使った効率的な開発が可能な他、ボイラープレートがherokuへのデプロイに必要な設定ファイルや静的ウェブページを返すエンドポイントを自動生成してくれる等など大変充実しています。

開発を始めるのなら、ぜひQuickStart(https://github.com/duct-framework/duct#quick-start)を参考に始めるといいでしょう。

=== フロントエンドアプリケーションを作るフレームワーク「re-frame」

「re-frame」はSPA（Single Page Application）を作るClojure向けのフレームワークの中でもっとも人気のあるもののうちのひとつです。

React.jsへの薄いラッパーライブラリのReagentにReduxのようなワークフローを加えたフレームワークですが、著者の経験上他のフレームワークよりも比較的シンプルで扱いやすいです。

もちろんボイラープレートも提供されていて、開発の始め方についてはre-frameのREADME（https://github.com/Day8/re-frame）を読むといいでしょう。

=== ネイティブアプリケーションを作るフレームワーク「re-natal」

re-natalは、ReactNative上でClojureを向けのフレームワークを使ったフロントエンドアプリケーションを動作させるためのツールキットです。

↑で紹介していたre-frameやその他のフレームワークからひとつを選んで開発することができるボイラープレートも提供されています。

現状ではClojureを使ったネイティブアプリケーション開発はこのre-natal一強の状態なので、もしClojureを使ったネイティブアプリケーション開発に興味があればre-natalのREADME（https://github.com/drapanjanas/re-natal）を読んでみてください。

== おわりに

この記事では、第二言語としてClojureに触れる人向けの入門的な内容を書かせていただきました。
書きたいことがありすぎてスコープを広げた結果少し端折り過ぎた部分もあるかと思うのですが、少しでも興味を持ってもらえたなら幸いです:)

== 参考文献

	* https://clojure.org/about/rationale
	* https://www.ohmsha.co.jp/book/9784274067891/
