= Redux on Android

== Reduxとは

=== Reduxが登場した背景

Androidアプリに求められる要件は年々複雑化してきており、それに伴って扱う状態も増えてきています。具体的には、UIに表示される画像やテキストデータ、ユーザーが入力したデータ、UI構築に必要な内部データなどが代表的なものです。さらにそれらのデータはサーバーから取得したものであったり、ローカルでキャッシュしたものであったりすることがより開発を難しくしています。シンプルなアプリであれば状態管理はそれほど問題になることはないですが、大規模なアプリになればなるほど状態管理はクリティカルな問題となることが多いです。

Reduxはこういった状態管理にまつわる問題を解消するために登場しました。本来はWebフロント開発で使われることが多いですが、Redux自体は非常にシンプルな考え方なので、Webフロント以外のプラットフォームにも容易に移植することができます。

=== Reduxの概要

Reduxを一言で表現すると、アプリの状態管理に特化したフレームワークです。Fluxがベースになっており、 アプリにおけるデータの流れを1方向に制限することで状態管理にまつわる問題を解決する、というアプローチを採用しています。

//image[redux-overview][Reduxの全体像]

Reduxは次の3原則を中心に設計されており、これらを実現するための最小構成なフレームワークとして実装されています。

==== Single source of truth

Reduxでは状態は1箇所で管理するというアプローチを取っています。これによりデバッグや状態のバリデーションなどもやりやすくなります。アプリの要件にも依存しますが、UNDO/REDOなども状態のスナップショットを保存することで容易に実装できます。

==== State is read-only

ReduxにおけるState（＝状態）は読み取り専用で定義されており、Stateの中身を変更することはできません。Stateを変更したい場合はStoreに対してActionをDispatchする必要があります。この制約があることにより、Stateの変更箇所が限定されることになるので、コードの見通しが良くなります。

==== Changes are made with pure functions

ReduxではStoreにDispatchされたActionをもとにStateを変更しますが、その変更処理はStore自体ではなく、Reducerが担当することになります。Reducerは現在のStateとActionを受け取り、そこから新しいStateを返す関数として実装すべし、という制約があります。この制約があることで、Reducerは同じStateとActionを与えて呼び出すと、必ず同じStateを返すため、テストが非常に書きやすくなるというメリットがあります。

=== Fluxとの違い

ReduxはFluxがベースになっており、基本的な概念は同じですが、以下のような違いがあります。

 * FluxではStoreの個数に制限はないが、ReduxではStoreが1個しかない

 * Fluxでは状態変更を誰が行うのかは定められていないが、ReduxではReducerが担当する

基本的にはFluxでは曖昧に表現されていた箇所に制約を設けたものがReduxであり、根本にある考え方は同じです。

== Androidにおける状態管理の難しさ

本節では、Androidアプリにおける状態管理でどのような難しさが存在するのかを説明していきます。

=== 複数のデータソースを扱う必要性

Androidに限った話ではありませんが、昨今のフロントエンド開発では当たり前のようにデータソースが複数存在している場合が多いです。具体的には、API経由でやり取りするサーバー、SQLiteなどを使ったローカルDB、SharedPreferencesを使ったKey-Valueストア、メモリやディスクなどを使ったキャッシュなどなど、代表的なものでもこれだけのデータソースがあります。これらのデータソースの特徴を意識しながらコードを書いていくのはかなり大変な作業です。

=== 状態に種類があること

Reduxは状態管理にまつわる問題を解決するために登場したものですが、状態と一口に言っても、実は色々な種類の状態があります。

==== 永続的な状態

サーバーやローカルDBに保存され、サービス内においては永続化されているものを指しています。

==== 一時的な状態

ユーザーが入力している最中のテキストや、メモリにキャッシュされた値などを指しています。

これらの状態は一貫性のある設計を採用していない限りは、コード内の色々な箇所で管理され、コードの見通しが悪くなる要因となります。

=== 非同期処理の扱い、非同期処理後の状態変更

Androidでは時間のかかる処理はメインスレッド以外のスレッドで実行します。Androidで非同期処理を実行する場合は、AsyncTask、Loader、Serviceなどを使うことになりますが、これらはそれぞれ実装方法が大きくことなっており、状態管理が複雑化する要因になってしまいます。また、マルチスレッドプログラミング自体がそもそも難易度の高い手法であることも状態管理を難しくする要因になります。

