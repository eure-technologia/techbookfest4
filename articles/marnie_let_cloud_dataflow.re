= Google Cloud Dataflowを使ったデータ処理入門

== 自己紹介

こんにちはこんにちは、株式会社エウレカのSREチームに所属している
@marnieです。
SREと言いつつgoやらjavaやらコード書いてる時間のが多い気がしますが、
気にしない方向で日々を過ごしています。

あまり関係ないですが、幼女戦記はコミック版が面白いので是非まだの方は
コミック版から入ってください。

== データラッシュ2018

さてさて、最近は猫も杓子もビッグデータ,機械学習という感じで、
データの活用以前に分析のための収集や処理といったニーズは増えていく一方ですね。

データが多すぎて処理がパンクしてデータ欠損する悲鳴もあれば、
これからデータ基盤を作りたいんだけど、どうしようといった悩みを持っていたり、
聞いた方もいらっしゃるのではないでしょうか。

そんなデータラッシュの風に翻弄されている私が、最近業務で触っていたGoogle Cloud Dataflowの良さやら、
コードの書き方、などなどをテーマに書いていきたいと思います。

== Cloud DataFlowって?

GoogleCloudPlatformが提供するストリーム/バッチ方式両方をサポートしたデータ処理用のフルマネージドサービスです。
ApacheBeamを基にしたSDKが提供されており任意のinput(pubsubやmysql,gcs)から得たデータの変換、
GCS,BigQueryへのデータ流し込みといったいわゆるETL(抽出/変換/データハウス出力)処理を
Java,Pythonでプログラムで表現することが可能です。

(*) 2018/02時点ではストリーム対応はJavaのみ

== どんなところがよいの?

* フルマネージドなのでリソース管理がほぼ不要。ワークロードバランスの調整を自動でよしなにやってくれる
* GCP内の別サービスとの連携が容易(CloudPubSubやBigQuery,GCSのようなDataStoreへのInput/Outputが標準でサポートされている)
* 大体の変換や分岐・繰り返し等をプログラミングで表現できる

CloudPubsubからのデータ入力などは実質数行で表現できますし、Window処理や並列処理といった大規模データ処理や分析でニーズの
あるところもサポートされています。
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

実際に使ってみないと分かりづらいので、今回はこんな感じのフローを例にとりながら説明をしていきたいと思います。

img

=== 前準備

CloudDataFlowの動作には以下が必要となりますので、
GCS(GoogleCloudStorage)に任意の名前でBucketを作成してください。

* CloudDataFlowが内部的に利用するstaging用のgcsBucketの作成
* CloudDataFlowが内部的に利用するtemp用のgcsBucketの作成

GCPのアカウントやプロジェクトの設定については準備できている事を前提としています。

=== 開発環境の準備

Streamingの対応は現状ではJavaSDKのみですので、今回はJavaで進めていきたいと思います。
pom.xmlに以下を記載してSDKをプロジェクトにダウンロードします。
//listnum[pom.xml][xml]{
    <dependency>
      <groupId>com.google.cloud.dataflow</groupId>
      <artifactId>google-cloud-dataflow-java-sdk-all</artifactId>
      <version>[2.2.0, 2.99)</version>
    </dependency>
//}

eclipseの場合はpluginからCloudDataFlow用のPluginなども用意されていますが、
この辺はIDEの好みで。

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
        // 実行するランナーを指定。GCP上で実行する場合はDataflowRunnerを指定。local実行の場合はDirectRunnerを指定します。
        options.setRunner(DataflowRunner.class);
        // streamingを有効にする
        options.setStreaming(true);
        // 動作時の名称を指定(同じ名称のジョブは同時に稼働できない
        options.setJobName("sample");
//}

コマンドラインから値を注入したい場合は以下のような書き方ができます。
今回の例ではsubscriptionとtableNameという引数の拡張をしたかったので
DataflowPipelineOptionsを継承したinterfaceを定義しています。

//listnum[実行時の引数の取り方のサンプル][java]{

    public static void main(String[] args) {
        PipelineOptionsFactory.register(XXXXOptions.class);
        XXXXOptions options = PipelineOptionsFactory.fromArgs(args)
                .withValidation()
                .as(XXXXLogOptions.class);
    }

    private interface XXXXOptions extends DataflowPipelineOptions {

        @Description("Input Pubsub subscription")
        @Validation.Required
        String getSubscription();
        void setSubscription(String subscription);

        @Description("Output BigQuery table")
        @Validation.Required
        String getBigQueryTable();
        void setBigQueryTable(String bigQueryTable);
    }
//}

//listnum[実行コマンドのサンプル][shell]{
	mvn compile exec:java \
	-Dexec.mainClass=YourMainClass \
	-Dexec.args="--project=YourGCPProject \
	--subscription=projects/yourProject/subscriptions/xxxxx \
	--bigQueryTable=yourProject:DatasetName.TableName \
	--tempLocation=gs://dataflowWorkSpace/tmp \
	--jobName=yourJobname-timestamp \
	--stagingLocation=gs://dataflowWorkSpace/staging"
//}

=== Dataflow上でのプログラミングを構成する基礎概念

基本的にDataflowのジョブをプログラミングする上では以下の
概念を取り扱うことになります。

- PipeLine
    - 処理ジョブを表現するオブジェクト
        - 基本的にPipeLineに処理の流れ(入力・変換・出力)を適用(apply)する事で処理を構築します。

- PCollection
    - 入力や出力といったデータを表現するオブジェクト

- 変換
    - 入力データを出力データに変換する処理部分

