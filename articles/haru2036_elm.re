= ElmでWebフロント入門
こんにちは。普段はAndroidアプリを書いている@haru2036です。趣味ではHaskellとか触って遊んでたりもします。
Webフロントをあんまりやったことがないことに気が付き、ちょっと変わったチョイスですがElmを触ってみることにしました。

この記事では、Elmの特徴的な部分であるThe Elm Architectureについて解説します。
今回は、サーバへHTTPリクエストを送り、返ってきた値を適切なViewに当てはめて表示する簡単なWebページを作ってみたいと思います。
初期状態から始まり、リクエストを送信し、帰ってきた値をViewにレンダリングするところまでのひととおりの動作を実装してみました。

基本的に、サーバサイドとフロントエンドはJSONでやり取りすることによって完結するようにしました。

== The Elm Architecture
ElmはもともとFRP（Functional Reactive Programming）と呼ばれるパラダイムで記述するプログラミング言語でしたが、現在はよりシンプルなElm Architectureという設計パターンに沿って開発していく言語になっています。
Elm Architectureは大きくModelとUpdateとViewの3パーツから成り立っています。以下でそれぞれについて説明します。

==={Model-1} Model
Modelは、アプリケーションのもつ状態です。アプリケーションはこのModelの内容が更新されることによって動作します。状態はここに集約されています。

=== Update
Updateは、アプリケーションの状態を更新します。送られてきたメッセージに応じて、現在のModelの内容を元に新しいModelに差し替えます。
Modelの更新はここで行われます。

=== View
Viewは、Modelの内容をレンダリングします。これにより、Modelの内容がユーザに見えるようになります。
Elm Architectureは大雑把にはこの3つがループして動作するような形になっています。初期状態から始まり、UpdateによりModelが更新されて、新しいModelの内容がViewによってレンダリングされるという形です。

=== MessageとCommand
この3つのパーツの間をやり取りするためにあるのがMessageとCommandです。
MessageはUpdateで処理されるアクションを表すデータ型で、そこに定義されたアクションが起こったときにMessageが発行されUpdateで処理されます。
たとえば、ボタンのクリック、APIへのリクエストのレスポンスが帰ってきた、等です。

Commandは外部とのやり取りを表します。APIへのリクエストを送るときにMessageをラップし、外部とのやり取りが完了するとラップされているMessageがUpdateに渡ります。

これを図にしてみると次のようになります。

//image[mvu][Model-View-Update]{
    画像
//}

このように、Model, View, Updateが順番に呼び出されループするような構成になっています。

== 実装してみよう
ここまでに説明したような仕組みによってElmで書かれたアプリケーションは動作しますが、実際のコードを見ながら追いかけてみましょう。

=== データ型の定義
アプリケーション内で用いるデータ型を定義していきましょう。今回は主にModelとMessageを定義します。

==== Model
まず、アプリケーションの状態であるModelを定義していきます。今回は次の２つの状態をもつアプリケーションとします。

 * 取得されたページの内容を表示している状態
 * ページの読み込み中の状態

//emlist[][]{
type Model = MainPage Page | LoadingPage
//}

==== Message
今回は何もしないケースとページの読み込みが完了したケースの２とおりがあるので、そのように定義しました。
//emlist[][]{
type Msg = NoOp | LoadMainPage （Result Http.Error Page）
//}

今回は次のような２つのメッセージを定義しました。

 * 何もしないということを表すメッセージ
 * 読み込みが完了したというイベントを表すメッセージ

==={Update-1} Update
@<tt>{update}は前述のMessageを受けてモデルの状態を更新し、同時に新たなCommandを発行する関数です。
たとえば、上記の @<tt>{LoadMainPage （Result Http.Error Page）} を受けた場合に、その結果の@<tt>{Page}を使ってModelを@<tt>{MainPage Page} にし、読み込みが完了したため何もしない、という処理を書いています。


