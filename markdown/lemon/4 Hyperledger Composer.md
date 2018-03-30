# 4 Hyperledger Composer
## 4.1 Hyperledger Composer Playground
Hyperledger ComposerをCloneしてみたら、早速 Playgroundで遊んでみましょう。
ComposerのPlaygroundを立ち上げるには、
```
composer-playground
```
と実行してください。
ブラウザが起動し、先ほどのユーザー作成スクリプトが実行された後であれば、次のような画面が表示されます。

![](4%20Hyperledger%20Composer/%E3%82%B9%E3%82%AF%E3%83%AA%E3%83%BC%E3%83%B3%E3%82%B7%E3%83%A7%E3%83%83%E3%83%88%202018-03-26%200.24.41.png)

Hyperledger Composerでは、Business Network Cardを作って、Hyperledger Fabric環境にデプロイすることができます。実際にはBusiness Network Archiveファイルというのを作ります。

![](4%20Hyperledger%20Composer/composer01.png)

Business Network Archiveファイルは、上の図のように四つのファイルからなるものです。アセット情報、ユーザー情報を含めたモデル、ビジネスロジック、アクセス制御ファイル、あとはクエリです。

Model: Assets, Participants, Transactionといった定義を行います。
Script: ChainCodeの実態部分です。ビジネスロジックを書きます。
Access Control:どういうユーザーが何ができるのかを定義する場所です。
Query: 外部に提供するエンドポイントの定義になります。
 

Composer では、この四種のファイルを編集し、アプリケーションの開発を行っていきます。

## 4.2 Playgroundで遊ぶ
Playgroundで遊ぶために、tutorialのblockchainを作成してデプロイしてみましょう。
```
yo hyperledger-composer:businessnetwork
```

これでhyperledger-composerでアプリケーションを作るためのテンプレートが作成されます。`my_first_business_network `という名前で作成します。
作成されたファイルを見てみましょう。


```org.example.biznet.cto:
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
```

こちらは、モデル定義ファイルです。記法が特殊ですが、これはHyperledger Composer独自のモデル言語です。
CTOファイルは3つの要素からなります。
1. 一つのネームスペースで囲われていること
2. 資産、それに関わる人物、処理、イベントの定義
3. 他のネームスペースからの定義を取り込む

Asset/ Participantはそれぞれを識別するIDを指定してあげる必要があり、transaction/ eventはその必要がありません。
そのため、participant/ assetのみ by ~ がついています。



```logic.js
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
```

このファイルはtransaction/ eventが飛んできたときにどういう処理を行うかを記すものです。
テンプレートのChangeAssetValueというtransactionの実行時に呼ばれる処理が定義されています。
トランザクションが発生するとブロックチェーン上に新たなブロックが生成されます。

ロジック上では、アセットの処理（追加・削除・更新・取得）をおこなうためにAssetRegistry、
モデル(Asset/ Participant/ Event)の作成処理にFactoryというものを使っています。

この他にQueryファイル/ AClファイルがあります。
早速サンプルをアーカイブしてブラウザ上で実行してみましょう。

サンプルフォルダ内で次のコマンドを実行しましょう
```:
composer archive create -t dir -n .
```

Composerによってアーカイブファイル(.bna)が作成されます。
次に、作成されたビジネスネットワークカードをどのユーザー権限でFabricにデプロイできるようにするかを設定してinstall処理を行います。
最初に作成したユーザーを使ってこのアーカイブされたファイルをいれましょう
```
 composer runtime install --card PeerAdmin@hlfv1 --businessNetworkName my_first_business_network
```

インストールしたら、起動させましょう。
```
composer network start --card PeerAdmin@hlfv1 --networkAdmin admin --networkAdminEnrollSecret adminpw --archiveFile my_first_business_network@0.0.1.bna --file networkadmin.card
```

ちなみに実際にnetwork(アプリ)が立ち上がってるかはコマンドを打つことで確認できます
```
composer network ping --card admin@my_first_business_network
```

再度
```
composer-playground
```
でPlaygroundを開くと次のようになっていると思います。


