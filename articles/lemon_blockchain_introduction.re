= 分散技術とこれから
== 分散の時代

フリーランスで働く人が増えています。
フリマアプリをつかったり、仮想通貨の売買とかもよく聞くようになりました。

世の中がどんどん分散する方に移ろうとしています。
言い換えれば個の時代なのかもしれません。

テクノロジーもその影響がでてきています。
それがブロックチェーンの登場です。

データややりとりの格納場所をどこかに集中させるのではなく、
みんなで分散して管理する技術、それがブロックチェーンです。

この章では、ブロックチェーンとは何かということ、
そしてブロックチェーンを実際に実装したHyperledger Fabricを
簡単に扱えるHyperledger Composerを通して触ってみる、ということを目標にしています。

章を通じてComposerの生成されるサンプルを使って感触を感じてもらえたら嬉しいです。

== ブロックチェーン

最近、仮想通貨の話がよく聞こえるようになってきたものの
その裏側にあるブロックチェーンの技術ははさほど話題になっていない印象を受けます。

//image[basic_network][ブロックチェーンネットワーク]{
//}

ブロックチェーンは「分散してユーザー同士で管理するシステム」です。
厳密には、分散台帳と呼ばれているものです。

ブロックチェーンは次のような特徴があります。
	1. 改竄できない
	2. 中央集権化を防げる

ブロックチェーンは、仮想通貨の取引を守るシステムだけではありません。
あらゆる”取引”を守る技術としての可能性を秘めています。

ブロックチェーンでは、取引情報を書き込んだブロックをつなげた情報を
台帳と呼ばれる形で書き込みます。その台帳自体がブロックチェーンです。

とあるノード（サーバーインスタンスの台帳）に取引情報が送信され、
ブロックが生成されると、そのブロックが他のノードに送信されます。
その際、受け取ったブロックが正しいものであるかどうかを検証します。
この検証に通過すると、ブロックは他のノードにも追加されます。

ブロックは、[取引情報 + 前のブロックのハッシュ値 + TimeStamp]
などからなることが多いです。

1.の改ざんできない、という部分については、上記の取引情報のブロックが
必ず前のブロックに紐づいたハッシュ値を持っているという部分からきています。一度送信され台帳に書き込まれたものは後から取り外すことはできないのです。

2.の中央集権化を防げるという面については、ネットワーク上の他のノードにブロックを共有し、台帳に書き込まれるようになっていることから、どこかのサーバーの台帳が失われても大丈夫なつくりになっていることです。
これにより情報がどこかに集約されて、その集約されたところがリスクテイクをするという従来の管理システムとはまったくことなるというところからきています。

//image[Smart_Contract][スマートコントラクトとブロックチェーンのやりとり]{
//}

=== スマートコントラクト
それぞれのブロック上に書き込まれる情報や、取引の処理は「スマートコントラクト」と呼ばれ、いわゆるブロックチェーンにおけるアプリケーション層となっています。
上の図に記したように、スマートコントラクトによってトランザクション（tx）の情報が作成・更新され、それが台帳に書き込まれます。どういったデータや取引を書き込むのか、ビジネスロジックなどを示したものがスマートコントラクトに当たります。

=== トランザクション
台帳状に書き込まれる一連の手続きを含んでいる処理のことを指します。
具体的には、仮想通貨のスマートコントラクトでは、「AliceさんがBobさんに100ドル渡した」
といったような手続きが書かれているものです。

この情報を受け取ったらブロックに書き込んで整合性を確認し、他の台帳にコンセンサス待ちのところへ送ります。

== Hyperledger Fabric

=== 仮想通貨に依存しないブロックチェーン
ブロックチェーンの大半は仮想通貨の裏で動くインフラのような立ち位置としてあり、そこを利用してアプリケーションを開発するようなものも多いです。

それに対して、オープンソースのブロックチェーン基盤を世界的に使おうという流れがあります。
Linux Foundationが主導する、Hyperledgerプロジェクトと呼ばれるものです。

//image[hyperledger_logo]["Hyperledger Project"]{
//}