== 状態管理の難しさにReduxで対抗する

本節では、Androidアプリ開発における状態管理の難しさに対して、Reduxで対抗するアプローチを説明していきます。

=== 状態管理がどのように変わるか

Reduxではアプリの状態は全てStoreで管理され、ViewはStoreから渡された状態を使って画面を構築することになります。Storeが保持する状態が変更された場合はStoreから通知を受け取って画面を再描画します。これにより、Viewが具体的なデータソースを知ることがありません。また、アプリの状態には「永続的な状態」と「一時的な状態」の2つが存在しますが、いずれもStoreで一括管理することでViewが状態の種類を意識する必要はありません。

=== 非同期処理がどのように変わるか

Reduxでは非同期処理と状態変更が明確に分離されています。Reduxにおける非同期処理の実装方法はいくつか存在しますが、一番メジャーなのはMiddlewareにて非同期処理を実行する方式です。非同期処理はMiddlewareが担当し、状態管理はStoreが担当するというように責務を分離することでテスタビリティを担保することができます。

== サンプルアプリ

本節では、Reduxを用いてサンプルアプリを実装していきます。今回実装するのは、カウンターとTODOアプリです。単一の機能を持ったアプリではReduxのメリットを活かしきれないため、複数の機能を組み合わせたサンプルアプリを実装しています。

//image[redux-sample-app][サンプルアプリ]

=== State

アプリ全体の状態を管理するクラスとして、AppStateを定義します。AppStateはカウンターを保持するCounterStateと、TODOを保持するTodoStateを持っています。CounterStateはInt型のカウンターを持ち、TodoStateはTodoクラスをリストで持っています。

//emlist[][]{
data class AppState(val counter: CounterState, val todo: TodoState)

data class CounterState(val count: Int)
data class TodoState(val todos: List<Todo>)
//}

=== Reducer

AppStateの変更を担当するクラスとして、AppReducerを定義します。AppReducerの内部では、CounterStateの変更を担当するCounterReducerと、TodoStateの変更を担当するTodoRedecerに処理が委譲されています。

//emlist[][]{
class AppReducer : Reducer<AppState, Action>() {
    private val counterReducer = CounterReducer()
    private val todoReducer = TodoReducer()
    override fun apply(state: AppState, action: Action): AppState {
        return when (action) {
            is CounterAction -> {
                state.copy(counter = counterReducer.apply(state.counter, action))
            }
            is TodoAction -> {
                state.copy(todo = todoReducer.apply(state.todo, action))
            }
            else -> state
        }
    }
}

class CounterReducer : Reducer<CounterState, CounterAction>() {
    override fun apply(state: CounterState, action: CounterAction): CounterState {
        return when (action) {
            is CounterAction.Increment -> state.copy(count = state.count + 1)
            is CounterAction.Decrement -> state.copy(count = state.count - 1)
            is CounterAction.Reset -> CounterState.initial()
        }
    }
}

class TodoReducer : Reducer<TodoState, TodoAction>() {
    override fun apply(state: TodoState, action: TodoAction): TodoState {
        return when (action) {
            is TodoAction.Add -> state.copy(todos = state.todos.plus(action.todo))
            is TodoAction.Reset -> TodoState.initial()
        }
    }
}
//}

AppReducerは現在のAppStateとDispatchされたActionから、新しいAppStateを生成する役割を持っています。AppReducerの内部では、引数のActionをもとに適切なReducerで処理が行われます。Reduxではアプリの状態を単一のStateで表現しますが、Reducerを分割することで状態変更処理が複雑化することを防いでいます。このようにAppReducerを中心にして複数のReducerを組み合わせる手法をCombineReducerと呼びます。

=== Store

AppStateを保持し、変更通知を担当するクラスとして、AppStoreを定義します。

//emlist[][]{
class AppStore(
	initial: AppState,
    reducer: Reducer<AppState, Action>
) : Store<AppState, Action>(initial, reducer)
//}

AppStoreの処理は親クラスであるStoreで実装されています。

