
= Web素人のHaskell+Elmひとりたび


Webとモバイルアプリだったらどちらかというとモバイルアプリのほうがよく書いているなーという筆者が、ちょろっとElmに入門してみました。
この記事では、Elmの特徴的な部分であるThe Elm Architectureについて解説します。




今回は、サーバへHTTPリクエストを送り、帰ってきた値を適切なViewに当てはめて表示する簡単なWebページを作ってみたいと思います。初期状態から始まり、リクエストを送信し、帰ってきた値をViewにレンダリングするところまでの一通りの動作が実装できます。


基本的に、サーバサイドとフロントエンドはJSONでやり取りすることによって完結するように作ります。

==  The Elm Architecture
ElmはもともとFRP(Functional Reactive Programming)と呼ばれるパラダイムで記述するプログラミング言語でしたが、現在はよりシンプルなElm Architectureという設計パターンに沿って開発していく言語になっています。

Elm Architectureは大きくModelとUpdateとViewの3パーツから成り立っています。以下でそれぞれについて説明します。


==={Model-1} Model

Modelは、アプリケーションの持つ状態です。アプリケーションはこのModelの内容が更新されることによって動作します。状態はここに集約されています。



=== Update



Updateは、アプリケーションの状態を更新します。送られてきたメッセージに応じて、現在のModelの内容を元に新しいModelに差し替えます。



=== View



Viewは、Modelの内容をレンダリングします。これにより、Modelの内容がユーザに見えるようになります。


Elm Architectureは大雑把にはこの3つがループして動作するような形になっています。初期状態のModelがあるところから、UpdateによりModelが更新され、新しいModelの内容がViewによってレンダリングされるという形です。



=== MessageとCommand

この3つのパーツの間をやり取りするためにあるのがMessageとCommandです。

MessageはUpdateで処理されるアクションを表すデータ型で、そこに定義されたアクションが起こったときにMessageが発行されUpdateで処理されます。
例えば、ボタンのクリック、APIへのリクエストへのレスポンスが帰ってきた、等です。
Commandは外部とのやり取りを表します。APIへのリクエストを送るときにMessageをラップし、外部とのやり取りが完了するとラップされているMessageがUpdateに渡ります。

これを図にしてみると以下のようになります。

//image[mvu.001][Model-View-Update]


== 実装してみよう


ここまでに説明したような仕組みによってElmで書かれたアプリケーションは動作しますが、実際のコードを見ながら追いかけてみましょう。


=== データ型の定義



アプリケーション内で用いるデータ型を定義していきましょう。今回は主にModelとMessageを定義します。



==== Model

まず、アプリケーションの状態であるModelを定義していきます。今回は以下の２つの状態を持つアプリケーションとします。

* 取得されたページの内容を表示している状態

* ページの読み込み中の状態


//emlist[][]{
type Model = MainPage Page | LoadingPage
//}



==== Message

今回は何もしないケースとページの読み込みが完了したケースの２通りがあるので、そのように定義しました。
//emlist[][]{
type Msg = NoOp | LoadMainPage (Result Http.Error Page)
//}


今回は単純に何もしないイベントと読み込みが完了した際のイベントを定義しました。



==={Update-1} Update


@<tt>{update}は前述のMessageを受けてモデルの状態を更新し、同時に新たなCommandを発行する関数です。例えば、上記の @<tt>{LoadMainPage (Result Http.Error Page)} を受けた場合に、その結果の@<tt>{Page}を使ってModelを@<tt>{MainPage Page} にし、読み込みが完了したため何もしない、という処理を書いています。


//emlist[][]{
update : Msg -> Model -> (Model, Cmd Msg)
update message model = case message of
    LoadMainPage (Ok result) -> (MainPage result, Cmd.none)
    _ -> (initialModel, Cmd.none)
}


上記のupdateの処理によりModelが更新され、その更新がViewによってレンダリングされることにより、ユーザの目にAPIから取得してきたPageの内容が表示されました。

==={View-1} View

//emlist[][]{
view : Model -> Html Msg
view model = case model of
  MainPage page -> mainPage page
  LoadingPage -> loadingPage
}

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
                           img [ src "https://storage.googleapis.com/mstdn-haru2036/accounts/avatars/000/000/001/original/baa74aa0b028b0a7.jpg"
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

//}
=== main
これでModel. Update, Viewが揃いました。
実際に実行する際はどのようにして実行されているのでしょうか。

elmも他の多くのプログラミング言語と同じく、main関数から始まります。今回のアプリケーションではこのような内容になっています。


//emlist[][]{
main : Program Never Model Msg
main =
     Html.program
          { init = init
          , view = view >> toUnstyled
          , update = update
          , subscriptions = subscriptions
          }
//}


Html.programにはinit, view, update, subscriptionsという4つの引数が渡されています。１つずつ見ていきましょう。@<tt>{init}は、初期状態を表すModelです。

== 参考文献
https://matsubara0507.github.io/posts/2017-12-13-elm-and-haskell-for-elmer.htmlhttps://guide.elm-lang.org/architecture/


== 思ったこと

 * hpackなるものを知らなかった、これ便利そう
 * Update、実のところはModelを更新じゃなくて現在のModelを元に新しいModelを作るなんだけど、その説明いるかな
