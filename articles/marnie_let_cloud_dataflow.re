= Google Cloud Dataflowを使ったデータ処理入門

== 自己紹介

こんにちはこんにちは、株式会社エウレカのSREチームに所属している
@marnieです。
SREと言いつつgoやらjavaやらコード書いてる時間のが多い気がしますが、
気にしない方向で日々を過ごしています。

あまり関係ないですが、幼女戦記はコミック版が面白いので是非まだの方は
コミック版から入ってください。

さてさて、最近は猫も杓子もビッグデータ,機械学習という感じで、
データの活用以前に分析のための収集や処理といったニーズは増えていく一方ですね。

データが多すぎて処理がパンクしてデータ欠損する悲鳴もあれば、
これからデータ基盤を作りたいんだけど、どうしようといった悩みを持っていたり、
聞いた方もいらっしゃるのではないでしょうか。

そんなデータラッシュの風に翻弄されている私が、最近業務で触っていたGoogle Cloud Dataflowの良さやら、
コードの書き方、などなどをテーマに書いていきたいと思います。

== Cloud DataFlowって?

GoogleCloudPlatformが提供するストリーム/バッチ方式でのデータ処理用のフルマネージドサービスで、
ApacheBeamを基にしたSDKが提供されており任意のinput(pubsubやmysql,gcs)から得たデータの変換、
GCS,BigQueryへのデータ流し込みといったいわゆるETL(抽出/変換/データハウス出力)を並列処理で
Java,Pythonでプログラムで表現することが可能です。
 
(*) 2018/02時点ではストリーム対応はJavaのみ

== どんなところがよいの?

* フルマネージドなのでリソース管理がほぼ不要。ワークロードバランスの調整を自動でよしなにやってくれる
* GCP内の別サービスとの連携が容易(CloudPubSubやBigQuery,GCSのようなDataStoreへのInput/Outputが標準でサポートされている)
* 大体の変換をプログラミングで表現できる

CloudPubsubからのデータ入力などは実質数行で表現できますし、Window処理や並列処理といった分析でニーズがあるところもサポート
されています。
国内の大規模サービス(Abema,mercari)でも採用されていたりと、事例も増えてきているのは、事例を求められがちな会社さんとしても
安心できるところですし、料金体系的にも従量課金なので、まずはデータ基盤を作ってみるという
これからデータ基盤を構築するフェーズには非常に適しています。
データ本当に使うんかなぁ、とかいろいろ考えるとSpark Streamingを0から構築しようぜ！とか男気を見せるよりはよっぽど敷居が低くて
スモールスタートしやすいですね。

とりあえずデータ基盤ほしいんだよ！規模とかわかんないけどさぁ！XX君!

そんなふわっとした無茶な要求に対しても華麗にカウンターを決められます。
転ばぬ先のDataFlow。

== 活用方法

いわゆるETL基盤としての活用も勿論可能ですが、同じGCPのサービスであるCloud Pub/Subをメッセージバスとして利用し、
CloudIoTCoreやCloudEndPoint等と組み合わせる事で,各端末~データストアまでのデータ集積基盤を構築する事も容易です。

Googleのサンプル画像から見る大雑把な利用モデル

img


== 実践しよう!

実際に使ってみないと分かりづらいので、今回はこんな感じのフローを作りながら説明をしていきたいと思います。

img

PubSub周りの連携を書きたかったので、入り口をCloudPubSubにしていますが、BigQuery内のテーブルAとテーブルBのデータを加工してテーブルCにロードするような挙動も勿論可能です。


=== 開発環境の作り方

pom.xmlに以下を記載してSDKをダウンロードします。
//listnum[pom.xml][xml]{

    <dependency>
      <groupId>com.google.cloud.dataflow</groupId>
      <artifactId>google-cloud-dataflow-java-sdk-all</artifactId>
      <version>[2.2.0, 2.99)</version>
    </dependency>
//}

=== 動作の設定についてのコードを書く

まず処理を行うパイプラインのオプションを設定します。