Hyperledgerは分散台帳の共通規格として構想され、実際の実装されたものはいくつかあります。
1. Hyperledger Fabric（IBM）
2. Hyperledger Sawtooth（Intel）
3. Hyperledger Iroha （Soramitsu）

とさまざまな会社が実装したものを作っています。今回はこの中でもおそらく一番メジャーな
Hyperledger Fabricを使った環境構築と、簡単なブロックチェーンアプリケーションの実装をおこなってみましょう。

=== 3.2 スマートコントラクト（チェーンコード）
HyperledgerFabricでは、スマートコントラクトを「チェーンコード」と読んでいます。
チェーンコードはGo, Typescript（Javascript）などに対応しています。

定義しなければいけない処理としては
* どういう資産をブロックチェーン上で管理するのか
* スマートコントラクト上に参加する人
* ビジネスロジック部分
* アクセス権限

といったものになります。
どういうユーザーがブロックチェーン上の情報を取れて取れないのかとか、
情報を使って何をどう判定するのかというような処理と、取引で用いる、あるいは
保存する資産（アセット）を定義してあげるようなイメージですね。

デプロイ、開発、定義、これらを比較的簡単に勧められるツールとしてHyperledger Composerがあります。

今回はそれを使ってスマートコントラクトの開発、並びにクライアントの開発を行っていきましょう。　

=== 環境構築（Node）
Hyperledger FabricとHyperledger Composerの両方を構築し、簡単にプレイグランドで遊ぶところから始めましょう。
対象は*Mac OS*としています。