![](4%20Hyperledger%20Composer/%E3%82%B9%E3%82%AF%E3%83%AA%E3%83%BC%E3%83%B3%E3%82%B7%E3%83%A7%E3%83%83%E3%83%88%202018-03-30%2015.36.38.png)

確かに先ほど作成したビジネスネットワークが追加されていることが確認できますね。Connect Nowよりエディター画面にいってみましょう。

ReadMe.md, Model File, Script Fileがあると思います。
上のタブからTestを押して見ましょう。
ここでは作成したモデルの作成/ トラザクションの実行が行えます。

![](4%20Hyperledger%20Composer/%E3%82%B9%E3%82%AF%E3%83%AA%E3%83%BC%E3%83%B3%E3%82%B7%E3%83%A7%E3%83%83%E3%83%88%202018-03-30%2015.40.18.png)右上のCreate New Participantからモデルを2つほど作成しましょう。
SampleAssetsについても同様に作成しましょう。

![](4%20Hyperledger%20Composer/%E3%82%B9%E3%82%AF%E3%83%AA%E3%83%BC%E3%83%B3%E3%82%B7%E3%83%A7%E3%83%83%E3%83%88%202018-03-30%2015.41.10.png)
それでは、左下のSubmit Transactionからトランザクションを発行して見ましょう

![](4%20Hyperledger%20Composer/%E3%82%B9%E3%82%AF%E3%83%AA%E3%83%BC%E3%83%B3%E3%82%B7%E3%83%A7%E3%83%83%E3%83%88%202018-03-30%2015.42.05.png)

アセットのIDは実存するものにしてください。
これを実行後、Assetsのほうへ行ってみると確かに値が書き換わっていることがわかります。

![](4%20Hyperledger%20Composer/%E3%82%B9%E3%82%AF%E3%83%AA%E3%83%BC%E3%83%B3%E3%82%B7%E3%83%A7%E3%83%83%E3%83%88%202018-03-30%2015.42.45.png)

このビジネスネットワーク上で発生したトランザクションは、左のAll Transactionよりみることができます。
アセット(Userとか)の追加処理、ビジネスロジックの更新、定義したトランザクション処理(ここではChangeAssetValue)が発生していることがわかりますね。

また、上タブのDefineからは実際のビジネスロジック
モデル定義を更新することができます。

このようにモデルと、ロジックは基本的なつなぎこみさえできていればブラウザ内で完結できるのも魅力的ですね。

## 4.2 Playgroundの先へ
実際にWebアプリケーションやネイティブから実行するためには、Transaction処理をAPIから処理できなければなりません

ComposerにはモデルファイルからRest APIを自動生成する便利な機能があります。

作成したフォルダ内(4章のmy_first_business_network)のルートで次のコマンドを実行して見てください。
```
composer-rest-server
```

そうすると色々聞かれると思いますので、
Business networkカード名は `admin@my_first_business_network`
APIにネームスペースを使用するかと聞かれるとNo,
認証処理は必要ないのでNo,
WebSocketによるイベント発行はYes,
Rest APIにTLSをいれるかはNoで答えます。
```
? Enter the name of the business network card to use: admin@my_first_business_network
? Specify if you want namespaces in the generated REST API: never use namespaces
? Specify if you want to enable authentication for the REST API using Passport: No
? Specify if you want to enable event publication over WebSockets: Yes
? Specify if you want to enable TLS security for the REST API: No
```

そうするとlocalhost:XXXX/ explorerというアドレスが発行される（ログででる）のでそちらを開いてみてください。

![](4%20Hyperledger%20Composer/%E3%82%B9%E3%82%AF%E3%83%AA%E3%83%BC%E3%83%B3%E3%82%B7%E3%83%A7%E3%83%83%E3%83%88%202018-03-30%2015.58.43.png)
このように各Transactionの実行に必要なAPIが記されたドキュメントが自動生成されていることがわかります。

Try it out!で実際にその場でAPIを実行することもできます

Curlコマンドでの実行方法も表示されるのでterminalから
実行し、同じ結果が返ってきてるか確認してみましょう。