//emlist[][]{
abstract class Store<STATE>(
	initial: STATE,
	private val reducer: Reducer<STATE, Action>
) {
    private val state = BehaviorRelay.createDefault(initial)
    fun getState(): Observable<STATE> {
        return state
    }
    fun dispatch(action: Action): Observable<Action> {
        return Observable.just(action)
                // 事前処理
                .doOnNext {
                    val newState = reducer.apply(state.value, it)
                    Handler(Looper.getMainLooper()).post { state.accept(newState) }
                }
				// 事後処理
    }
}
//}

Storeでは、現在のStateをプロパティとして保持し、Storeの外部からはgetStateメソッドを呼び出すことでStateを取得することができます。また、Stateに対するReducerもプロパティとして保持しており、 Reducerはdispatchメソッドで渡されたActionをもとに状態変更を行います。

=== Middleware

ReduxではStoreに渡されたActionに任意の加工を施す仕組みがあり、Middlewareと呼ばれています。例えば、状態変更の前後でStateをログに出力したり、非同期なActionを実行してから状態変更を行ったり、といった具合に任意の拡張を加えることが可能です。

MiddlewareはActionの前後に割り込むことから、以下のようなインターフェースを定義します。

//emlist[][]{
interface Middleware {
    fun before(action: Action): Observable<Action>
    fun after(action: Action): Observable<Action>
}
//}

具体例として、ログを出力するMiddlewareと、非同期処理を実行するMiddlewareを紹介します。

//emlist[][]{
// ログを出力するMiddleware
class LoggerMiddleware : Middleware {
    override fun before(action: Action): Observable<Action> {
        Log.d("LoggerMiddleware", "Before dispatch: $action")
        return Observable.just(action)
    }
    override fun after(action: Action): Observable<Action> {
        Log.d("LoggerMiddleware", "After dispatch: $action")
        return Observable.just(action)
    }
}
// 非同期処理を実行するMiddleware
class ThunkMiddleware : Middleware {
    override fun before(action: Action): Observable<Action> {
        return if (action is AsyncAction) {
            action.execute()
        } else {
            Observable.just(action)
        }
    }
    override fun after(action: Action): Observable<Action> {
        return Observable.just(action)
    }
}
//}

MiddlewareはActionが実行される前後で作用することから、 Storeのdispatchメソッドを次のように修正します。

//emlist[][]{
private fun dispatch(action: Action): Observable<Action> {
    return Observable.just(action)
            .flatMap {
                var observable = Observable.just(it)
                for (middleware in middlewares) {
                    observable = observable
                            .flatMap { middleware.before(it) }
                }
                observable
            }
            .doOnNext {
                val newState = reducer.apply(state.value, it)
                Handler(Looper.getMainLooper()).post { state.accept(newState) }
            }
            .flatMap {
                var observable = Observable.just(it)
                for (middleware in middlewares) {
                    observable = observable
                            .flatMap { middleware.after(it) }
                }
                observable
            }
}
//}

=== Action

状態変更のトリガーを担当するクラスとして、Actionを定義します。

カウンター機能では、カウントの「インクリメント」、「デクリメント」、「リセット」という状態変更があると仮定して、それぞれをActionとして定義します。TODO機能では、TODOの「追加」、「リセット」という状態変更があると仮定して、カウンター機能と同様にそれぞれをActionとして定義します。

//emlist[][]{
sealed class CounterAction : Action {
    class Increment : CounterAction()
    class Decrement : CounterAction()
    class Reset : CounterAction()
}

sealed class TodoAction : Action {
    class Add(val todo: Todo) : TodoAction()
    class Reset : TodoAction()
}
//}

=== Storeの利用イメージ

Storeの状態変更を行うためには、dispatchメソッドを呼び出します。

//emlist[][]{
val action = CounterAction.Increment()
store.dispatch(it)
//}

Storeが保持する状態を監視するためには、getStateメソッドを使います。

//emlist[][]{
store.getState()
	.map { it.counter.count.toString() }
	.subscribe(binding.textView.text())
//}

=== リポジトリ

本書で紹介したサンプルコードは次のリポジトリで公開しています。ご興味ある方は覗いてみてください。

 * https://github.com/yuyakaido/Relux

== まとめ

TODO: まとめ的なものを書く