(この項では、2018/ 03 時点でのComposerとFabricのインストールガイドを参考に作成しています）
Composerの使用には、nodeが必要です。バージョンを指定できるよう、Node Version Managerをインストールしておきましょう。

//cmd{
  curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.33.0/install.sh | bash
//}

この際、Gitが初めてであればXcode Command Line Toolのインストールが求められることがあります。
Xcodeをインストールしたことがあれば入っていると思いますが、求められる場合はInstallという選択をタップして先に勧めてください

//cmd{
  touch .bash_profile
//}

上記のコマンドを $HOME( = Users/username/）で実行し、PATHの設定ファイルを作成します。

その後、
//cmd{
  curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.33.0/install.sh | bash
//}

を実行してください。NVMのインストールが始まります。インストールが終了したら、ターミナルを開き直してください。

NVMはバージョン管理ツールなので、バージョンを指定してインストールできます。
Nodeの環境は、特定のバージョンでしか実行できないパッケージも多く、バージョン管理ツール経由でNodejsをインストールすることを強くお勧めします。
Nvmが入ったら、次のコマンドを叩いてnodeをインストールしましょう。

//cmd{
  nvm install --lts
//}

NodeのLTSバージョン（Long Term Supportバージョン）を入れておきましょう。安定しています。
2018/03時点では、LTSは8.10.0です。

//cmd{
  nvm use --lts
//}

インストール後、上のコマンドを叩いてnvmに紐づくNodeを指定します。
そのあとNodeのバージョンがいくつかを調べるために
//cmd{
node --version
//}
を叩いてみてください。

=== 環境構築（Docker）
Hyperledgerの環境にはDockerが必要です。

@<href>{https://docs.docker.com/docker-for-mac/install/=download-docker-for-mac, "Install Docker for Mac | Docker Documentation"}

よりDockerをダウンロードしてインストールしておいてください。

=== 環境構築（VSCode）
推奨エディタとしてVisual Studio Codeをあげていますので


@<href>{https://code.visualstudio.com, "Visual Studio Code - Code Editing. Redefined"}

よりインストールしてください。
加えて、Visual Studio Codeには、Hyperledger Composer用の拡張機能が提供されています。
左の拡張機能ボタンより`Hyperledger Composer Extension`で検索してインストールしておきましょう。

=== Hyperledger Composerの環境構築
まずはComposerのCLIを入れましょう
//cmd{
npm install -g composer-cli
//}

続いて、
//cmd{
npm install -g composer-rest-server
//}

ComposerのRest Serverを立てるパッケージも入れましょう
//cmd{
npm install -g generator-hyperledger-composer
//}

Composerのgeneratorも入れます。このgeneratorを使用するにはyoを使います
//cmd{
npm install -g yo
//}

Composerのファイルはこちらで以上です。次項でPlaygroundを扱うのでPlaygroundを落としてください
//cmd{
npm install -g composer-playground
//}

=== Hyperledger Fabricの開発環境構築
Composerのデプロイ先であるブロックチェーン環境の構築も行います
//cmd{
  mkdir ~/fabric-tools && cd ~/fabric-tools
  curl -O https://raw.githubusercontent.com/hyperledger/composer-tools/master/packages/fabric-dev-servers/fabric-dev-servers.zip
  unzip fabric-dev-servers.zip
//}
FABRICの環境が解凍されるので、
//cmd{
cd ~/fabric-tools
./downloadFabric.sh
//}
で、Hyperledger Fabricの環境のダウンロードを始めてください。

フォルダ内で次のコマンドを実行することでHyperledger Fabricの環境のセットアップが完了します。（Dockerが起動している状態で行ってください）
//cmd{
 ./startFabric.sh
 ./createPeerAdminCard.sh
//}

== Hyperledger Composer
=== Hyperledger Composer Playground
Hyperledger ComposerをCloneしてみたら、早速 Playgroundで遊んでみましょう。
ComposerのPlaygroundを立ち上げるには、
//cmd{
composer-playground
//}
と実行してください。
ブラウザが起動し、先ほどのユーザー作成スクリプトが実行された後であれば、次のような画面が表示されます。

//image[first_composer_view][Composer初回時]{
//}

Hyperledger Composerでは、Business Network Cardを作って、Hyperledger Fabric環境にデプロイすることができます。実際にはBusiness Network Archiveファイルというのを作ります。

//image[composer01][]{

//}

Business Network Archiveファイルは、上の図のように四つのファイルからなるものです。アセット情報、ユーザー情報を含めたモデル、ビジネスロジック、アクセス制御ファイル、あとはクエリです。

 * Model: Assets, Participants, Transactionといった定義を行います。
 * Script: ChainCodeの実態部分です。ビジネスロジックを書きます。
 * Access Control:どういうユーザーが何ができるのかを定義する場所です。
 * Query: 外部に提供するエンドポイントの定義になります。


Composer では、この四種のファイルを編集し、アプリケーションの開発を行っていきます。

=== Playgroundで遊ぶ
Playgroundで遊ぶために、tutorialのblockchainを作成してデプロイしてみましょう。
//cmd{
yo hyperledger-composer:businessnetwork
//}

これでhyperledger-composerでアプリケーションを作るためのテンプレートが作成されます。`my_first_business_network `という名前で作成します。
作成されたファイルを見てみましょう。


//list[org.example.biznet.cto][org.example.biznet.cto][cto]{
namespace org.example.biznet

participant User identified by email {
  o String email
}

asset SampleAsset identified by assetId {
  o String assetId
  o String value
}

transaction ChangeAssetValue {
  o String newValue
  --> Asset relatedAsset
}
//}

こちらは、モデル定義ファイルです。記法が特殊ですが、これはHyperledger Composer独自のモデル言語です。
CTOファイルは3つの要素からなります。
1. 一つのネームスペースで囲われていること
2. 資産、それに関わる人物、処理、イベントの定義
3. 他のネームスペースからの定義を取り込む

Asset/ Participantはそれぞれを識別するIDを指定してあげる必要があり、transaction/ eventはその必要がありません。
そのため、participant/ assetのみ by ~ がついています。



//list[logic.js][ビジネスロジックを各ファイル][js]{
'use strict';
/**
 * Write your transction processor functions here
 */

/**
 * Sample transaction
 * @param {org.example.biznet.ChangeAssetValue} changeAssetValue
 * @transaction
 */
function onChangeAssetValue(changeAssetValue) {
    var assetRegistry;
    var id = changeAssetValue.relatedAsset.assetId;
    return getAssetRegistry('org.example.biznet.SampleAsset')
        .then(function(ar) {
            assetRegistry = ar;
            return assetRegistry.get(id);
        })
        .then(function(asset) {
            asset.value = changeAssetValue.newValue;
            return assetRegistry.update(asset);
        });
}
//}

このファイルはtransaction/ eventが飛んできたときにどういう処理を行うかを記すものです。
テンプレートのChangeAssetValueというtransactionの実行時に呼ばれる処理が定義されています。
トランザクションが発生するとブロックチェーン上に新たなブロックが生成されます。

ロジック上では、アセットの処理（追加・削除・更新・取得）をおこなうためにAssetRegistry、
モデル(Asset/ Participant/ Event)の作成処理にFactoryというものを使っています。

この他にQueryファイル/ AClファイルがあります。
早速サンプルをアーカイブしてブラウザ上で実行してみましょう。

サンプルフォルダ内で次のコマンドを実行しましょう
//cmd{
composer archive create -t dir -n .
//}

Composerによってアーカイブファイル(.bna)が作成されます。
次に、作成されたビジネスネットワークカードをどのユーザー権限でFabricにデプロイできるようにするかを設定してinstall処理を行います。
最初に作成したユーザーを使ってこのアーカイブされたファイルをいれましょう
//cmd{
 composer runtime install --card PeerAdmin@hlfv1 --businessNetworkName my_first_business_network
//}

インストールしたら、起動させましょう。
//cmd{
composer network start --card PeerAdmin@hlfv1 --networkAdmin admin --networkAdminEnrollSecret adminpw --archiveFile my_first_business_network@0.0.1.bna --file networkadmin.card
//}

ちなみに実際にnetwork（アプリ）が立ち上がってるかはコマンドを打つことで確認できます
//cmd{
composer network ping --card admin@my_first_business_network
//}

再度
//cmd{
composer-playground
//}
でPlaygroundを開くと次のようになっていると思います。


//image[added_business_network][ビジネスネットワーク追加後の画面]{
//}

確かに先ほど作成したビジネスネットワークが追加されていることが確認できますね。Connect Nowよりエディター画面にいってみましょう。

ReadMe.md, Model File, Script Fileがあると思います。
上のタブからTestを押して見ましょう。
ここでは作成したモデルの作成/ トラザクションの実行が行えます。

//image[create_new_participant][Composer画面]{
//}

右上のCreate New Participantからモデルを2つほど作成しましょう。
SampleAssetsについても同様に作成しましょう。

//image[create_transaction][Composer画面]{
//}
それでは、左下のSubmit Transactionからトランザクションを発行して見ましょう

//image[submit_transaction][Composer画面]{
//}

アセットのIDは実存するものにしてください。
これを実行後、Assetsのほうへ行ってみると確かに値が書き換わっていることがわかります。

//image[confirm_changed_assets][Composer画面]{
//}

このビジネスネットワーク上で発生したトランザクションは、左のAll Transactionよりみることができます。
アセット(Userとか)の追加処理、ビジネスロジックの更新、定義したトランザクション処理（ここではChangeAssetValue）が発生していることがわかりますね。

また、上タブのDefineからは実際のビジネスロジック
モデル定義を更新することができます。

このようにモデルと、ロジックは基本的なつなぎこみさえできていればブラウザ内で完結できるのも魅力的ですね。

=== Playgroundの先へ
実際にWebアプリケーションやネイティブから実行するためには、Transaction処理をAPIから処理できなければなりません

ComposerにはモデルファイルからRest APIを自動生成する便利な機能があります。

作成したフォルダ内(4章のmy_first_business_network)のルートで次のコマンドを実行して見てください。
//cmd{
composer-rest-server
//}

そうすると色々聞かれると思いますので、
Business networkカード名は `admin@my_first_business_network`
APIにネームスペースを使用するかと聞かれるとNo,
認証処理は必要ないのでNo,
WebSocketによるイベント発行はYes,
Rest APIにTLSをいれるかはNoで答えます。
//cmd{
? Enter the name of the business network card to use: admin@my_first_business_network
? Specify if you want namespaces in the generated REST API: never use namespaces
? Specify if you want to enable authentication for the REST API using Passport: No
? Specify if you want to enable event publication over WebSockets: Yes
? Specify if you want to enable TLS security for the REST API: No
//}

そうするとlocalhost:XXXX/ explorerというアドレスが発行される（ログででる）のでそちらを開いてみてください。

//image[generated_api_docs][generated API Docs]{
//}
このように各Transactionの実行に必要なAPIが記されたドキュメントが自動生成されていることがわかります。

Try it out!で実際にその場でAPIを実行することもできます

Curlコマンドでの実行方法も表示されるのでterminalから
実行し、同じ結果が返ってきてるか確認してみましょう。

== アプリケーションクライアントを実装してみよう
Rest APIができたので、簡易的なフロントを作成してみましょう。今回はVuejsを使います。

=== Vue
Vue.jsは速度、軽量性、コンポーネント思考を兼ね備えた非常に優れたWebフレームワークです。
//image[7D0724FE-9B9B-4655-B62E-E17815552BFC][Vue]{
//}
プロジェクトのテンプレート生成には`vue-cli`というのがあるのでそれを利用します。

Npmでインストールしましょう
//cmd{
npm install -g @vue/cli
# yarnユーザーは下のコマンドを叩いてください
yarn global add @vue/cli
//}

インストールが終わったらプロジェクトを作ります
//cmd{
vue init webpack composer-app

? Project name composer-app
? Project description A Vue.js project
? Author XXXXXXX
? Vue build standalone
? Install vue-router? No
? Use ESLint to lint your code? No
? Set up unit tests No
? Setup e2e tests with Nightwatch? No
? Should we run `npm install` for you after the project has been created? (recommended) npm

cd composer-app
npm run dev
//}

プロジェクトテンプレートが作成され、実行されました。
指定アドレスをブラウザで開くと次のような画面が表示されます。

//image[vue_template][Vue]{
//}

HttpClientにはAxiosを使いますので追加しましょう
`npm install —save axios`

エディターで作成されたファイルを編集します。
Vuejsでは、.`vue`ファイルの編集を通してコンポーネントの開発を進めていきます。
サンプルで生成されたComposerのAPIを使ってAssetsの取得と更新を行える画面の実装をやってみましょう。

=== HelloWorld.vue
Vuejsでは、各Componentが.vueファイルごとに作成されています。
Helloworkd.vueを次のように修正してみましょう

//list[HelloWorld.vue][Component][vue]{
  <template>
    <div class="hello">
      <div v-for='(asset, index) in assets' :key='index'>
        <div class="asset-element"  v-on:click="selected(asset)">
            <div> ID: {{ asset.assetId }} </div>
            <div> Value: {{ asset.value   }} </div>
        </div>
      </div>
      <div class="create-assets-form">
        <input v-model="newId" placeholder="please input asset id">
        <input v-model="newValue" placeholder="please input asset value">
        <button v-on:click="addAssets" class="add-button"> create assets</button>
      </div>
    </div>
  </template>

  <script>
  import axios from 'axios'
  export default {
    name: 'HelloWorld',
    data () {
      return {
        assets: [],
        newId: "",
        newValue: ""
      }
    },
    created: function() {
      this.requestAssets()
    },
    methods: {
      requestAssets: function() {
        axios({
          method:'get',
          url:'http://localhost:3000/api/SampleAsset'
        }).then(res=>{
          // console.log()
          this.$data.assets = res.data
        });
      },
      addAssets: function() {

        let id = this.$data.newId;
        let val = this.$data.newValue;

        if (id.length <= 0 || val.length <= 0) {
          return;
        }

        let filtered = this.$data.assets.filter(el=>el.assetId == id)
        if(filtered.length <= 0) {
          // IDがない場合は新規作成処理
          axios({
            method:'post',
            url:`http://localhost:3000/api/SampleAsset`,
            data: {
                "$class": "org.example.biznet.SampleAsset",
                "assetId": id,
                "value": val
            }
          }).then(res=>{
            this.$data.assets.push(res.data)
          });

          return
        }
        console.log('該当するIDの値を更新します')
          axios({
            method:'put',
            url:`http://localhost:3000/api/SampleAsset/${id}`,
            data: {
                "$class": "org.example.biznet.SampleAsset",
                "assetId": id,
                "value": val
            }
          }).then(res=>{
            this.requestAssets()
          });
      },
      selected: function(asset) {
        this.$data.newId = asset.assetId
        this.$data.newValue = asset.value
      }
    }
  }
  </script>

  <!-- Add "scoped" attribute to limit CSS to this component only -->
  <style scoped>
  .asset-element {
    height: 40px;
    border-radius:12px;
    background-color:antiquewhite;
    display: flex;
    justify-content: space-around;
    align-items: center;
    box-shadow: 4px 2px 4px rgba(0,0,0,0.6);
    transition: 0.3s;
    margin: 8px;
  }

  .asset-element:hover {
    box-shadow: 4px 10px 4px rgba(0,0,0,0.6);
    transition: 0.3s;
  }

  .create-assets-form {
    height: 40px;
    display: flex;
    justify-content: space-around;
    align-items: center;
  }
  </style>
//}

コード中のlocalhostのポートは適宜読み替えてください。


//image[vue-assets-update][アセット更新画面]{
//}

画像のように、Assetsの更新、作成を行える画面が表示されました。



== これから起こること
=== 感謝
お疲れ様でした。今回は、ほんの少しだけチュートリアルを進める程度に解説を抑えました。

将来的にはがっつりビジネスロジックを含めたアプリケーション実装をやってみたいものです。

どういう定義が必要で、REST APIとブロックチェーンの連携がおこなわれるかを一気通貫で解説できたので、ここからどういうロジックを作るのか、見た目を作るのかは皆様にお任せしたいと思います。

実際に手を動かして確認をしたため、読みながらやってみると、
ブロックチェーンというバズワードはインフラレイヤー、データ構造の話にすぎないことに気づいた方もいらっしゃったかもしれません。

Hyperledger Composerの思想として、煩わしい部分を取り除いて、アプリケーションのためのブロックチェーンをいかに実現するかというテーマがあるようです。

サービスはビジネスに繋がらなければ価値を提供できません。
価値提供の手段としてのブロックチェーンを習得するにも、
Composerは非常にお勧めでした。


Hyperledgerのリポジトリにsample-networks（https://github.com/hyperledger/composer-sample-networks）というのがあります。
Composerで実装されたいくつかのサンプルをみることができます。
これを参考に自分なりのアプリケーションを作ってみるのも面白いですね。



=== これから
ブロックチェーンは2章で話したとおり、だれかが台帳を失ってもすぐに取り戻すことができる技術です。分散管理しているからですね。

分散して管理できるということは、個人的にはふたつの価値のイノベーションがおこると思っています。

1つはどこかに中央集権化して管理されているものが分散管理されることでそこに集中していた利益がみんな享受できるようになる。

公文書とか、マイナンバーとか、privateでありながらpublicに使われるようなものがどこかのサーバーに収納されているというのは非常にリスクだと思います。

これが分散されればリスクの分散とともに、管理者の既得権益を壊すことができると思います。

もうひとつは、管理しても価値がなかったものを管理するようになりあらたなビジネス価値が発掘される。

野菜の生産地、国籍、血統、コストに対してお金にならなくてみていなかったものが低コスト・低リスクで管理できるようになるためそれ自体に価値が生まれるというモデルです。どこの農家の野菜だから安全だし、栄養価も高い、というブランドが作れたりすると思います。

他にもブロックチェーンの起こせるイノベーションはまだまだあると思います。金融以外の分野でも。
ブロックチェーン x アプリケーションを通じて新たな価値提供ができるよう、自分も勉強していきたいとおもいます。