//emlist[][]{
update : Msg -> Model -> (Model, Cmd Msg)
update message model = case message of
    LoadMainPage (Ok result) -> (MainPage result, Cmd.none)
    _ -> (initialModel, Cmd.none)
//}

上記のupdateの処理によりModelが更新され、その更新がViewによってレンダリングされることにより、ユーザの目にAPIから取得してきたPageの内容が表示されました。

==={View-1} View
//emlist[][]{
view : Model -> Html Msg
view model = case model of
  MainPage page -> mainPage page
  LoadingPage -> loadingPage
//}

今回は、ページが表示された状態とページが読み込み中の状態それぞれに別の画面を表示したかったのでそれを切り分けるようにしています。

実際のViewはこのようにHTMLに変換できる形式で記述します。
//emlist[][]{
mainPage : Page -> Html Msg
mainPage page = pageWrapper <| div [ class "main-card"
                       , css
                         [ display inlineBlock
                         , backgroundColor (rgb 250 250 250)
                         , width (vw 70)
                         , height (pct 40)
                         , top (pct 0)
                         , bottom (px 0)
                         , right (pct 50)
                         , left (pct 0)
                         , margin4 auto auto auto auto
                         , position absolute
                         , boxShadow5 (px 0) (px 3) (px 5) (px 0) (rgba 0 0 0 0.5)
                         , borderRadius (px 3)
                         , overflow hidden
                         ]
                       ]

                       [ div
                         [ css
                           [
                             height (pct 50)
                           , width(vh 40)
                           ]
                         ]
                         [
                           img [ src "..."
                         , css [
                                 borderRadius (pct 50)
                               , height (pct 100)
                               ]
                             ] []
                         ]
                       , div [
                         css [position relative
                             , width (pct 100)
                             , height (pct 100)
                             ]
                         ] [
                           h1 [] [ text page.pageTitle ]
                         , section
                           [ class "" ]
                           [ text page.pageBody ]
                       ]
                     ]

pageWrapper : Html Msg -> Html Msg
pageWrapper body = div [ class "main-wrapper"
                 , css [ position relative
                       , width (pct 100)
                       , height (pct 100)
                       , backgroundImage (url "...")
                       , display block
                       , backgroundSize cover
                       , backgroundRepeat noRepeat
                       , backgroundPosition center
                       ]
                 ]
                 [ body ]
//}

このコードは少し長いですが、HTMLとCSSの要素と、そのViewに挿入するModelの中の値をひとまとめに書くことができることがわかります。

これらはelm-html(https://github.com/elm-lang/html)とelm-css(https://github.com/rtfeldman/elm-css)というライブラリを用いる形で記述しています。

@<tt>{div}や@<tt>{img}を始めとしたHTMLの要素を関数として、その属性や子要素を引数として記述していますが、通常のHTMLと同じように読むことも可能な見た目になっているため簡単に記述できます。
また、@<tt>{pageWrapper}のように引数に他のViewを取ってViewを組み立てるということも可能です。


このようにしてModelとView、Updateを定義することによってThe Elm Architectureのアプリケーションを記述することができました。あとは前述のライブラリが提供している@<tt>{Html.program}にこれらの処理と初期状態のモデル等をわたし、それをmain関数とすることによって実際にアプリケーションが動くようになります。

== おわりに
この記事では、Elmアプリケーションの流れをひととおり追ってみました。この記事でElmに興味を持った方へは、詳しい情報を得るために公式サイト(http://elm-lang.org/)のサンプルコードを読んでみることをお勧めします。


Elmはelm-reactorやオンラインエディタ等、試しに使ってみる時へのサポートもしっかりしているのでまずは試しに触ってみるというのもお勧めです！

== 参考文献
 * https://guide.elm-lang.org/architecture/
 * https://matsubara0507.github.io/posts/2017-12-13-elm-and-haskell-for-elmer.htmlhttps://guide.elm-lang.org/architecture/