
= Web素人のHaskell+Elmひとりたび



Haskell昔はちょっとだけ触ってて最近はKotlinばかり書いてる筆者が、WebサイトをHaskell+Elmで作ってみるおはなしです。



== 目次




 * 何を作ろう
 * The Elm Architecture


 * Model
 * Update
 * View
 * Message
 * Command

 * サーバサイド（Haskell）との連携について
 * 実装してみよう


 * Modelを定義する
 * 初期状態を定義する
 * Updateを用意する
 * Viewを書く
 * Messageを送る
 * Commandを送る




== 何を作ろう



今回は、サーバへHTTPリクエストを送り、帰ってきた値を適切なViewに当てはめて表示する簡単なWebページを作ってみたいと思います。初期状態から始まり、リクエストを送信し、帰ってきた値をViewにレンダリングするところまでの一通りの動作が実装できます。


基本的に、サーバサイドとフロントエンドはJSONでやり取りすることによって完結するように作ります。msgとcmdのはなし

==  The Elm Architecture
ElmはもともとFRP(Functional Reactive Programming)と呼ばれるパラダイムで記述するプログラミング言語でしたが、現在はよりシンプルなElm Architectureという設計パターンに沿って開発していく言語になっています。Elm Architectureは大きくModelとUpdateとViewの3パーツから成り立っています。以下でそれぞれについて説明します。



=== Model



Modelは、アプリケーションの持つ状態です。アプリケーションはこのModelの内容が更新されることによって動作します。（多分：状態はここに集約されます。）



=== Update



Updateは、アプリケーションの状態を更新します。送られてきたメッセージに応じて、現在のModelの内容を元に新しいModelに差し替えます。



=== View



Viewは、Modelの内容をレンダリングします。これにより、Modelの内容がユーザに見えるようになります。


Elm Architectureは大雑把にはこの3つがループして動作するような形になっています。初期状態のModelがあるところから、UpdateによりModelが更新され、新しいModelの内容がViewによってレンダリングされるという形です。



=== MessageとCommand



この3つのパーツの間をやり取りするためにあるのがMessageとCommandです。MessageはUpdateで処理されるアクションを表すデータ型で、そこに定義されたアクションが起こったときにMessageが発行されUpdateで処理されます。例えば、ボタンのクリック、APIへのリクエストへのレスポンスが帰ってきた、等です。Commandは外部とのやり取りを表します。APIへのリクエストを送るときに使います。



== 実装してみよう



以上のような方法によってElmで書かれたアプリケーションは動作しますが、実際のコードを見ながら追いかけてみましょう。



=== データ型の定義



アプリケーション内で用いるデータ型を定義していきましょう。今回は主にModelとMessageを定義します。



==== Model



まず、アプリケーションの状態であるModelを定義していきます。今回は以下の２つの状態を持つアプリケーションとします。* 取得されたページの内容を表示している状態* ページの読み込み中の状態


//emlist[][]{
type Model = MainPage Page | LoadingPage
//}



==== Message



Messageはその名の通り、アプリケーション内でイベント（？）を伝達するためのデータ型です。


//emlist[][]{
type Msg = NoOp | LoadMainPage (Result Http.Error Page)
//}


今回は単純に何もしないイベントと読み込みが完了した際のイベントを定義しました。



=== update, viewの定義



updateは前述のMessageを受けてモデルの状態を更新し、同時に新たなCommandを発行する関数です。例えば、上記の @<tt>{LoadMainPage (Result Http.Error Page)} を受けた場合に、その結果の@<tt>{Page}を使ってModelを@<tt>{MainPage Page} にし、読み込みが完了したため何もしない、という処理を書いています。


//emlist[][]{
update : Msg -> Model -> (Model, Cmd Msg)
update message model = case message of
    LoadMainPage (Ok result) -> (MainPage result, Cmd.none)
    _ -> (initialModel, Cmd.none)
//}


上記のupdateの処理によりModelが更新され、その更新がViewによってレンダリングされることにより、ユーザの目にAPIから取得してきたPageの内容が表示されました。



=== main



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


Html.programにはinit, view, update, subscriptionsという4つの引数が渡されています。１つずつ見ていきましょう。@<tt>{init}は、初期状態を表すModelです。## 参考文献https://matsubara0507.github.io/posts/2017-12-13-elm-and-haskell-for-elmer.htmlhttps://guide.elm-lang.org/architecture/



== 思ったこと




 * hpackなるものを知らなかった、これ便利そう
 * Update、実のところはModelを更新じゃなくて現在のModelを元に新しいModelを作るなんだけど、その説明いるかな