//listnum[pipeLineOptionsのサンプルコード][java]{
        DataflowPipelineOptions options = PipelineOptionsFactory.create()
                .as(DataflowPipelineOptions.class);
        // 自分の使っているプロジェクト名を指定
        options.setProject("your gcp porject");
        // Dataflowがstagingに利用するGCSBucketを作って指定
        options.setStagingLocation("gs://hoge/staging");
        // Dataflowが一時利用するGCSBucketを作って指定
        options.setTempLocation("gs://hoge/tmp");
        // 実行するランナーを指定。GCP上で実行する場合はDataflowRunnerを指定。local実行の場合はDirectRunner
        options.setRunner(DataflowRunner.class);
        // streamingを有効にする
        options.setStreaming(true);
        // 動作時の名称を指定(同じ名称のジョブは同時に稼働できない
        options.setJobName("sample");
//}

公式(https://beam.apache.org/documentation/runners/dataflow/)にも説明がありますので、大雑把なところだけ。

(*) 上の例では、設定をコード上から分離したかったので、JavaのResourceBundleを使ってますが動作させるだけなら直値でも動きます。

=== Dataflow上でのプログラミングを構成する基礎概念

基本的にDataflowのジョブをプログラミングする上では以下の
概念を取り扱うことになります。

- PipeLine
    - 処理ジョブを表現するオブジェクト
        - 基本的にPipeLineに処理の流れ(入力・変換・出力)を適用(apply)していく。

- PCollection
    - データを表現するオブジェクト

- 変換
    - 入力データを出力データに変換する処理部分

- PipeLineI/O
    - 入力ないし、出力の定義

基本的にPipeLineに自分の書いた変換処理とPipeLineI/O等の必要な処理を適用して
ジョブを構築する事になります。

=== inputの読み取り~outputまでのコードを書く

上の図に書いた３つの動作

* CloudPubSubからのメッセージ読み取り
* json(string)をBigQueryにoutputする形(BigQueryRow)に変換する
* BigQuery上のテーブルにロードする

をパイプラインに適用していきます。

//listnum[パイプライン構築のサンプルコード][java]{
        //パイプライン（処理するジョブ)オブジェクトを生成
        Pipeline p = Pipeline.create(options);
        TableSchema schema = SampleSchemaFactory.create();
        // 処理内容を適用する
        // pubsubのsubscriptionからデータを読み出す
        p.apply(PubsubIO.readStrings().fromSubscription("your pubsub subscription"))
        // 5分間隔のwindowを指定(なくても可)
                .apply(Window.<String>into(FixedWindows.of(Duration.standardMinutes(5))))
        // pubsubからの入力に対する変換を設定 (実装は後述)
                .apply(ParDo.of(new BigQueryRowConverter()))
        // BigQueryへの書き込みを設定
                .apply("WriteToBQ", BigQueryIO.writeTableRows()
                        //書き込み先テーブル名を指定
                        .to(TableDestination("dataset_name:table_name","description"))
                        //書き込み先のschemaをObjectで定義して渡す
                        .withSchema(schema)
                        //テーブルがなければ作成する(オプション)
                        .withCreateDisposition(BigQueryIO.Write.CreateDisposition.CREATE_IF_NEEDED)
                        //テーブル末尾にデータを挿入していく（オプション)
                        .withWriteDisposition(BigQueryIO.Write.WriteDisposition.WRITE_APPEND));
        // 実行
        p.run();
//}

* 上記で設定したJsonObjectからBigQueryへの変換(`BigQueryRowConverter`)の実装

ParDoを使った変換の場合、DoFnを継承し抽象メソッドであるprocessElementの中にデータの取り出し〜PCollectionへの変換を実装します。

//listnum[変換処理のサンプルコード][java]{
package com.mycompany.dataflow_sample.converter;

import com.google.api.services.bigquery.model.TableRow;
import com.google.gson.Gson;
import com.mycompany.dataflow_sample.entity.SampleInputJson;
import org.apache.beam.sdk.transforms.DoFn;

public class BigQueryRowConverter extends DoFn<String,TableRow> {

    @ProcessElement
    public void processElement(ProcessContext dofn) throws Exception {
      // 入力を受け取る
      String json = dofn.element();
      Gson gson = new Gson();
      // jsonをobjectに変換
      SampleInputJson jsonObj = gson.fromJson(json,SampleInputJson.class);
      // jsonの内容をbigqueryのtableRowに変換していく
      TableRow output = new TableRow();
      TableRow attributesOutput = new TableRow();
      TableRow attr2Output = new TableRow();
      // 出力にデータをセットする
      attributesOutput.set("attr1", jsonObj.attributes.attr1);
      attributesOutput.set("attr2", jsonObj.attributes.attr2);
      attr2Output.set("attr2_prop1",jsonObj.attributes.attr2.prop1);
      attr2Output.set("attr2_prop2",jsonObj.attributes.attr2.prop2);

      attributesOutput .set("attr2",attr2Output);
      output.set("attributes", attributesOutput );
      output.set("name", jsonObj.name);
      output.set("ts", jsonObj.timeStamp/1000);
      // 出力する
      dofn.output(output);
    }
}
//}

* 上記の`SampleSchemaFactory.create()`の実装

//listnum[BigQueryのSchemaObjectのサンプルコード][java]{
package com.mycompany.dataflow_sample.schema;

import com.google.api.services.bigquery.model.TableFieldSchema;
import com.google.api.services.bigquery.model.TableSchema;
import java.util.ArrayList;
import java.util.List;

public class SampleSchemaFactory {
    public static TableSchema create() {
        List<TableFieldSchema> fields;
        fields = new ArrayList<> ();
        fields.add(new TableFieldSchema().setName("name").setType("STRING"));
        fields.add(new TableFieldSchema().setName("ts").setType("TIMESTAMP"));
        fields.add(new TableFieldSchema().setName("attributes").setType("RECORD")
                .setFields(new ArrayList<TableFieldSchema>() {
                    {
                        add(new TableFieldSchema().setName("attr1").setType("STRING"));
                        add(new TableFieldSchema().setName("attr2").setType("RECORD")
                            .setFields(new ArrayList<TableFieldSchema>() {
                                {
                                    add(new TableFieldSchema().setName("prop1").setType("INTEGER"));
                                    add(new TableFieldSchema().setName("prop2").setType("STRING"));
                                }
                            })
                        );
                    }
                })
        );
        TableSchema schema = new TableSchema().setFields(fields);

        return schema;
    }
}

//}
こんな感じ。

=== デプロイ/テスト

* デプロイ

先ほど書いたJavaのコードをビルドして実行するだけで実際にGCP上にジョブがデプロイされます。

img

* CloudPubSubからメッセージを送る

GCPのコンソール上からメッセージを送ります。

img

BigQueryにデータが挿入されていれば動作確認はOKです :)

