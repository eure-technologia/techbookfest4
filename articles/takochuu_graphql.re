= GraphQLあるいはWebAPIインターフェースについて

== 自己紹介
こんにちは！はじめまして。
株式会社エウレカでPairsのサーバーサイドを書いています。@takochuuです。@<br>{}
主にSPA / ネイティブアプリ向けのAPIを書くことが多いので、GraphQLをはじめとした
APIに用いられる技術と最近着目されているAPI向けの技術に関してこの章を書きたいと思います。


== はじめに
本章ではWeb APIに用いられる技術について解説します。@<br>{}
RESTの流行と共に、json形式でのRequest / Responseが主流となっていますがGraphQL / Protocol Bufferのような新たな潮流が生まれています。@<br>{}
本章はこのうちGraphQLに焦点を当て、goでサーバーサイドの実装を行います。


== Web API インターフェースの歴史
APIとはいわずもがな、Application Programming Interface（アプリケーション・プログラミング・インタフェース）の略です。@<br>{}
Web界隈ではサービスの内外からCRUD操作を可能にする役割として浸透していますが、元々はOSレベルの用語です。@<br>{}
@<br>{}

Webの世界にAPIの概念が持ち込まれたのは、Ajaxの発見によるものが大きいと筆者は考えています。@<br>{}
Ajaxの発見により、WebServerがレンダリングされたHTMLを返すだけのものから、jsonを用いてデータを返却するものへと変化を遂げます。@<br>{}
上記の発見によりデータと表現としてのコードが分割されることになり、今日のSPAやネイティブアプリの発展につながっています。@<br>{}
今でこそ使用されるプロトコル/データ形式はhttp/jsonが一般的になっていますが、それまではSOAP/XMLを利用していました。@<br>{}
SOAPでは、プラットフォームに依存しないプロトコルが採用されています。@<br>{}
SOAPで採用されているXMLには冗長性があり、Javascriptや動的型付け言語で扱うには面倒だという側面があります。@<br>{}
それに対してRESTで採用されることが多いjsonは、定義の完結さやJavascriptでそのまま扱えるということもあり、急速に普及することになりました。@<br>{}