- PipeLineI/O
    - 入力ないし、出力関連のAPI群です。BigQueryやGCS,PubSub,Fileなど大体の用途の物は標準で用意されています。

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
        // 5分間隔のwindowを指定(使いたい場合は)
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
//}

=== デプロイ/テスト

* デプロイ

先ほど書いたJavaのコードをビルドして実行するだけで実際にGCP上にジョブがデプロイされます。

img

同じjobNameのものはDeployができないので、実運用ではjobのsuffixにtimestampをつけて
一つ前のJobをDorainで止めて新しいJobを立ち上げるような運用がよいのかなぁと。
仕組み上、新しいジョブのデプロイと古いジョブの入れ替えに必ずダウンタイムが発生するので
データが常に流れ続けるような物を入力にとる場合はCloudPub/Subのようなデータの滞留が可能な
メッセージバスで受けてSubscriptionから入力をとるようにする必要があります。

* CloudPubSubからメッセージを送る

GCPのコンソール上からメッセージを送ります。

img

BigQueryにデータが挿入されていれば動作確認はOKです :)

=== TIPS

実際に運用しながら気づいた所や役に立った情報等を記載していきます。

==== 動的に出力先を変更する

例えばデータの値によって動的に出力先を振り分けるというようなケースの場合は
SerializableFunctionをimplementしたカスタムクラスを実装すると実現が容易です。
下記の例ではデータのタイムスタンプを参照して日別のテーブルへのinsertを実装しています。

//listnum[PipeLine側のサンプル][java]{
        p.apply(PubsubIO.readStrings().fromSubscription(options.getSubscription()))
                .apply(Window.into(FixedWindows.of(Duration.standardMinutes(5))))
                .apply(ParDo.of(new BigQueryRowConverter()))
                .apply("WriteToBQ", BigQueryIO.writeTableRows()
                        .to(new DayPartitionDestinations("dataset:tableName")
                        .withSchema(XXXXSchemaFactory.create())
                        .withCreateDisposition(BigQueryIO.Write.CreateDisposition.CREATE_IF_NEEDED)
                        .withWriteDisposition(BigQueryIO.Write.WriteDisposition.WRITE_APPEND));
        p.run();
//}

//listnum[Destinationsのサンプルコード][java]{
import com.google.api.services.bigquery.model.TableRow;
import org.apache.beam.sdk.io.gcp.bigquery.TableDestination;
import org.apache.beam.sdk.transforms.SerializableFunction;
import org.apache.beam.sdk.values.ValueInSingleWindow;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TimeZone;

public class DayPartitionDestinations implements SerializableFunction<ValueInSingleWindow<TableRow>, TableDestination> {
    private final String tablePrefix;

    public DayPartitionDestinations(String tableId) {
        tablePrefix = tableId + "_";
    }

    @Override
    public TableDestination apply(ValueInSingleWindow<TableRow> input) {
        Double timeStamp = (Double)(input.getValue().get("ts"));
        SimpleDateFormat sdf = new java.text.SimpleDateFormat("YYYYMMdd");
        Date date = new Date((long)(timeStamp*1000));
        sdf.setTimeZone(TimeZone.getTimeZone("UTC"));
        return new TableDestination(tablePrefix + sdf.format(date), "datalog");
    }
//}

DynamicDestinationsを利用しても上記と同じようなコードで実現できますので、興味がある方はこちらを参照してみてください。
https://beam.apache.org/documentation/sdks/javadoc/2.0.0/org/apache/beam/sdk/io/gcp/bigquery/DynamicDestinations.html

==== ログの確認

コンソール上から作成したジョブをクリックし、詳細画面からログが確認できます。
stackdriverにも自動でログが転送されていますので、モニタリング、監視などはCloudFunction経由で
整形して送るような形でエウレカでは運用しています。

img

==== エラー(例外発生)時の挙動

DataflowJobを実行中にExceptionが発生した場合、PubSubにACKを送らないので再度データが取り出される事になります。
従って

 - BigQueryへのロード部分等で処理がこけてもPubSubのメッセージが破棄されないので自動でリトライされる(通信レベルでのリトライの考慮は不要)
 - 反面でデータが正しくない場合はPubSubにデータが残り続ける限り、エラーを吐き続ける事になるので、Validationをかけてエラーデータを破棄する等のハンドリングを適切に行う必要があります。(何回かエラーが溢れて涙目になりました。)

==== BigQueryのQuota上限

出力先をBigQueryにとる場合はBigQueryのQuota上限にあたらないかは注意する必要があります。
（よほどのデータ量でなければ大丈夫だと思いますが。
あまりデータ量が多い場合、Windowでデータを集計して減らす、Quotaの引き上げをGoogleに打診する等
をしたほうがよいです。

https://cloud.google.com/bigquery/quotas#streaming_inserts

== まとめ

さて、まだまだ色々と説明できていない部分もありますが、ちょっとしたサンプルを基に
今回CloudDataFlowの使い方や良さについて語ってみました。

最初は手探りになると思いますので、github上のwordCountのsampleや
apacheBeamのリファレンスやSDKのJavaDocを参考にすると理解が進むと思います :)

私もまだまだ研究中ですので、この記事を読んで
以下のような気持ちや特徴をお持ちの方は、カジュアルランチもやってますので
是非お気軽にエウレカを訪問ください!

* わしのデータフローは108段あるぞ
* おい、そんなデータ処理で大丈夫か？
* 一緒に全国大会に出たい
* メガネっ娘である

== 参考にしたサイトや役に立った情報

https://github.com/GoogleCloudPlatform/DataflowSDK-examples
https://beam.apache.org/
https://cloud.google.com/dataflow/?hl=ja