=== ログの確認

コンソール上から作成したジョブをクリックし、詳細画面からログが確認できます。
stackdriverにも自動でログが転送されていますので、モニタリング、監視などはそちらを使うと良いです。

img

=== エラー(例外発生)時の挙動

DataflowJobを実行中にExceptionが発生した場合、PubSubにACKを送らないので再度データが取り出される事になります。

従って

 - BigQueryへのロード部分等で処理がこけてもPubSubのメッセージが破棄されないので自動でリトライされる(通信レベルでのリトライの考慮は不要)
 - 反面でデータが正しくない場合はエラーを吐き続ける事になるので、Validationをかけてエラーデータに移す、ロギングする、破棄する等のハンドリングを適切に行う必要がある。(何回かエラーが溢れて涙目になりました。)

== まとめ

さて、まだまだ色々と説明できていない部分もありますが、ちょっとしたサンプルを基に
今回CloudDataFlowの使い方や良さについて語ってみました。

私もまだまだ研究中ですので、この記事を読んで
以下のような気持ちや特徴をお持ちの方は、カジュアルランチもやってますので
是非お気軽にエウレカを訪問ください!

* わしのデータフローは108段あるぞ
* そんなデータ処理で大丈夫か？
* 一緒に全国大会に出たい
* メガネっ娘である

それではまたどこか、エオルゼアとかヴァナディールで。

== 参考

https://github.com/GoogleCloudPlatform/DataflowSDK-examples
https://beam.apache.org/
https://cloud.google.com/dataflow/?hl=ja