== 課題
RESTはRoyFielding氏が2000年に発表した論文（@<href>{https://en.wikipedia.org/wiki/Representational_state_transfer}）に基づいていますがHTTP上の対話モデルをアーキテクチャとして定義しているに過ぎず、endpointのpathの定義の方法やバージョニング、エラー処理等については何の言及もありません。@<br>{}
実際のプロダクト開発として考えると、次のような問題に直面したみなさんも多いのではないでしょうか。@<br>{}
@<br>{}
1. 書く人によってendpointの命名方法が異なる@<br>{}
2. Request/Responseに一貫したSchema定義がないことにより、大規模開発時に一貫性が失われる（ところによりSchema定義を別途書くなどの面倒さを抱えることになる）@<br>{}
3. RESTで対応仕切れない部分が出て来る（GETでの更新処理など）@<br>{}
4. 欲しい情報だけではなく、不要な情報も返してしまっている@<br>{}
@<br>{}
上記のような状態に悩まされている読者の方もいらっしゃるのではないでしょうか。@<br>{}
実際、筆者が今まで開発してきたプロダクトもこのような状態で悩まされていました。


== GraphQLとは何か
上記のような状態を解決するために筆者が注目している技術のひとつとして、GraphQLがあります。@<br>{}
GraphQLとはFacebookが開発しているクエリ言語で、 2015年のReact.js Confで発表されました。@<br>{}
近年ではGithubのv4APIにて採用されていることもあり徐々に浸透しつつある技術の1つではないでしょうか。

//image[GraphQL][GraphQL]{
//}

早速サンプルとしてGithubのv4APIに対してGraphiQLを用いてデータを取得をしてみましょう。@<br>{}
https://developer.github.com/v4/explorer/


このような形でクエリを書いて実行するとサーバーからjson形式で書いたクエリに対応したレスポンスが返ってきていることがわかります。


//image[GraphiQL][デフォルトのクエリ]{
//}


クエリを書き換え、このようにしてみると返却されるレスポンスが変化しています。


//image[GrapiQL_mod][クエリを変更後]{
//}


GrapiQLは、次のようにドキュメントを閲覧する機能も実装しており、非常に強力なツールになっています。@<br>{}
※これがないと開発が進まないという程に

//image[GrapiQL_Document][右側にドキュメントが表示されます]{
//}

== GraphQLを活用することの利点
では、GraphQLを使うと何が嬉しいのでしょうか？@<br>{}
おさらいとして、RESTを用いたWebAPIではエンドポイントが異なる場合には必要なデータを取得するために複数回httpリクエストを送る必要がありました。@<br>{}
その結果、欲しいデータだけではなく不必要なデータも受け取っており、必要以上にペイロードが大きくなってしまうことがあります。
Schemaの定義次第ですが、さきほど提示したサンプルのように欲しい情報を一度のリクエストで取得することが可能です。これにより、ペイロードのサイズを小さくすることが可能です。@<br>{}
@<br>{}
そして、2つ目にはRequestのSchemaが定義できることが大きい利点です。@<br>{}
Web開発あるあるネタですが、「あれっ、このAPIどんな値が返ってくるの？使われてる？」みたいなことがなくなるのでネイティブ / Webフロントエンドとの認識統一が優位になります。@<br>{}
（技術で解決できる問題をコミュニケーションで解決するのは面倒ですよね）


== GraphQLのgo実装（サーバーサイド）
ここからはいよいよ、goを使ってサーバーサイドの実装解説に入ります。@<br>{}
今回、GraphQLのライブラリとして [github.com/graphql-go/graphql]（https://github.com/graphql-go/graphql） を利用します。@<br>{}
現在頻繁にcommitされており、goでGraphQLを扱う際のライブラリではstar数が最大となっているのでこちらを利用していきましょう。@<br>{}
@<br>{}
GraphQLを用いて読み込みを行う場合は、Queryという構文を使用します。@<br>{}
簡単にサーバーサイドの処理を実装してみます。@<br>{}

=== データ取得

まず、GraphQLを利用するためSchemaを定義する必要があります。@<br>{}
今回は簡単に name / id という名前でデータを引けるようにSchemaを定義します。

//emlist[][]{
package main

import (
  "github.com/graphql-go/graphql"
)

var q graphql.ObjectConfig = graphql.ObjectConfig{
  Name: "query",
  Fields: graphql.Fields{
    "id": &graphql.Field{
      Type:    graphql.ID,
      Resolve: resolveID,
    },
    "name": &graphql.Field{
      Type:    graphql.String,
      Resolve: resolveName,
    },
  },
}

var schemaConfig graphql.SchemaConfig = graphql.SchemaConfig{
  Query: graphql.NewObject（q）,
}

var schema, _ = graphql.NewSchema（schemaConfig）
//}

まず、このようにしてSchemaを定義します。
次に、http経由でクエリを受け付けられるように実装します。


//emlist[][]{
package main

import (
  "bytes"
  "encoding/json"
  "fmt"
  "net/http"

  "github.com/graphql-go/graphql"
)

var q graphql.ObjectConfig = graphql.ObjectConfig{
  Name: "query",
  Fields: graphql.Fields{
    "id": &graphql.Field{
      Type:    graphql.ID,
      Resolve: resolveID,
    },
    "name": &graphql.Field{
      Type:    graphql.String,
      Resolve: resolveName,
    },
  },
}

var schemaConfig graphql.SchemaConfig = graphql.SchemaConfig{
  Query: graphql.NewObject（q）,
}
var schema, _ = graphql.NewSchema（schemaConfig）

func executeQuery（query string, schema graphql.Schema） *graphql.Result {
  r := graphql.Do(graphql.Params{
    Schema:        schema,
    RequestString: query,
  })

  if len（r.Errors） > 0 {
    fmt.Printf（"エラーがあるよ: %v", r.Errors）
  }

  j, _ := json.Marshal（r）
  fmt.Printf（"%s \n", j）

  return r
}

func handler（w http.ResponseWriter, r *http.Request） {
  bufBody := new（bytes.Buffer）
  bufBody.ReadFrom（r.Body）
  query := bufBody.String()

  result := executeQuery（query, schema）
  json.NewEncoder（w）.Encode（result）
}

func main() {
  http.HandleFunc（"/", handler）
  http.ListenAndServe（":8080", nil）
}

func resolveID（p graphql.ResolveParams） （interface{}, error） {
  return 1, nil
}

func resolveName（p graphql.ResolveParams） （interface{}, error） {
  return "hoge", nil
}
//}

goを書いたプログラマにはおなじみですが、@<code>{http.HandleFunc}を用いて@<code>{localhost:8080}にてアクセスを受けられるようにしています。@<br>{}
このように schemaを定義して@<code>{graphql.Do} にschemaとクエリを渡すことで処理します。
@<code>{go run main.go}でサーバーを立てて、curlでデータを取得してみます。

//cmd{
#!/bin/bash
curl -X POST -d '{ id }' http://localhost:8080/
curl -X POST -d '{ name }' http://localhost:8080/
curl -X POST -d '{ id, name }' http://localhost:8080/
//}

サーバーから返却されたレスポンスはこちらです

//cmd{
{"data":{"id":"1"}}
{"data":{"name":"hoge"}}
{"data":{"id":"1","name":"hoge"}}
//}

上記のように、curlで指定したfieldと対応する値が返却されていることがわかります。

//emlist[][]{
"id": &graphql.Field{
  Type:    graphql.ID,
  Resolve: resolveID,
},
//}

指定されたfieldに対するvalueはResolveに対して定義している@<code>{resolveID} と @<code>{resolveName} で値を生成して返却しています。@<br>{}
Queryに引数を渡すこともできますので、試してみましょう。@<br>{}
さきほど記述した、サーバーサイドのコードを一部書き換えて次のようにします。

//emlist[][]{
"id": &graphql.Field{
  "id": &graphql.Field{
    Type: graphql.ID,
    Args: graphql.FieldConfigArgument{
      "id": &graphql.ArgumentConfig{
        Type: graphql.Int,
      },
    },
    Resolve: resolveID,
  },
}

func resolveID（p graphql.ResolveParams） （interface{}, error） {
  return p.Args["id"], nil
}
//}

この状態で、さきほど実行したcurlを変更して、次のようなクエリを投げます。

//cmd{
curl -X POST -d '{ id（id: 100）, name }' http://localhost:8080/
//}

レスポンスを見ると、このように引数として渡した100がidのvalueとして帰ってきていることがわかります。

//cmd{
{"data":{"id":"100","name":"hoge"}}
//}

今回は端折っていますが、具体的にサービスを実装する際は @<code>{Resolve} にマッピングした関数に渡ってきた引数を利用して、データを引っ張ることになります。

=== 書き込み

GraphQLでは、読み込み時はQueryを使用していましたが、書き込み等のWrite操作を行う際にはMutationを使用します。@<br>{}
今から提示するソースコードは、書き込みを行う実装を紹介します。

//emlist[][]{
var m graphql.ObjectConfig = graphql.ObjectConfig{
  Name: "User",
  Fields: graphql.Fields{
    "user": &graphql.Field{
      Type: graphql.NewObject(graphql.ObjectConfig{
        Name: "Params",
        Fields: graphql.Fields{
          "id": &graphql.Field{
            Type: graphql.Int,
          },
          "address": &graphql.Field{
            Type: graphql.NewObject(graphql.ObjectConfig{
              Name: "state",
              Fields: graphql.Fields{
                "state": &graphql.Field{
                  Type: graphql.String,
                },
                "city": &graphql.Field{
                  Type: graphql.String,
                },
              },
            }),
          },
        },
      }),
      Args: graphql.FieldConfigArgument{
        "id": &graphql.ArgumentConfig{
          Type: graphql.Int,
        },
      },
      Resolve: func(p graphql.ResolveParams) (interface{}, error) {
        // ここで更新処理をする
        return User{
          Id: 10000,
          Address: Address{
            State: "三宿",
            City:  "世田谷区",
          },
        }, nil
      },
    },
  },
}

type User struct {
  Id      int64 `json:"id"`
  Address Address `json:"address"`
}

type Address struct {
  State string `json:"state"`
  City  string `json:"city"`
}

var schemaConfig graphql.SchemaConfig = graphql.SchemaConfig{
  Query:    graphql.NewObject(q),
  Mutation: graphql.NewObject(m),
}
//}

さきほどのソースコードを改造して、@<code>{graphql.SchemaConfig} に対してMutationフィールドを定義をします。@<br>{}
その上で、同じように@<code>{graphql.ObjectConfig}を定義してあげます。@<br>{}

上記ソースコードのように構造体を定義し`tag`でマッピングすることも可能です。@<br>{}
早速クエリを投げてみましょう。

//cmd{
curl -X POST -d 'mutation { user(id: 100){ id, address{ state, city }}}' http://localhost:8080/
//}

//cmd{
{"data":{"user":{"address":{"city":"世田谷区","state":"三宿"},"id":10000}}}
//}

このように、渡した内容に応じてサーバーサイドで計算して値を返すことが可能になっています。@<br>{}
本来はRDBMSに書き込みを行ったりという形ですが今回は簡略化して書いています。

== おわりに
このように、書き込み / 読み込みともに柔軟にフロントエンドから実行できることがGraphQLの大きな利点となります。@<br>{}
現在はまだProduction環境にGraphQLを適用しているサービスは多くないと思いますが、まだまだこれから普及の余地がある技術だと感じます。@<br>{}
REST / GraphQLとどちらがいいではなく適材適所だと思うので、普及したタイミングやサービスへ導入したタイミングでまた話ができればと思います。@<br>{}
@<br>{}
それでは、楽しんでいただけましたか？ありがとうございました！またよろしくおねがいします！
